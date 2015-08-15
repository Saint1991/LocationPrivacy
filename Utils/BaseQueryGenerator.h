#pragma once
#include "Serializable.h"
#include "TableStructure.h"

namespace Db
{
	class BaseQueryGenerator 
	{
	protected:
		std::unique_ptr<Db::TableStructure> table_info;
		static std::string make_round_bracket_clause(const std::list<std::string>& elements);
		static std::string make_round_bracket_clause(const std::list<std::shared_ptr<Serializable>>& elements);
	
	public:	
		BaseQueryGenerator(Db::TableStructure table_info);
		virtual ~BaseQueryGenerator();
		virtual const std::string make_create_table_query() const = 0;
		virtual const std::string make_insert_query() const = 0;
		virtual const std::string make_select_query(const std::string& where_clause = "") const = 0;
		virtual const std::string make_update_query(const std::string& where_clause = "") const = 0;
	};
}