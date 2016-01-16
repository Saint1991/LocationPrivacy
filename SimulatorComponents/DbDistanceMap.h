#pragma once
#include "MySQLDb.h"
#include "Rectangle.h"
#include "Node.h"

namespace Map
{

	///<summary>
	/// DBから距離マップを読み出し格納する
	///</summary>
	class DbDistanceMap
	{
	private:
		std::shared_ptr<std::vector<std::unordered_map<Graph::node_id, double>>> distance_map;
		std::unique_ptr<Db::MySQLDb> db;
		std::unique_ptr<std::unordered_map<Graph::node_id, int>> index_map;
		std::string db_name;
		std::string table_name;
	public:
		DbDistanceMap(
			const std::string& setting_file_path,
			const std::string& db_name,
			const std::string& table_name = "distances");
		~DbDistanceMap();
		void load(const Graph::Rectangle<Geography::LatLng>& boundary);

		double get_distance(Graph::node_id from, Graph::node_id to) const;
	};
}


