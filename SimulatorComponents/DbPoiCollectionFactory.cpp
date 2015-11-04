#include "stdafx.h"
#include "DbPoiCollectionFactory.h"

namespace Map
{

	///<summary>
	/// コンストラクタ
	///</summary>
	DbPoiCollectionFactory::DbPoiCollectionFactory(const std::string& setting_file_path, const std::string& db_name, 	const std::string& poi_table_name, const std::string& connection_table_name)
		: db_name(db_name), poi_table(poi_table_name), connection_table(connection_table_name)
	{
		std::unique_ptr<Db::IDbSettingsLoader> loader = std::make_unique<Db::DbSettingsFileLoader>(setting_file_path);
		db = std::make_unique<Db::MySQLDb>(std::move(loader));
	}


	///<summary>
	/// デストラクタ
	///</summary>
	DbPoiCollectionFactory::~DbPoiCollectionFactory()
	{
	}


	///<summary>
	/// POIの読み込み，作成
	///</summary>
	void DbPoiCollectionFactory::create_nodes(const Graph::Rectangle<Geography::LatLng>& boundary)
	{
		if (!db->use(db_name)) return;
		std::stringstream query;
		std::stringstream sub;
		std::stringstream in;
		in << "(SELECT DISTINCT(node_id) FROM nodes WHERE latitude BETWEEN " << boundary.bottom << " AND " << boundary.top << " AND longitude BETWEEN " << boundary.left << " AND " << boundary.right << ")";
		sub << "(SELECT DISTINCT(id) FROM " << connection_table << " WHERE to1 IN " << in.str() << " AND to2 IN " << in.str() << ")";
		query << "SELECT id, latitude, longitude, category_id, category_name, venue_name FROM " << poi_table << " WHERE id IN " << sub.str() << " ORDER BY id ASC;";
		sql::ResultSet* result = db->raw_query(query.str());

		result->beforeFirst();
		while (result->next()) {
			Graph::node_id id = result->getInt64("id");
			double latitude = result->getDouble("latitude");
			double longitude = result->getDouble("longitude");
			std::string category_id = result->getString("category_id");
			std::string category_name = result->getString("category_name");
			std::string venue_name = result->getString("venue_name");
			add_node(std::make_shared<BasicPoi>(id, Geography::LatLng(latitude, longitude), venue_name, category_id, category_name));
		}
		delete result;
	}

	///<summary>
	/// POIの接続関係の作成
	///</summary>
	void DbPoiCollectionFactory::set_connectivities(const Graph::Rectangle<Geography::LatLng>& boundary)
	{
		if (!db->use(db_name)) return;
		std::stringstream query;
		std::stringstream in;
		in << "(SELECT DISTINCT(node_id) FROM nodes WHERE latitude BETWEEN " << boundary.bottom << " AND " << boundary.top << " AND longitude BETWEEN " << boundary.left << " AND " << boundary.right << ")";
		query << "SELECT id, to1, to2, distance1, distance2 FROM " << connection_table << " WHERE to1 IN " << in.str() << " AND to2 IN " << in.str() << ";";
		sql::ResultSet* result = db->raw_query(query.str());

		result->beforeFirst();
		while (result->next()) {
			Graph::node_id id = result->getInt64("id");
			Graph::node_id to1 = result->getInt64("to1");
			Graph::node_id to2 = result->getInt64("to2");
			double distance1 = result->getDouble("distance1");
			double distance2 = result->getDouble("distance2");
			std::shared_ptr<BasicPoi> poi = node_collection->get_by_id(id);
			poi->set_relation(to1, distance1, to2, distance2);
		}

		delete result;
	}
}

