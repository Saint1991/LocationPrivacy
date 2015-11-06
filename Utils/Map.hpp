
namespace Graph
{
	
	///<summary>
	/// コンストラクタ
	///</summary>
	template <typename NODE, typename POI, typename PATH>
	Map<NODE, POI, PATH>::Map(std::shared_ptr<IRoutingModule<NODE, PATH>> routing_method)
		: node_collection(nullptr),
		  poi_collection(nullptr),
		  routing_method(std::move(routing_method))
	{		
	}

	///<summary>
	/// デストラクタ
	///</summary>
	template <typename NODE, typename POI, typename PATH>
	Map<NODE, POI, PATH>::~Map()
	{
	}


	///<summary>
	/// Mapの初期化
	/// 最初に必ずこれを呼ぶ必要があることに注意
	///</summary>
	template <typename NODE, typename POI, typename PATH>
	void Map<NODE, POI, PATH>::load(const Graph::Rectangle<Geography::LatLng>& boundary)
	{
		
		std::cout << "Boundary: " << std::to_string(boundary.height()) << "m ×" << std::to_string(boundary.width()) << "m" << std::endl;

		node_collection = std::make_shared<const Collection::IdentifiableCollection<Graph::node_id, NODE>>();
		poi_collection = std::make_shared<const Collection::IdentifiableCollection<Graph::node_id, POI>>();

		std::cout << "Loading Nodes and connectivities"  << std::endl;
		build_map(boundary);
		std::cout << "Complete! " << node_collection->size() << " Nodes Loaded." << std::endl;

		std::cout << "Setup Routing Client" << std::endl;
		routing_client = std::make_unique<RoutingClient<NODE, PATH>>(node_collection, routing_method);
		std::cout << "Complete!" << std::endl;

		std::cout << "Start Building R-Tree Index." << std::endl;
		build_rtree_index();
		std::cout << "Comlete! Map Loaded." << std::endl;
	}

	///<summary>
	/// R-Treeのインデックスを構築する
	///</summary>
	template <typename NODE, typename POI, typename PATH>
	void Map<NODE, POI, PATH>::build_rtree_index()
	{
		boost::geometry::index::quadratic<16> param;
		MyPoiAdapter adapter;
		rtree_index = std::make_unique<rtree>(param, adapter);
		
		poi_collection->foreach([&](rtree_value poi) {
			rtree_index->insert(poi);
		});
	}
	

