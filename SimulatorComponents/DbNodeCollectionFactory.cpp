#include "stdafx.h"
#include "DbNodeCollectionFactory.h"

namespace Map 
{
	
	///<summary>
	/// コンストラクタ
	///</summary>
	DbNodeCollectionFactory::DbNodeCollectionFactory(const std::string& setting_file_path,	const std::string& db_name, const std::string& node_table_name, const std::string& connection_table_name)
		: db_name(db_name), node_table(node_table_name), connection_table(connection_table_name)
	{
		std::unique_ptr<Db::IDbSettingsLoader> loader = std::make_unique<Db::DbSettingsFileLoader>(setting_file_path);
		db = std::make_unique<Db::MySQLDb>(std::move(loader));
	}

	///<summary>
	/// デストラクタ
	///</summary>
	DbNodeCollectionFactory::~DbNodeCollectionFactory()
	{
	}


	///<summary>
	/// DBからノードを読み出す
	///</summary>
	void DbNodeCollectionFactory::create_nodes()
	{
		if (!db->use(db_name)) return;
		std::stringstream query;
		query << "SELECT node_id, latitude, longitude FROM " << node_table << " ORDER BY node_id ASC;";
		sql::ResultSet* result = db->raw_query(query.str());
		
		result->beforeFirst();
		while (result->next()) {
			Graph::node_id id = result->getInt64("node_id");
			double latitude = result->getDouble("latitude");
			double longitude = result->getDouble("longitude");
			add_node(std::make_shared<BasicMapNode>(id, Geography::LatLng(latitude, longitude)));
		}
		delete result;
	}


	///<summary>
	/// DBから接続関係を読み出す
	///</summary>
	void DbNodeCollectionFactory::set_connectivities()
	{
		if (!db->use(db_name)) return;
		std::stringstream query;
		query << "SELECT node_id1, node_id2, distance FROM " << connection_table << ";";
		sql::ResultSet* result = db->raw_query(query.str());

		result->beforeFirst();
		while (result->next()) {
			Graph::node_id id1 = result->getInt64("node_id1");
			Graph::node_id id2 = result->getInt64("node_id2");
			double distance = result->getDouble("distance");
			connect_each_other(id1, id2, std::make_shared<Graph::BasicPathData>(distance));
		}
		delete result;
	}
	
}

