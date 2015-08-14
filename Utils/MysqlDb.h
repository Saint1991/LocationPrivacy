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

namespace Db 
{
	class MYSQL_DB_API MysqlDb
	{
	private:
		std::unique_ptr<Db::DbSettings const> settings;
		sql::Connection* connection = nullptr;

		template <typename RESULT_TYPE>
		RESULT_TYPE* execute_if_connection_is_alive(const std::function<RESULT_TYPE(void)>& execute_function);
		
	public:
		
		MysqlDb(std::unique_ptr<Db::IDbSettingsLoader> loader);
		MysqlDb(Db::DbSettings settings);
		~MysqlDb();

		bool use(const char* dbname);
		bool execute(const char* query);
		sql::ResultSet* row_query(const char* query);
		const std::list<std::string> get_databases();
		const std::list<std::string> get_tables();
	};
}