	///<summary>
	/// fromからtoまでの最短ルートの系列とその距離を返す．
	/// 不正な値の場合はMapNodeIndicator(INVALID, NodeType::INVALID)の系列とNO_CONNECTIONを返す
	/// distance_thresholdを設定した場合，最短距離がその値以上とわかった時点で探索を打ち切り上記を返す
	///</summary>
	template <typename NODE, typename POI, typename PATH>
	std::pair<std::vector<MapNodeIndicator>, double> Map<NODE, POI, PATH>::get_shortest_path_info(const MapNodeIndicator& from, const MapNodeIndicator& to, double distance_threshold) const
	{

		std::vector<MapNodeIndicator> ret_route;
		double ret_distance = NO_CONNECTION;

		//両方INTERSECTIONの場合
		if (from.type() == NodeType::INTERSECTION && to.type() == NodeType::INTERSECTION) {
			RouteInfo<PATH> info = routing_client->shortest_path_info(from.id(), to.id(), distance_threshold);
			ret_distance = info.total_distance;
			for (std::vector<node_id>::const_iterator iter = info.route->begin(); iter != info.route->end(); iter++) {
				ret_route.push_back(MapNodeIndicator(*iter, NodeType::INTERSECTION));
			}
		}

		//両方POIの場合
		else if (from.type() == NodeType::POI && to.type() == NodeType::POI) {
			
			std::shared_ptr<POI const> poi_from = poi_collection->read_by_id(from.id());
			std::shared_ptr<POI const> poi_to = poi_collection->read_by_id(to.id());
			std::pair<node_id, node_id> between_from = poi_from->between();
			std::pair<node_id, node_id> between_to = poi_to->between();
			double distance_poi_from_to_first = poi_from->distance_to(between_from.first);
			double distance_poi_from_to_second = poi_from->distance_to(between_from.second);
			double distance_poi_to_to_first = poi_to->distance_to(between_to.first);
			double distance_poi_to_to_second = poi_to->distance_to(between_to.second);

			RouteInfo<PATH> first_first = routing_client->shortest_path_info(between_from.first, between_to.first, distance_threshold);
			RouteInfo<PATH> first_second = routing_client->shortest_path_info(between_from.first, between_to.second, distance_threshold);
			RouteInfo<PATH> second_first = routing_client->shortest_path_info(between_from.second, between_to.first, distance_threshold);
			RouteInfo<PATH> second_second = routing_client->shortest_path_info(between_from.second, between_to.second, distance_threshold);

			first_first.total_distance += first_first.total_distance == DBL_MAX || distance_poi_from_to_first == DBL_MAX || distance_poi_to_to_first == DBL_MAX ? 0 : distance_poi_from_to_first + distance_poi_to_to_first;
			first_second.total_distance += first_second.total_distance == DBL_MAX || distance_poi_from_to_first == DBL_MAX || distance_poi_to_to_second == DBL_MAX ? 0 : distance_poi_from_to_first + distance_poi_to_to_second;
			second_first.total_distance += second_first.total_distance == DBL_MAX || distance_poi_from_to_second == DBL_MAX || distance_poi_to_to_first == DBL_MAX ? 0 : distance_poi_from_to_second + distance_poi_to_to_first;
			second_second.total_distance += second_second.total_distance == DBL_MAX || distance_poi_from_to_second == DBL_MAX || distance_poi_to_to_second == DBL_MAX ? 0 : distance_poi_from_to_second + distance_poi_to_to_second;
			
			std::vector<std::pair<node_id, RouteInfo<PATH>>> route_list({
				{ between_from.first, first_first },
				{ between_from.first, first_second },
				{ between_from.second, second_first },
				{ between_from.second, second_second }
			});

			std::vector<std::pair<node_id, RouteInfo<PATH>>>::const_iterator min_route = std::min_element(route_list.begin(), route_list.end(), [](const std::pair<node_id, RouteInfo<PATH>>& left, const std::pair<node_id, RouteInfo<PATH>>& right) {
				return left.second.total_distance < right.second.total_distance;
			});
			
			ret_distance = min_route->second.total_distance;
			if (min_route->second.route != nullptr) {
				ret_route.push_back(MapNodeIndicator(min_route->first, NodeType::INTERSECTION));
				for (std::vector<node_id>::const_iterator iter = min_route->second.route->begin(); iter != min_route->second.route->end(); iter++) {
					ret_route.push_back(MapNodeIndicator(*iter, NodeType::INTERSECTION));
				}
				ret_route.push_back(to);
			}
		}

		//一方のみPOIの場合
		else {

			node_id poi_id = from.type() == NodeType::POI ? from.id() : to.id();

			std::shared_ptr<POI const> poi = poi_collection->read_by_id(poi_id);
			std::pair<node_id, node_id> between = poi->between();
			double distance_to_first = poi->distance_to(between.first);
			double distance_to_second = poi->distance_to(between.second);

			RouteInfo<PATH> route_first = from.type() == NodeType::POI ? routing_client->shortest_path_info(between.first, to.id(), distance_threshold) : routing_client->shortest_path_info(from.id(), between.first, distance_threshold);
			RouteInfo<PATH> route_second = from.type() == NodeType::POI ? routing_client->shortest_path_info(between.second, to.id(), distance_threshold) : routing_client->shortest_path_info(from.id(), between.second, distance_threshold);

			route_first.total_distance += route_first.total_distance == NO_CONNECTION || distance_to_first == NO_CONNECTION ? 0 : distance_to_first;
			route_second.total_distance += route_second.total_distance == NO_CONNECTION || distance_to_second == NO_CONNECTION ? 0 : distance_to_second;

			RouteInfo<PATH> min_route = route_first.total_distance < route_second.total_distance ? route_first : route_second;
			ret_distance = min_route.total_distance;
			if (min_route.route != nullptr) {
				node_id between_which = route_first.total_distance < route_second.total_distance ? between.first : between.second;
				if (from.type() == NodeType::POI) ret_route.push_back(MapNodeIndicator(between_which, NodeType::INTERSECTION));
				for (std::vector<node_id>::const_iterator iter = min_route.route->begin(); iter != min_route.route->end(); iter++) {
					ret_route.push_back(MapNodeIndicator(*iter, NodeType::INTERSECTION));
				}
				if (to.type() == NodeType::POI) ret_route.push_back(to);
			}
		}

		if (ret_distance == NO_CONNECTION) ret_route = { MapNodeIndicator(-1, NodeType::INVALID) };
		return std::make_pair(ret_route, ret_distance);
	}

