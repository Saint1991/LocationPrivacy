#include "stdafx.h"
#include "HayashidaDbMap.h"


namespace Map {

	///<summary>
	/// �R���X�g���N�^
	/// Routing��WarshallFloyd��p����
	///</summary>
	HayashidaDbMap::HayashidaDbMap(
		std::shared_ptr<Graph::IRoutingModule<BasicMapNode, BasicRoad>> routing_method,
		const std::string& setting_file_path,
		const std::string& db_name,
		const std::string& node_table,
		const std::string& node_connection_table,
		const std::string& poi_table,
		const std::string& poi_connection_table)
		: BasicDbMap(std::move(routing_method),setting_file_path,db_name,node_table,node_connection_table,poi_table,poi_connection_table)
	{
	}


	///<summary>
	/// �f�X�g���N�^
	///</summary>
	HayashidaDbMap::~HayashidaDbMap()
	{
	}

	///<summary>
	/// NODE_TYPE��OTHER�ł��錻�ݒn�����ԋ߂�Node��Ԃ��D
	///</summary>
	std::shared_ptr<Graph::MapNodeIndicator> HayashidaDbMap::get_nearest_node_of_now_position(Geography::LatLng& now_pos) const
	{
		int length = 0.00000001;
		Graph::Rectangle<Geography::LatLng> rect(now_pos.lat() + length, now_pos.lng() - length, now_pos.lat() - length, now_pos.lng());

		std::vector<std::shared_ptr<BasicMapNode const>> nearest_poi = find_nodes_within_boundary(rect);
		while (!nearest_poi.empty()) {
			rect.transform_rect_of_latlang_to_x_times(1.01);
			nearest_poi = find_nodes_within_boundary(rect);
		}
		
		std::sort(nearest_poi.begin(), nearest_poi.end(), [&](std::shared_ptr<BasicMapNode const>& node1, std::shared_ptr<BasicMapNode const>& node2) {
			double dist1 = Geography::GeoCalculation::lambert_distance(now_pos, Geography::LatLng(node1->data->lat(), node1->data->lng()));
			double dist2 = Geography::GeoCalculation::lambert_distance(now_pos, Geography::LatLng(node2->data->lat(), node2->data->lng()));
			return dist1 < dist2;
		});

		return std::make_shared<Graph::MapNodeIndicator>(nearest_poi.front()->get_id(), Graph::NodeType::INTERSECTION);
	}
	
	///<summary>
	/// random�p�X�������
	///</summary>
	HayashidaDbMap::path_info HayashidaDbMap::search_random_path(const Graph::MapNodeIndicator& from, const Graph::MapNodeIndicator& to, double distance_threshold) {

		double theta = M_PI_2;//�߂Â�������s���ۂ̊p�x[rad]
		
		//from��to�����������͋���0.0��Ԃ�
		if (from == to) return std::make_pair(std::vector<Graph::MapNodeIndicator>(), 0.0);
		
		//�H�≓���̌o�H��ݒ肵�Ȃ��悤�ɂ��邽�߂�臒l�ݒ�
		double upper_lat = get_static_node(from.id())->lat();
		double lower_lat = get_static_node(from.id())->lat();
		if (upper_lat < lower_lat) std::swap(upper_lat, lower_lat);
		
		auto check_limit_angle = [=](Geography::LatLng node_position) 
		{
			return node_position.lat() < upper_lat && node_position > lower_lat ? true : false;
		};

		std::vector<Graph::MapNodeIndicator> ret_route;
		double ret_distance = Graph::NO_CONNECTION;
		
		
		//����INTERSECTION�̏ꍇ
		if (from.type() == Graph::NodeType::INTERSECTION && to.type() == Graph::NodeType::INTERSECTION) {
			while(ret_route.back() != to.id()) {
				//from��to��Node���擾
				std::shared_ptr<BasicMapNode const> intersection_info_of_from = get_static_node(from.id());
				std::shared_ptr<BasicMapNode const> intersection_info_of_to = get_static_node(to.id());

				//from�̐ڑ���m�[�h���擾
				//�������poi���܂܂��H
				std::vector<Graph::node_id> connecting_node_list = intersection_info_of_from->get_connecting_node_list();

				std::vector<Graph::node_id> candidate_node_list;
				for (std::vector<Graph::node_id>::iterator iter = connecting_node_list.begin(); iter != connecting_node_list.end(); iter++) {
					//�߂Â��Ă����������l���ł��������������
					double angle_of_iter_to_from = Geography::GeoCalculation::lambert_azimuth_angle(*get_static_node(*iter)->data, *intersection_info_of_from->data);
					double angle_of_iter_to_to = Geography::GeoCalculation::lambert_azimuth_angle(*get_static_node(*iter)->data, *intersection_info_of_to->data);
					double check_angle = std::abs(angle_of_iter_to_from - angle_of_iter_to_to);
					if (check_angle > M_PI) check_angle = std::abs(2 * M_PI - check_angle);
					if (check_angle > theta) {
						candidate_node_list.push_back(*iter);
					}
				}
				if (candidate_node_list.empty()) throw std::invalid_argument("candidate_node_list is empty!!");

				//candidate_node_list���V���b�t�����āC�K���ɂP�I��
				std::random_device device;
				std::mt19937_64 generator(device());
				std::shuffle(candidate_node_list.begin(), candidate_node_list.end(), generator);

				check_limit_angle(*get_static_node(candidate_node_list.front())->data) ?
					ret_route.push_back(candidate_node_list.front()) : ret_route.push_back(to.id());
			}
		}
		
		//����POI�̏ꍇ
		else if (from.type() == Graph::NodeType::POI && to.type() == Graph::NodeType::POI) {
		
		
		}


		//���Ƃ�distance���J�o�[���镔�����L�q����D
		if (ret_distance == Graph::NO_CONNECTION) ret_route = { Graph::MapNodeIndicator(-1, Graph::NodeType::INVALID) };
		


		return std::make_pair(ret_route, ret_distance);		
	}

