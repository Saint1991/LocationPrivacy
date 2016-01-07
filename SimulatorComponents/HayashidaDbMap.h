#ifdef SIMULATORCOMPONENTS_EXPORTS
#define HAYASHIDA_DB_MAP_API __declspec(dllexport)
#else
#define HAYASHIDA_DB_MAP_API __declspec(dllimport)
#endif

#pragma once
#include "BasicDbMap.h"
#include "BasicMapNode.h"
#include "BasicPoi.h"
#include "BasicRoad.h"
#include "DbNodeCollectionFactory.h"
#include "DbPoiCollectionFactory.h"
#include "Probability.h"
#include "Rectangle.h"
#include "IRoutingModule.h"
#include "LatLng.h"

namespace Map
{

	class HAYASHIDA_DB_MAP_API HayashidaDbMap : public BasicDbMap
	{
	
	public:
		typedef std::pair<std::vector<Graph::MapNodeIndicator>, double> path_info;
		typedef std::pair<Graph::MapNodeIndicator, std::shared_ptr<Geography::LatLng const>> node_pos_info;

		HayashidaDbMap(std::shared_ptr<Graph::IRoutingModule<BasicMapNode, BasicRoad>> routing_method,
			const std::string& setting_file_path,
			const std::string& db_name,
			const std::string& node_table = "nodes",
			const std::string& node_connection_table = "node_connections",
			const std::string& poi_table = "pois",
			const std::string& poi_connection_table = "poi_connections"
			);
		virtual ~HayashidaDbMap();
		
		std::shared_ptr<BasicPoi const> get_nearest_node_of_now_position(Geography::LatLng now_pos);
		path_info search_random_path(const Graph::MapNodeIndicator& from, const Graph::MapNodeIndicator& to, double distance_threshold);
		path_info get_random_path_info(const Graph::MapNodeIndicator& from, const Graph::MapNodeIndicator& to, double distance_threshold = DBL_MAX) const;
	};

}