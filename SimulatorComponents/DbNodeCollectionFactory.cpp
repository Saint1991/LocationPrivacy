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
		db->use(db_name);
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
		query << "SELECT id, latitude, longitude FROM " << node_table << " ORDER BY id ASC;";
		sql::ResultSet* result = db->raw_query(query.str());
		
		result->beforeFirst();
		while (result->next()) {
			Graph::node_id id = result->getInt64("id");
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
		query << "SELECT id1, id2, distance FROM " << connection_table << ";";
		sql::ResultSet* result = db->raw_query(query.str());

		result->beforeFirst();
		while (result->next()) {
			Graph::node_id id1 = result->getInt64("id1");
			Graph::node_id id2 = result->getInt64("id2");
			double distance = result->getDouble("distance");
			connect_each_other(id1, id2, std::make_shared<Graph::BasicPathData>(distance));
		}
		delete result;
	}
	
}

