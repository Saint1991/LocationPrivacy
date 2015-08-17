#pragma once
#include "Column.h"

namespace Db 
{

	///<summary>
	/// テーブルの構造を表す構造体
	///</summary>
	struct TableStructure
	{
		
		std::string table_name;
		std::unique_ptr<std::list<Db::Column>> columns;
		std::unique_ptr<std::list<std::string>> primary_keys = nullptr;
	
		TableStructure(std::string table_name, std::list<Db::Column> columns);
		TableStructure(const Db::TableStructure& structure);
		virtual ~TableStructure();

		bool is_column_exists(const std::string& column_name) const;
		const unsigned int get_column_index(const std::string& column_name) const;
		const std::list<std::string> get_column_name_list() const;
		bool add_primary_key(std::string primary_key);
	};
}


