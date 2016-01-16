#pragma once
#include "BasicDbMap.h"

namespace Map
{

	///<summary>
	/// ê∂ÇÃnode_collectionÇÇ∆ÇÍÇÈÉ}ÉbÉv
	///</summary>
	class BasicDbMap2 : public BasicDbMap
	{
	public:
		BasicDbMap2(
			std::shared_ptr<Graph::IRoutingModule<BasicMapNode, BasicRoad>> routing_method,
			const std::string& setting_file_path,
			const std::string& db_name,
			const std::string& node_table = "nodes",
			const std::string& node_connection_table = "node_connections",
			const std::string& poi_table = "pois",
			const std::string& poi_connection_table = "poi_connections");
		~BasicDbMap2();
		std::shared_ptr<const Collection::IdentifiableCollection<Graph::node_id, BasicMapNode>> get_node_collection();
	};
}