	///<summary>
	/// from����to�܂ł�random���[�g�̌n��Ƃ��̋�����Ԃ��D
	/// �s���Ȓl�̏ꍇ��Graph::MapNodeIndicator(INVALID, Graph::NodeType::INVALID)�̌n���NO_CONNECTION��Ԃ�
	/// distance_threshold��ݒ肵���ꍇ�C�ŒZ���������̒l�ȏ�Ƃ킩�������_�ŒT����ł��؂��L��Ԃ�
	///</summary>
	std::pair<std::vector<Graph::MapNodeIndicator>, double> HayashidaDbMap::get_random_path_info(const Graph::MapNodeIndicator& from, const Graph::MapNodeIndicator& to, double distance_threshold) const
	{
		if (from == to) return std::make_pair(std::vector<Graph::MapNodeIndicator>(), 0.0);

		std::vector<Graph::MapNodeIndicator> ret_route;
		double ret_distance = Graph::NO_CONNECTION;

		
		//����INTERSECTION�̏ꍇ
		if (from.type() == Graph::NodeType::INTERSECTION && to.type() == Graph::NodeType::INTERSECTION) {
			Graph::RouteInfo<BasicRoad> info = routing_client->shortest_path_info(from.id(), to.id(), distance_threshold);
			
			ret_distance = info.total_distance;
			for (std::vector<Graph::node_id>::const_iterator iter = info.route->begin(); iter != info.route->end(); iter++) {
				ret_route.push_back(Graph::MapNodeIndicator(*iter, Graph::NodeType::INTERSECTION));
			}
		}

		//����POI�̏ꍇ
		else if (from.type() == Graph::NodeType::POI && to.type() == Graph::NodeType::POI) {

			std::shared_ptr<BasicPoi const> poi_from = poi_collection->read_by_id(from.id());
			std::shared_ptr<BasicPoi const> poi_to = poi_collection->read_by_id(to.id());
			std::pair<Graph::node_id, Graph::node_id> between_from = poi_from->between();
			std::pair<Graph::node_id, Graph::node_id> between_to = poi_to->between();
			double distance_poi_from_to_first = poi_from->distance_to(between_from.first);
			double distance_poi_from_to_second = poi_from->distance_to(between_from.second);
			double distance_poi_to_to_first = poi_to->distance_to(between_to.first);
			double distance_poi_to_to_second = poi_to->distance_to(between_to.second);

			Graph::RouteInfo<BasicRoad> first_first = routing_client->shortest_path_info(between_from.first, between_to.first, distance_threshold);
			Graph::RouteInfo<BasicRoad> first_second = routing_client->shortest_path_info(between_from.first, between_to.second, distance_threshold);
			Graph::RouteInfo<BasicRoad> second_first = routing_client->shortest_path_info(between_from.second, between_to.first, distance_threshold);
			Graph::RouteInfo<BasicRoad> second_second = routing_client->shortest_path_info(between_from.second, between_to.second, distance_threshold);

			first_first.total_distance += first_first.total_distance == DBL_MAX || distance_poi_from_to_first == DBL_MAX || distance_poi_to_to_first == DBL_MAX ? 0 : distance_poi_from_to_first + distance_poi_to_to_first;
			first_second.total_distance += first_second.total_distance == DBL_MAX || distance_poi_from_to_first == DBL_MAX || distance_poi_to_to_second == DBL_MAX ? 0 : distance_poi_from_to_first + distance_poi_to_to_second;
			second_first.total_distance += second_first.total_distance == DBL_MAX || distance_poi_from_to_second == DBL_MAX || distance_poi_to_to_first == DBL_MAX ? 0 : distance_poi_from_to_second + distance_poi_to_to_first;
			second_second.total_distance += second_second.total_distance == DBL_MAX || distance_poi_from_to_second == DBL_MAX || distance_poi_to_to_second == DBL_MAX ? 0 : distance_poi_from_to_second + distance_poi_to_to_second;

			std::vector<std::pair<Graph::node_id, Graph::RouteInfo<BasicRoad>>> route_list({
				{ between_from.first, first_first },
				{ between_from.first, first_second },
				{ between_from.second, second_first },
				{ between_from.second, second_second }
			});

			std::vector<std::pair<Graph::node_id, Graph::RouteInfo<BasicRoad>>>::const_iterator min_route = std::min_element(route_list.begin(), route_list.end(), [](const std::pair<Graph::node_id, Graph::RouteInfo<BasicRoad>>& left, const std::pair<Graph::node_id, Graph::RouteInfo<BasicRoad>>& right) {
				return left.second.total_distance < right.second.total_distance;
			});

			ret_distance = min_route->second.total_distance;
			if (min_route->second.route != nullptr) {
				ret_route.push_back(Graph::MapNodeIndicator(min_route->first, Graph::NodeType::INTERSECTION));
				for (std::vector<Graph::node_id>::const_iterator iter = min_route->second.route->begin(); iter != min_route->second.route->end(); iter++) {
					ret_route.push_back(Graph::MapNodeIndicator(*iter, Graph::NodeType::INTERSECTION));
				}
				ret_route.push_back(to);
			}
		}

		//����̂�BasicPoi�̏ꍇ
		else {

			Graph::node_id poi_id = from.type() == Graph::NodeType::POI ? from.id() : to.id();

			std::shared_ptr<BasicPoi const> poi = poi_collection->read_by_id(poi_id);
			std::pair<Graph::node_id, Graph::node_id> between = poi->between();
			double distance_to_first = poi->distance_to(between.first);
			double distance_to_second = poi->distance_to(between.second);

			Graph::RouteInfo<BasicRoad> route_first = from.type() == Graph::NodeType::POI ? routing_client->shortest_path_info(between.first, to.id(), distance_threshold) : routing_client->shortest_path_info(from.id(), between.first, distance_threshold);
			Graph::RouteInfo<BasicRoad> route_second = from.type() == Graph::NodeType::POI ? routing_client->shortest_path_info(between.second, to.id(), distance_threshold) : routing_client->shortest_path_info(from.id(), between.second, distance_threshold);

			route_first.total_distance += route_first.total_distance == Graph::NO_CONNECTION || distance_to_first == Graph::NO_CONNECTION ? 0 : distance_to_first;
			route_second.total_distance += route_second.total_distance == Graph::NO_CONNECTION || distance_to_second == Graph::NO_CONNECTION ? 0 : distance_to_second;

			Graph::RouteInfo<BasicRoad> min_route = route_first.total_distance < route_second.total_distance ? route_first : route_second;
			ret_distance = min_route.total_distance;
			if (min_route.route != nullptr) {
				Graph::node_id between_which = route_first.total_distance < route_second.total_distance ? between.first : between.second;
				if (from.type() == Graph::NodeType::POI) ret_route.push_back(Graph::MapNodeIndicator(between_which, Graph::NodeType::INTERSECTION));
				for (std::vector<Graph::node_id>::const_iterator iter = min_route.route->begin(); iter != min_route.route->end(); iter++) {
					ret_route.push_back(Graph::MapNodeIndicator(*iter, Graph::NodeType::INTERSECTION));
				}
				if (to.type() == Graph::NodeType::POI) ret_route.push_back(to);
			}
		}

		if (ret_distance == Graph::NO_CONNECTION) ret_route = { Graph::MapNodeIndicator(-1, Graph::NodeType::INVALID) };
		
		return std::make_pair(ret_route, ret_distance);
	}

