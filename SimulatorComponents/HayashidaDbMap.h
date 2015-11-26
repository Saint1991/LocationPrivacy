#ifdef SIMULATORCOMPONENTS_EXPORTS
#define HAYASHIDA_DB_MAP_API __declspec(dllexport)
#else
#define HAYASHIDA_DB_MAP_API __declspec(dllimport)
#endif

#pragma once
#include "Map.h"
#include "BasicMapNode.h"
#include "BasicPoi.h"
#include "BasicRoad.h"
#include "DbNodeCollectionFactory.h"
#include "DbPoiCollectionFactory.h"
#include "Probability.h"
#include "Rectangle.h"
#include "IRoutingModule.h"

namespace Map
{

	class HAYASHIDA_DB_MAP_API HayashidaDbMap : public Graph::Map<BasicMapNode, BasicPoi, BasicRoad>
	{
	protected:
		std::string setting_file_path;
		std::string db_name;
		std::string node_table;
		std::string node_connection_table;
		std::string poi_table;
		std::string poi_connection_table;

		void build_map(const Graph::Rectangle<Geography::LatLng>& boundary);

	public:
		HayashidaDbMap(std::shared_ptr<Graph::IRoutingModule<BasicMapNode, BasicRoad>> routing_method,
			const std::string& setting_file_path,
			const std::string& db_name,
			const std::string& node_table = "nodes",
			const std::string& node_connection_table = "node_connections",
			const std::string& poi_table = "pois",
			const std::string& poi_connection_table = "poi_connections"
			);
		virtual ~HayashidaDbMap();



	};

}