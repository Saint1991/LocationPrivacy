
namespace Graph
{
	
	///<summary>
	/// コンストラクタ
	///</summary>
	template <typename NODE, typename POI, typename PATH>
	Map<NODE, POI, PATH>::Map()
		: node_collection(std::make_shared<const Collection::IdentifiableCollection<Graph::node_id, NODE>>()),
		  poi_collection(std::make_shared<Collection::IdentifiableCollection<Graph::node_id, POI>>())
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
	void Map<NODE, POI, PATH>::initialize(std::unique_ptr<RoutingMethod<NODE, PATH>> routing_method)
	{
		build_map();
		routing_table = routing_method->create_routing_table(node_collection);
		build_rtree_index();
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
	/// fromからtoまでの最短パスのうち，INTERSECTION系列部分の先頭と末尾を取得する
	/// first=>先頭, second=>末尾
	///</summary>
	template <typename NODE, typename POI, typename PATH>
	std::pair<node_id, node_id> Map<NODE, POI, PATH>::get_intersection_ends_of_shortest_path(const MapNodeIndicator& from, const MapNodeIndicator& to) const
	{

		//両方INTERSECTIONの場合
		if (from.type() == NodeType::INTERSECTION && to.type() == NodeType::INTERSECTION) {
			return std::make_pair(from.id(), to.id());
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

			double distance_first_first = distance_poi_from_to_first == NO_CONNECTION || distance_poi_to_to_first == NO_CONNECTION ?
				NO_CONNECTION : distance_poi_from_to_first + distance_poi_to_to_first + routing_table->shortest_distance(between_from.first, between_to.first);
			double distance_first_second = distance_poi_from_to_first == NO_CONNECTION || distance_poi_to_to_second == NO_CONNECTION ?
				NO_CONNECTION : distance_poi_from_to_first + distance_poi_to_to_second + routing_table->shortest_distance(between_from.first, between_to.second);
			double distance_second_first = distance_poi_from_to_second == NO_CONNECTION || distance_poi_to_to_first == NO_CONNECTION ?
				NO_CONNECTION : distance_poi_from_to_second + distance_poi_to_to_first + routing_table->shortest_distance(between_from.second, between_to.first);
			double distance_second_second = distance_poi_from_to_second == NO_CONNECTION || distance_poi_to_to_second == NO_CONNECTION ?
				NO_CONNECTION : distance_poi_from_to_second + distance_poi_to_to_second + routing_table->shortest_distance(between_from.second, between_to.second);
			
			std::pair<node_id, node_id> ret = std::make_pair(NOWHERE, NOWHERE);
			
			std::unordered_map<double, std::pair<node_id, node_id>> values = {
				{ distance_first_first, std::make_pair(between_from.first, between_to.first) },
				{ distance_first_second, std::make_pair(between_from.first, between_to.second) },
				{ distance_second_first, std::make_pair(between_from.second, between_to.first) },
				{ distance_second_second, std::make_pair(between_from.second, between_to.second) }
			};

			double min_distance = DBL_MAX;
			for (std::unordered_map<double, std::pair<node_id, node_id>>::const_iterator iter = values.begin(); iter != values.end(); iter++) {
				if (iter->first < min_distance) {
					min_distance = iter->first;
					ret = iter->second;
				}
			}
			return ret;
		}

		//一方のみPOIの場合
		else {

			node_id poi_id = from.type() == NodeType::POI ? from.id() : to.id();

			std::shared_ptr<POI const> poi = poi_collection->read_by_id(poi_id);
			std::pair<node_id, node_id> between = poi->between();
			double distance_to_first = poi->distance_to(between.first);
			double distance_to_second = poi->distance_to(between.second);

			double distance_path_to_first = from.type() == NodeType::POI ? routing_table->shortest_distance(between.first, to.id()) : routing_table->shortest_distance(from.id(), between.first);
			double distance_path_to_second = from.type() == NodeType::POI ? routing_table->shortest_distance(between.second, to.id()) : routing_table->shortest_distance(from.id(), between.second);

			distance_to_first = distance_to_first == NO_CONNECTION || distance_path_to_first == NO_CONNECTION ?
				NO_CONNECTION : distance_to_first + distance_path_to_first;
			distance_to_second = distance_to_second == NO_CONNECTION || distance_path_to_second == NO_CONNECTION ?
				NO_CONNECTION : distance_to_second + distance_path_to_second;

			if (distance_to_first < distance_to_second) {
				return from.type() == NodeType::POI ? std::make_pair(between.first, to.id()) : std::make_pair(from.id(), between.first);
			}
			else {
				return from.type() == NodeType::POI ? std::make_pair(between.second, to.id()) : std::make_pair(from.id(), between.second);
			}
		}
		return std::make_pair(NOWHERE, NOWHERE);
	}

	///<summary>
	/// fromからtoまでの最短距離を求める
	/// オーバーフロー周りのバグに注意
	///</summary>
	template <typename NODE, typename POI, typename PATH>
	double Map<NODE, POI, PATH>::shortest_distance(const MapNodeIndicator& from, const MapNodeIndicator& to) const
	{
		//両方INTERSECTIONの場合
		if (from.type() == NodeType::INTERSECTION && to.type() == NodeType::INTERSECTION) {
			return routing_table->shortest_distance(from.id(), to.id());
		}
		
		std::pair<node_id, node_id> intersection_ends = get_intersection_ends_of_shortest_path(from, to);
		double route_distance = routing_table->shortest_distance(intersection_ends.first, intersection_ends.second);
		if (route_distance == NO_CONNECTION) return NO_CONNECTION;

		//fromのみPOIの場合
		if (from.type() == NodeType::POI && to.type() == NodeType::INTERSECTION){
			std::shared_ptr<POI const> poi_from = poi_collection->read_by_id(from.id());
			double distance_to_first = poi_from->distance_to(intersection_ends.first);
			return  distance_to_first + route_distance;
		}

		//toのみPOIの場合
		else if (from.type() == NodeType::INTERSECTION && to.type() == NodeType::POI) {
			std::shared_ptr<POI const> poi_to = poi_collection->read_by_id(to.id());
			double distance_to_last = poi_to->distance_to(intersection_ends.second);
			return route_distance + distance_to_last;
		}

		//両方POIの場合
		else {
			std::shared_ptr<POI const> poi_from = poi_collection->read_by_id(from.id());
			std::shared_ptr<POI const> poi_to = poi_collection->read_by_id(to.id());
			double distance_to_first = poi_from->distance_to(intersection_ends.first);
			double distance_to_last = poi_to->distance_to(intersection_ends.second);
			return distance_to_first + route_distance + distance_to_last;
		}
	}

	

	///<summary>
	/// MapNodeIndicatorの系列で経路を返す．
	/// source == destinationの場合は空のリストが返されます．
	/// 指定が不正な場合はNOWHEREのみをふくむVectorを返す
	///</summary>
	template <typename NODE, typename POI, typename PATH>
	const std::vector<MapNodeIndicator> Map<NODE, POI, PATH>::get_shortest_path(const MapNodeIndicator& source, const MapNodeIndicator& destination) const
	{

		std::vector<MapNodeIndicator> ret;
		std::vector<node_id> intersection_path;
		
		std::pair<node_id, node_id> intersection_ends = get_intersection_ends_of_shortest_path(source, destination);

		if (source.type() == NodeType::POI) {
			ret.push_back(MapNodeIndicator(intersection_ends.first, NodeType::INTERSECTION));
		}
		
		intersection_path = routing_table->get_shortest_path(intersection_ends.first, intersection_ends.second);
		for (std::vector<node_id>::const_iterator iter = intersection_path.begin(); iter != intersection_path.end(); iter++) {
			ret.push_back(MapNodeIndicator(*iter, NodeType::INTERSECTION));
		}
		
		if (destination.type() == NodeType::POI) ret.push_back(destination);
		return ret;
	}

	

	///<summary>
	/// fromからtoまで平均速度avg_speed[m/s]で移動した際の所要時間を計算します
	/// 到達不可能な場合はNO_CONNECTIONを返します．
	///</summary>
	template <typename NODE, typename POI, typename PATH>
	double Map<NODE, POI, PATH>::calc_necessary_time(const MapNodeIndicator& from, const MapNodeIndicator& to, const double& avg_speed) const
	{
		double distance = shortest_distance(from, to);
		return distance / avg_speed;
	}


	///<summary>
	/// fromからtoへ平均速度speed[m/s]で移動したときの制限時間time_limit[s]の到達可能性を調べる
	///</summary>
	template <typename NODE, typename POI, typename PATH>
	bool Map<NODE, POI, PATH>::is_reachable(const MapNodeIndicator& from, const MapNodeIndicator& to, const double& avg_speed, const double& time_limit) const
	{
		return calc_necessary_time(from, to, avg_speed) <= time_limit;
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

