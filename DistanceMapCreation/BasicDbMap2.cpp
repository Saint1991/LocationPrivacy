#include "stdafx.h"
#include "BasicDbMap2.h"

namespace Map
{
	BasicDbMap2::BasicDbMap2(
		std::shared_ptr<Graph::IRoutingModule<BasicMapNode, BasicRoad>> routing_method,
		const std::string& setting_file_path,
		const std::string& db_name,
		const std::string& node_table,
		const std::string& node_connection_table,
		const std::string& poi_table,
		const std::string& poi_connection_table) : BasicDbMap(routing_method, setting_file_path, db_name, node_table, node_connection_table, poi_table, poi_connection_table)
	{

	}

	BasicDbMap2::~BasicDbMap2() 
	{
	
	}

	std::shared_ptr<const Collection::IdentifiableCollection<Graph::node_id, Map::BasicMapNode>> BasicDbMap2::get_node_collection()
	{
		return node_collection;
	}
}