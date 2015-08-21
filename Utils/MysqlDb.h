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
#include "TableStructure.h"
#include "QueryGenerateUtility.h"
#include "Bindable.h"

namespace Db 
{
	class MYSQL_DB_API MySQLDb
	{
	private:		
		std::unique_ptr<Db::DbSettings const> settings;
		sql::Connection* connection = nullptr;

		template <typename RESULT_TYPE>
		RESULT_TYPE* execute_if_connection_is_alive(const std::function<RESULT_TYPE*(void)>& execute_function);
		
	public:
		
		MySQLDb(std::unique_ptr<Db::IDbSettingsLoader> loader);
		MySQLDb(Db::DbSettings settings);
		~MySQLDb();

		bool use(const std::string& dbname);
		bool execute(const std::string& query);
		sql::ResultSet* raw_query(const std::string& query);
		const std::list<std::string> get_databases();
		const std::list<std::string> get_tables();

		///Ç±Ç±ÇÁÇ÷ÇÒÇÕóvçƒåüì¢
		bool create_table(const Db::TableStructure& table_info);
		bool insert(const std::string& table_name, const std::list<std::string>& columns, std::shared_ptr<Bindable const> data);
		bool insert(const Db::TableStructure& insert_columns, std::shared_ptr<Bindable const> data);
		bool insert(const std::string& table_name, const std::list<std::string>& columns, const std::list<std::shared_ptr<Bindable const>>& data_list);
		bool insert(const Db::TableStructure& insert_columns, const std::list<std::shared_ptr<Bindable const>>& data_list);
		int update(const std::string& table_name, const std::list<std::string>& columns, std::shared_ptr<Bindable const> data, const std::string& where_clause);
		int update(const Db::TableStructure& update_columns, std::shared_ptr<Bindable const> data, std::string where_clause);
		sql::ResultSet* select(const std::string& table_name, const std::list<std::string>& columns, const std::string& where_clause = "");
		sql::ResultSet* select(const Db::TableStructure& select_columns, const std::string where_clause = "");
	};
}


