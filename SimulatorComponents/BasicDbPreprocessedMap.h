#ifdef SIMULATORCOMPONENTS_EXPORTS
#define BASICDBPREPROCESSEDMAP_API __declspec(dllexport)
#else
#define BASICDBPREPROCESSEDMAP_API __declspec(dllimport)
#endif
#pragma once
#include "DbDistanceMap.h"
#include "BasicDbMap.h"

namespace Map
{
	class BASICDBPREPROCESSEDMAP_API BasicDbPreprocessedMap : public BasicDbMap
	{
	protected:
		std::unique_ptr<DbDistanceMap> distance_map;
		void build_map(const Graph::Rectangle<Geography::LatLng>& boundary);

	public:
		BasicDbPreprocessedMap(
			std::shared_ptr<Graph::IRoutingModule<BasicMapNode, BasicRoad>> routing_method,
			const std::string& setting_file_path,
			const std::string& db_name,
			const std::string& node_table = "nodes",
			const std::string& node_connection_table = "node_connections",
			const std::string& poi_table = "pois",
			const std::string& poi_connection_table = "poi_connections");
		~BasicDbPreprocessedMap();
		
		double shortest_distance(const Graph::MapNodeIndicator& from, const Graph::MapNodeIndicator& to) const;
		double calc_necessary_time(const Graph::MapNodeIndicator& from, const Graph::MapNodeIndicator& to, const double& avg_speed) const;
		bool is_reachable(const Graph::MapNodeIndicator& from, const Graph::MapNodeIndicator& to, const double& avg_speed, const double& time_limit) const;


	};
}


