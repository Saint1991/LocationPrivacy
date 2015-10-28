#include "stdafx.h"
#include "DbBasicUserLoader.h"


namespace User
{


	///<summary>
	/// コンストラクタ
	///</summary>
	DbBasicUserLoader::DbBasicUserLoader(const std::string& setting_file_path, const std::string& db_name, const std::string& user_table_name, const std::string& venue_table_name)
		: db(std::make_unique<Db::MySQLDb>(std::move(std::make_unique<Db::DbSettingsFileLoader>(setting_file_path)))),
		db_name(db_name), user_table_name(user_table_name), venue_table_name(venue_table_name)
	{
	}


	///<summary>
	/// デストラクタ
	///</summary>
	DbBasicUserLoader::~DbBasicUserLoader()
	{
	}


	///<summary>
	/// データベースからBasicUserを作成する
	/// 読出しに失敗した場合はnullptrが返る
	///</summary>
	std::shared_ptr<BasicUser<>> DbBasicUserLoader::load_user(unsigned int user_id)
	{
		if (!db->use(db_name)) return nullptr;
		std::stringstream query;
		query << "SELECT venue_id, timestamp category_id FROM " << user_table_name << " INNER JOIN " << venue_table_name << " ON " << user_table_name << ".venue_id = " << venue_table_name << ".id WHERE user_id = " << user_id << "ORDER BY timestamp ASC;";
		sql::ResultSet* result = db->raw_query(query.str());

		std::unique_ptr<std::vector<std::string>> timestamps = std::make_unique<std::vector<std::string>>();
		result->beforeFirst();
		while (result->next()) {
			int venue_id = result->getInt("venue_id");
			std::string timestamp = result->getString("timestamp");
			std::string category_id = result->getString("category_id");
			timestamps->push_back(timestamp);
		}
		//std::shared_ptr<BasicUser<>> user = std::make_shared<BasicUser<>>( , , 0);
		delete result;
		//return user;
		return nullptr;
	}
}

