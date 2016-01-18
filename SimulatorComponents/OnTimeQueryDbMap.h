#ifdef SIMULATORCOMPONENTS_EXPORTS
#define ONTIMEQUERYMAP_API __declspec(dllexport)
#else
#define ONTIMEQUERYMAP_API __declspec(dllimport)
#endif
#pragma once
#include "BasicDbMap.h"

namespace Map
{

	///<summary>
	/// DBから最短距離を逐次読み出しするマップ
	///</summary>
	class ONTIMEQUERYMAP_API OnTimeQueryDbMap : public BasicDbMap
	{
	protected:
		std::unique_ptr<Db::MySQLDb> db;

	public:
		OnTimeQueryDbMap(
			std::shared_ptr<Graph::IRoutingModule<BasicMapNode, BasicRoad>> routing_method,
			const std::string& setting_file_path,
			const std::string& db_name,
			const std::string& node_table = "nodes",
			const std::string& node_connection_table = "node_connections",
			const std::string& poi_table = "pois",
			const std::string& poi_connection_table = "poi_connections");
		~OnTimeQueryDbMap();

		double shortest_distance(const Graph::MapNodeIndicator& from, const Graph::MapNodeIndicator& to, double distance_threshold = DBL_MAX) const;
		double calc_necessary_time(const Graph::MapNodeIndicator& from, const Graph::MapNodeIndicator& to, const double& avg_speed, double distance_threshold = DBL_MAX) const;
		bool is_reachable(const Graph::MapNodeIndicator& from, const Graph::MapNodeIndicator& to, const double& avg_speed, const double& time_limit) const;
	};
}


