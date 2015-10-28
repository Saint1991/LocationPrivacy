#ifdef SIMULATORCOMPONENTS_EXPORTS
#define DB_BASICUSER_LOADER_API __declspec(dllexport)
#else
#define DB_BASICUSER_LOADER_API __declspec(dllimport)
#endif

#pragma once
#include "BasicUser.h"
#include "MySQLDb.h"
#include "DbSettingsFileLoader.h"
namespace User 
{

	///<summary>
	/// データベースからユーザデータを読み出してBasicUserを作成するクラス
	///</summary>
	class DB_BASICUSER_LOADER_API DbBasicUserLoader
	{
	protected:
		std::unique_ptr<Db::MySQLDb> db;
		std::string db_name;
		std::string user_table_name;
		std::string venue_table_name;
	public:
		DbBasicUserLoader(const std::string& setting_file_path, const std::string& db_name = "map_tokyo", const std::string& user_table_name = "checkins", const std::string& venue_table_name = "pois");
		~DbBasicUserLoader();
		std::shared_ptr<BasicUser<>> load_user(unsigned int user_id);
	};
}


