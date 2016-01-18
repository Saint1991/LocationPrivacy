#include "stdafx.h"
#include "BasicDbPreprocessedMap.h"

namespace Map
{
	BasicDbPreprocessedMap::BasicDbPreprocessedMap(
		std::shared_ptr<Graph::IRoutingModule<BasicMapNode, BasicRoad>> routing_method,
		const std::string& setting_file_path,
		const std::string& db_name,
		const std::string& node_table,
		const std::string& node_connection_table,
		const std::string& poi_table,
		const std::string& poi_connection_table) 
		: BasicDbMap(routing_method, setting_file_path, db_name, node_table, node_connection_table, poi_table, poi_connection_table),
		distance_map(std::make_unique<DbDistanceMap>(setting_file_path, db_name))
	{
	}

	BasicDbPreprocessedMap::~BasicDbPreprocessedMap()
	{
	}

	void BasicDbPreprocessedMap::build_map(const Graph::Rectangle<Geography::LatLng>& boundary)
	{
		BasicDbMap::build_map(boundary);
		std::cout << "Loading distance map" << std::endl;
		distance_map->load(boundary);
		std::cout << "Complete!" << std::endl;
	}

	double BasicDbPreprocessedMap::shortest_distance(const Graph::MapNodeIndicator& from, const Graph::MapNodeIndicator& to, double distance_threshold) const 
	{
		if (from == to) return 0.0;

		double ret_distance = Graph::NO_CONNECTION;

		//óºï˚INTERSECTIONÇÃèÍçá
		if (from.type() == Graph::NodeType::INTERSECTION && to.type() == Graph::NodeType::INTERSECTION) {
			ret_distance = distance_map->get_distance(from.id(), to.id());
		}

		//óºï˚POIÇÃèÍçá
		else if (from.type() == Graph::NodeType::POI && to.type() == Graph::NodeType::POI) {

			std::shared_ptr<BasicPoi const> poi_from = poi_collection->read_by_id(from.id());
			std::shared_ptr<BasicPoi const> poi_to = poi_collection->read_by_id(to.id());
			std::pair<Graph::node_id, Graph::node_id> between_from = poi_from->between();
			std::pair<Graph::node_id, Graph::node_id> between_to = poi_to->between();
			double distance_poi_from_to_first = poi_from->distance_to(between_from.first);
			double distance_poi_from_to_second = poi_from->distance_to(between_from.second);
			double distance_poi_to_to_first = poi_to->distance_to(between_to.first);
			double distance_poi_to_to_second = poi_to->distance_to(between_to.second);

			double first_first = distance_map->get_distance(between_from.first, between_to.first);
			double first_second = distance_map->get_distance(between_from.first, between_to.second);
			double second_first = distance_map->get_distance(between_from.second, between_to.first);
			double second_second = distance_map->get_distance(between_from.second, between_to.second);

			first_first += first_first == DBL_MAX || distance_poi_from_to_first == DBL_MAX || distance_poi_to_to_first == DBL_MAX ? 0 : distance_poi_from_to_first + distance_poi_to_to_first;
			first_second += first_second == DBL_MAX || distance_poi_from_to_first == DBL_MAX || distance_poi_to_to_second == DBL_MAX ? 0 : distance_poi_from_to_first + distance_poi_to_to_second;
			second_first += second_first == DBL_MAX || distance_poi_from_to_second == DBL_MAX || distance_poi_to_to_first == DBL_MAX ? 0 : distance_poi_from_to_second + distance_poi_to_to_first;
			second_second += second_second == DBL_MAX || distance_poi_from_to_second == DBL_MAX || distance_poi_to_to_second == DBL_MAX ? 0 : distance_poi_from_to_second + distance_poi_to_to_second;

			std::vector<double> distance_list({
				first_first,
				first_second,
				second_first,
				second_second
			});

			ret_distance = *std::min_element(distance_list.begin(), distance_list.end());
		}
		
		//àÍï˚ÇÃÇ›POIÇÃèÍçá
		else {

			Graph::node_id poi_id = from.type() == Graph::NodeType::POI ? from.id() : to.id();

			std::shared_ptr<BasicPoi const> poi = poi_collection->read_by_id(poi_id);
			std::pair<Graph::node_id, Graph::node_id> between = poi->between();
			double distance_to_first = poi->distance_to(between.first);
			double distance_to_second = poi->distance_to(between.second);

			double distance_first = from.type() == Graph::NodeType::POI ? distance_map->get_distance(between.first, to.id()) : distance_map->get_distance(from.id(), between.first);
			double distance_second = from.type() == Graph::NodeType::POI ? distance_map->get_distance(between.second, to.id()) : distance_map->get_distance(from.id(), between.second);

			distance_first += distance_first == Graph::NO_CONNECTION || distance_to_first == Graph::NO_CONNECTION ? 0 : distance_to_first;
			distance_second += distance_second == Graph::NO_CONNECTION || distance_to_second == Graph::NO_CONNECTION ? 0 : distance_to_second;

			ret_distance = min(distance_first, distance_second);
		}


		return ret_distance > distance_threshold ? DBL_MAX : ret_distance;
	}

	double BasicDbPreprocessedMap::calc_necessary_time(const Graph::MapNodeIndicator& from, const Graph::MapNodeIndicator& to, const double& avg_speed, double distance_threshold) const
	{
		double distance = shortest_distance(from, to, distance_threshold);
		return distance == DBL_MAX ? DBL_MAX : distance / avg_speed;
	}

	bool BasicDbPreprocessedMap::is_reachable(const Graph::MapNodeIndicator& from, const Graph::MapNodeIndicator& to, const double& avg_speed, const double& timelimit) const
	{
		return calc_necessary_time(from, to, avg_speed) <= timelimit;
	}

}