	///<summary>
	/// fromからtoまでの最短距離を求める
	/// オーバーフロー周りのバグに注意
	/// distance_thresholdが設定されている場合は最短路がその値を超えている場合探索を打ち切りNO_CONNECTIONを返す
	///</summary>
	template <typename NODE, typename POI, typename PATH>
	double Map<NODE, POI, PATH>::shortest_distance(const MapNodeIndicator& from, const MapNodeIndicator& to, double distance_threshold) const
	{
		return get_shortest_path_info(from, to, distance_threshold).second;
	}

	

	///<summary>
	/// MapNodeIndicatorの系列で経路を返す．
	/// source == destinationの場合は空のリストが返されます．
	/// distance_thresholdを設定した場合，最短路の長さがdistance_threshold以上の場合NOWHERE, NodeType::INVALIDのみのVectorを返す
	/// from, toが不正な値の場合も同様
	///</summary>
	template <typename NODE, typename POI, typename PATH>
	std::vector<MapNodeIndicator> Map<NODE, POI, PATH>::get_shortest_path(const MapNodeIndicator& source, const MapNodeIndicator& destination, double distance_threshold) const
	{
		return get_shortest_path_info(source, destination, distance_threshold).first;
	}

	

	///<summary>
	/// fromからtoまで平均速度avg_speed[m/s]で移動した際の所要時間を計算します
	/// 到達不可能な場合および最短路の距離がdistance_thresholdより大きい場合はNO_CONNECTIONを返します．
	/// 
	///</summary>
	template <typename NODE, typename POI, typename PATH>
	double Map<NODE, POI, PATH>::calc_necessary_time(const MapNodeIndicator& from, const MapNodeIndicator& to, const double& avg_speed, double distance_threshold) const
	{
		double distance = shortest_distance(from, to, distance_threshold);
		return distance == DBL_MAX ? NO_CONNECTION : distance / avg_speed;
	}


	///<summary>
	/// fromからtoへ平均速度speed[m/s]で移動したときの制限時間time_limit[s]の到達可能性を調べる
	///</summary>
	template <typename NODE, typename POI, typename PATH>
	bool Map<NODE, POI, PATH>::is_reachable(const MapNodeIndicator& from, const MapNodeIndicator& to, const double& avg_speed, const double& time_limit) const
	{
		double distance_threshold = avg_speed * time_limit;
		return calc_necessary_time(from, to, avg_speed, distance_threshold) <= time_limit;
	}


	///<summary>
	/// 領域内に含まれるPOI一覧を取得する
	///</summary>
	template <typename NODE, typename POI, typename PATH>
	std::vector<std::shared_ptr<POI const>> Map<NODE, POI, PATH>::find_pois_within_boundary(const box& boundary) const
	{
		std::vector<rtree_value> ret;
		rtree_index->query(index::within(boundary), std::back_inserter(ret));
		return ret;
	}


	///<summary>
	/// 領域内に含まれるPOI一覧を取得する
	///</summary>
	template <typename NODE, typename POI, typename PATH>
	std::vector<std::shared_ptr<POI const>> Map<NODE, POI, PATH>::find_pois_within_boundary(const Rectangle<>& boundary) const
	{
		box query_box(point(boundary.left, boundary.bottom), point(boundary.right, boundary.top));
		return find_pois_within_boundary(query_box);
	}

	///<summary>
	/// 領域内に含まれるPOI一覧を取得する
	///</summary>
	template <typename NODE, typename POI, typename PATH>
	std::vector<std::shared_ptr<POI const>> Map<NODE, POI, PATH>::find_pois_within_boundary(const Rectangle<Geography::LatLng>& boundary) const
	{
		box query_box(point(boundary.left, boundary.bottom), point(boundary.right, boundary.top));
		return find_pois_within_boundary(query_box);
	}

	///<summary>
	/// 指定したIDのノードを変更不可の状態で取得する
	///</summary>
	template <typename NODE, typename POI, typename PATH>
	std::shared_ptr<NODE const> Map<NODE, POI, PATH>::get_static_node(const node_id& id) const
	{
		return node_collection->read_by_id(id);
	}

	///<summary>
	/// 指定したPOIノードを変更不可の状態で取得する
	///</summary>
	template <typename NODE, typename POI, typename PATH>
	std::shared_ptr<POI const> Map<NODE, POI, PATH>::get_static_poi(const node_id& poi_id) const
	{
		return poi_collection->read_by_id(poi_id);
	}
}

