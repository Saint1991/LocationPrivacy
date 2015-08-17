#pragma once
#include "Serializable.h"
#include "TableStructure.h"

namespace Db
{
	class QueryGenerateUtility
	{
	private:
		QueryGenerateUtility();
		~QueryGenerateUtility();
	
	public:
		static std::string make_round_bracket_clause(const std::list<std::string>& elements);
		static std::string make_round_bracket_clause(const std::list<std::shared_ptr<Serializable>>& elements);
		static const std::string make_create_table_query(const Db::TableStructure& table_info);
		static const std::string make_insert_query(const Db::TableStructure& insert_columns);
		static const std::string make_insert_query(const std::string& table_name, const std::list<std::string>& columns);
		static const std::string make_select_query(const std::string& table_name, const std::list<std::string>& columns, const std::string& where_clause = "");
		static const std::string make_select_query(const Db::TableStructure& select_columns, const std::string& where_clause = "");
		static const std::string make_update_query(const std::string& table_name, const std::list<std::string>& columns, const std::string& where_clause);
		static const std::string make_update_query(const Db::TableStructure& update_columns, const std::string& where_clause = "");
	};
}


