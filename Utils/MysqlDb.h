#ifdef UTILS_EXPORTS
#define MYSQL_DB_API __declspec(dllexport)
#else
#define MYSQL_DB_API __declspec(dllimport)
#endif
#pragma once
#include "IDbSettingsLoader.h"
#include "DbSettings.h"
#include "cppconn\driver.h"
#include "cppconn\exception.h"
#include "cppconn\statement.h"
#include "cppconn\prepared_statement.h"
#include "cppconn\resultset.h"
#include "Column.h"
#include "BaseQueryGenerator.h"

namespace Db 
{
	class MYSQL_DB_API MySQLDb
	{
	private:
		std::unique_ptr<Db::DbSettings const> settings;
		sql::Connection* connection = nullptr;

		template <typename RESULT_TYPE>
		RESULT_TYPE* execute_if_connection_is_alive(const std::function<RESULT_TYPE(void)>& execute_function);
		
	public:
		
		MySQLDb(std::unique_ptr<Db::IDbSettingsLoader> loader);
		MySQLDb(Db::DbSettings settings);
		~MySQLDb();

		bool use(const std::string& dbname);
		bool execute(const std::string& query);
		sql::ResultSet* row_query(const std::string& query);
		const std::list<std::string> get_databases();
		const std::list<std::string> get_tables();

		///Ç±Ç±ÇÁÇ÷ÇÒÇÕóvçƒåüì¢
		bool create_table(std::unique_ptr<Db::BaseQueryGenerator> generator);
		bool insert(std::unique_ptr<Db::BaseQueryGenerator> generator, std::unordered_map<std::string, std::string> values);
		bool insert(std::unique_ptr<Db::BaseQueryGenerator> generator, std::list<std::unordered_map<std::string, std::string>> value_list);
		bool update(std::unique_ptr<Db::BaseQueryGenerator> generator, std::unordered_map<std::string, std::string> values, std::string where_clause = "");
		sql::ResultSet* select(std::unique_ptr<Db::BaseQueryGenerator> generator, std::string where_clause = "");
	};
}


