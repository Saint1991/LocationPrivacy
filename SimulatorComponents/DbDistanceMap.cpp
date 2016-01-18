#include "stdafx.h"
#include "DbDistanceMap.h"
#include "DbSettingsFileLoader.h"

namespace Map
{

	///<summary>
	/// コンストラクタ
	///</summary>
	DbDistanceMap::DbDistanceMap(
		const std::string& setting_file_path,
		const std::string& db_name,
		const std::string& table_name)
		: db_name(db_name), table_name(table_name), index_map(std::make_unique<std::unordered_map<Graph::node_id, int>>()), distance_map(std::make_shared<std::vector<std::unordered_map<Graph::node_id, double>>>())
	{
		std::unique_ptr<Db::IDbSettingsLoader> loader = std::make_unique<Db::DbSettingsFileLoader>(setting_file_path);
		db = std::make_unique<Db::MySQLDb>(std::move(loader));
	}


	///<summary>
	/// デストラクタ
	///</summary>
	DbDistanceMap::~DbDistanceMap()
	{
	}

	void DbDistanceMap::load(const Graph::Rectangle<Geography::LatLng>& boundary)
	{
		db->use(db_name);
		std::stringstream query;
		//query << "SELECT id1, id2, distance FROM " << table_name << " WHERE id1 IN (SELECT DISTINCT(node_id) FROM nodes WHERE longitude BETWEEN " << boundary.left << " AND " << boundary.right << " AND latitude BETWEEN " << boundary.bottom << " AND " << boundary.top << ") ORDER BY id1 ASC;";
		query << "SELECT * FROM " << table_name << ";";
		std::cout << query.str() << std::endl;

		Time::Timer timer;
		timer.start();
		sql::ResultSet* result = db->raw_query(query.str());
		timer.end();

		std::cout << timer.duration() << "sec" << std::endl;

		std::unordered_map<Graph::node_id, double> distances ;
		result->beforeFirst();
		result->next();
		int current_id = result->getInt("id1");
		index_map->insert(std::make_pair(current_id, 0));

		do {
			int id1 = result->getInt("id1");
			if (id1 != current_id) {
				distance_map->push_back(distances);
				distances = std::unordered_map<Graph::node_id, double>();
				int current_index = distance_map->size();
				index_map->insert(std::make_pair(id1, current_index));
				current_id = id1;
			}
			int id2 = result->getInt("id2");
			double distance = result->getDouble("distance");
			distances.insert(std::make_pair(id2, distance));
		} while (result->next());
		distance_map->push_back(distances);
		delete result;
	}


	///<summary>
	/// 格納されている距離を読み出す
	/// 格納されていない読み出しの場合はDBL_MAXを返す
	///</summary>
	double DbDistanceMap::get_distance(Graph::node_id from, Graph::node_id to) const
	{
		if (to < from) std::swap(from, to);
		std::unordered_map<Graph::node_id, int>::const_iterator iter = index_map->find(from);
		if (iter == index_map->end()) return DBL_MAX;
		int index = iter->second;
		std::unordered_map<Graph::node_id, double>::const_iterator ret = distance_map->at(index).find(to);
		if (ret == distance_map->at(index).end()) return DBL_MAX;
		return ret->second;
	}
}
