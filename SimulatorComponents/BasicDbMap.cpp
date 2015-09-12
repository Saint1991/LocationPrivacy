#include "stdafx.h"
#include "BasicDbMap.h"

namespace Map
{

	///<summary>
	/// コンストラクタ
	/// RoutingはWarshallFloydを用いる
	///</summary>
	BasicDbMap::BasicDbMap (
		const std::string& setting_file_path,
		const std::string& db_name,
		const std::string& node_table,
		const std::string& node_connection_table,
		const std::string& poi_table,
		const std::string& poi_connection_table)
		: Graph::Map<BasicMapNode, BasicPoi, BasicRoad>(), 
		setting_file_path(setting_file_path),
		db_name(db_name),
		node_table(node_table),
		node_connection_table(node_connection_table),
		poi_table(poi_table),
		poi_connection_table(poi_connection_table)
	{
	}


	///<summary>
	/// デストラクタ
	///</summary>
	BasicDbMap::~BasicDbMap()
	{
	}


	///<summary>
	/// マップを構成する (構成内容の実装はそれぞれDbNodeCollectionFactory, DbPoiCollectionFactoryの中)
	///</summary>
	void BasicDbMap::build_map()
	{
		std::unique_ptr<DbNodeCollectionFactory> node_factory = std::make_unique<DbNodeCollectionFactory>(setting_file_path, db_name, node_table, node_connection_table);
		std::unique_ptr<DbPoiCollectionFactory> poi_factory = std::make_unique<DbPoiCollectionFactory>(setting_file_path, db_name, poi_table, poi_connection_table);

		node_collection = node_factory->create_static_node_collection();
		poi_collection = poi_factory->create_static_node_collection();
	}
}




