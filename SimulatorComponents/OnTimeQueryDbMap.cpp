#include "stdafx.h"
#include "OnTimeQueryDbMap.h"

namespace Map
{

	OnTimeQueryDbMap::OnTimeQueryDbMap(
		std::shared_ptr<Graph::IRoutingModule<BasicMapNode, BasicRoad>> routing_method,
		const std::string& setting_file_path,
		const std::string& db_name,
		const std::string& node_table,
		const std::string& node_connection_table,
		const std::string& poi_table,
		const std::string& poi_connection_table) 
		: BasicDbMap(routing_method, setting_file_path, db_name, node_table, node_connection_table, poi_table, poi_connection_table)
	{
		std::unique_ptr<Db::IDbSettingsLoader> loader = std::make_unique<Db::DbSettingsFileLoader>(setting_file_path);
		db = std::make_unique<Db::MySQLDb>(std::move(loader));
	}


	OnTimeQueryDbMap::~OnTimeQueryDbMap()
	{
	}

	double OnTimeQueryDbMap::shortest_distance(const Graph::MapNodeIndicator& from, const Graph::MapNodeIndicator& to, double distance_threshold) const
	{
		if (from == to) return 0.0;

		double ret_distance = Graph::NO_CONNECTION;
		
		std::stringstream query;
		query << "SELECT distance FROM distances WHERE id1 = ? AND id2 = ?;";
		sql::PreparedStatement* statement = db->prepare(query.str());

		//óºï˚INTERSECTIONÇÃèÍçá
		if (from.type() == Graph::NodeType::INTERSECTION && to.type() == Graph::NodeType::INTERSECTION) {
			Graph::node_id from_id = from.id();
			Graph::node_id to_id = to.id();
			if (from_id > to_id) std::swap(from_id, to_id);
			statement->setInt(1, from_id);
			statement->setInt(2, to_id);
			sql::ResultSet* result = statement->executeQuery();
			result->beforeFirst();
			ret_distance = result->next() ? result->getDouble("distance") : DBL_MAX;
			delete result;
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

			if (between_from.first < between_to.first) {
				statement->setInt(1, between_from.first);
				statement->setInt(2, between_to.first);
			}
			else {
				statement->setInt(2, between_from.first);
				statement->setInt(1, between_to.first);
			}
			sql::ResultSet* result = statement->executeQuery();
			result->beforeFirst();
			double first_first = result->next() ? result->getDouble("distance") : DBL_MAX;
			
			if (between_from.first < between_to.second) {
				statement->setInt(1, between_from.first);
				statement->setInt(2, between_to.second);
			}
			else {
				statement->setInt(2, between_from.first);
				statement->setInt(1, between_to.second);
			}
			result = statement->executeQuery();
			result->beforeFirst();
			double first_second = result->next() ? result->getDouble("distance") : DBL_MAX;
			

			if (between_from.second < between_to.first) {
				statement->setInt(1, between_from.second);
				statement->setInt(2, between_to.first);
			}
			else {
				statement->setInt(2, between_from.second);
				statement->setInt(1, between_to.first);
			}
			result = statement->executeQuery();
			result->beforeFirst();
			double second_first = result->next() ? result->getDouble("distance") : DBL_MAX;
			
			if (between_from.second < between_to.second) {
				statement->setInt(1, between_from.second);
				statement->setInt(2, between_to.second);
			}
			else {
				statement->setInt(2, between_from.second);
				statement->setInt(1, between_to.second);
			}
			result = statement->executeQuery();
			result->beforeFirst();
			double second_second = result->next() ? result->getDouble("distance") : DBL_MAX;

			delete result;

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

			if (from.type() == Graph::NodeType::POI) {
				Graph::node_id from_id = min(between.first, to.id());
				Graph::node_id to_id = max(between.first, to.id());
				statement->setInt(1, from_id);
				statement->setInt(2, to_id);
			}
			else {
				Graph::node_id from_id = min(from.id(), between.first);
				Graph::node_id to_id = max(from.id(), between.first);
				statement->setInt(1, from_id);
				statement->setInt(2, to_id);
			}
			sql::ResultSet* result = statement->executeQuery();
			result->beforeFirst();
			double distance_first = result->next() ? result->getDouble("distance") : DBL_MAX;
			

			if (from.type() == Graph::NodeType::POI) {
				Graph::node_id from_id = min(between.second, to.id());
				Graph::node_id to_id = max(between.second, to.id());
				statement->setInt(1, from_id);
				statement->setInt(2, to_id);
			}
			else {
				Graph::node_id from_id = min(from.id(), between.second);
				Graph::node_id to_id = max(from.id(), between.second);
				statement->setInt(1, from_id);
				statement->setInt(2, to_id);
			}
			result = statement->executeQuery();
			result->beforeFirst();
			double distance_second = from.type() == result->next() ? result->getDouble("distance") : DBL_MAX;
			delete result;

			distance_first += distance_first == Graph::NO_CONNECTION || distance_to_first == Graph::NO_CONNECTION ? 0 : distance_to_first;
			distance_second += distance_second == Graph::NO_CONNECTION || distance_to_second == Graph::NO_CONNECTION ? 0 : distance_to_second;

			ret_distance = min(distance_first, distance_second);
		}


		return ret_distance > distance_threshold ? DBL_MAX : ret_distance;
	}

	double OnTimeQueryDbMap::calc_necessary_time(const Graph::MapNodeIndicator& from, const Graph::MapNodeIndicator& to, const double& avg_speed, double distance_threshold) const
	{
		double distance = shortest_distance(from, to, distance_threshold);
		return distance == DBL_MAX ? DBL_MAX : distance / avg_speed;
	}

	bool OnTimeQueryDbMap::is_reachable(const Graph::MapNodeIndicator& from, const Graph::MapNodeIndicator& to, const double& avg_speed, const double& time_limit) const
	{
		return calc_necessary_time(from, to, avg_speed) <= time_limit;
	}

}