	///<summary>
	/// R-Tree�̃C���f�b�N�X���\�z����
	///</summary>
	void HayashidaDbMap::build_rtree_index_of_node()
	{
		boost::geometry::index::quadratic<16> param;
		MyNodeAdapter adapter;
		rtree_index_of_node = std::make_unique<rtree>(param, adapter);

		node_collection->foreach([&](rtree_value node) {
			rtree_index_of_node->insert(node);
		});
	}

	///<summary>
	/// �̈���Ɋ܂܂��Node�ꗗ���擾����
	///</summary>
	std::vector<std::shared_ptr<BasicMapNode const>> HayashidaDbMap::find_nodes_within_boundary(const box& boundary) const
	{
		std::vector<rtree_value> ret;
		rtree_index_of_node->query(index::intersects(boundary), std::back_inserter(ret));
		return ret;
	}


	///<summary>
	/// �̈���Ɋ܂܂��Node�ꗗ���擾����
	///</summary>
	std::vector<std::shared_ptr<BasicMapNode const>> HayashidaDbMap::find_nodes_within_boundary(const Graph::Rectangle<>& boundary) const
	{
		box query_box(point(boundary.left, boundary.bottom), point(boundary.right, boundary.top));
		return find_nodes_within_boundary(query_box);
	}

	///<summary>
	/// �̈���Ɋ܂܂��Node�ꗗ���擾����
	///</summary>
	std::vector<std::shared_ptr<BasicMapNode const>> HayashidaDbMap::find_nodes_within_boundary(const Graph::Rectangle<Geography::LatLng>& boundary) const
	{
		box query_box(point(boundary.left, boundary.bottom), point(boundary.right, boundary.top));
		return find_nodes_within_boundary(query_box);
	}
}

