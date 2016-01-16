#include "stdafx.h"
#include "DbDistanceMap.h"
#include "DbSettingsFileLoader.h"

namespace Map
{

	///<summary>
	/// コンストラクタ
	///</summary>
	DbDistanceMap::DbDistanceMap(
		const std::string& setting_file_path,
		const std::string& db_name,
		const std::string& table_name)
		: db_name(db_name), table_name(table_name), index_map(std::make_unique<std::unordered_map<Graph::node_id, int>>()), distance_map(std::make_shared<std::vector<std::unordered_map<Graph::node_id, double>>>())
	{
		std::unique_ptr<Db::IDbSettingsLoader> loader = std::make_unique<Db::DbSettingsFileLoader>(setting_file_path);
		db = std::make_unique<Db::MySQLDb>(std::move(loader));
	}


	///<summary>
	/// デストラクタ
	///</summary>
	DbDistanceMap::~DbDistanceMap()
	{
	}

}
