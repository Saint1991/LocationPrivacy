#ifdef UTILS_EXPORTS
#define TABLE_STRUCTURE_API __declspec(dllexport)
#else
#define TABLE_STRUCTURE_API __declspec(dllimport)
#endif

#pragma once
#include "Column.h"
#include "Serializable.h"

namespace Db 
{

	///<summary>
	/// テーブルの構造を表す構造体
	///</summary>
	struct TableStructure : public Serializable
	{
	protected:
		std::shared_ptr<std::list<Db::Column>> columns = nullptr;
		std::shared_ptr<std::list<std::string>> primary_keys = nullptr;
	
	public:
		std::string table_name = "";	
		TABLE_STRUCTURE_API TableStructure(const std::string& table_name, std::list<Db::Column> columns);
		TABLE_STRUCTURE_API TableStructure(const Db::TableStructure& structure);
		TABLE_STRUCTURE_API virtual ~TableStructure();

		TABLE_STRUCTURE_API bool is_column_exists(const std::string& column_name) const;
		TABLE_STRUCTURE_API unsigned int get_column_index(const std::string& column_name) const;
		TABLE_STRUCTURE_API const std::shared_ptr<const std::list<Column>> get_column_list() const;
		TABLE_STRUCTURE_API const std::shared_ptr<const std::list<std::string>> get_primary_keys() const;
		TABLE_STRUCTURE_API const std::vector<std::string> get_column_names() const;
		TABLE_STRUCTURE_API const std::string to_string() const;
		TABLE_STRUCTURE_API bool add_column(const Column& column);
		TABLE_STRUCTURE_API bool add_primary_key(std::string primary_key);
	};
}


