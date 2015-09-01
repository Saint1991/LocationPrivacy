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
	/// �e�[�u���̍\����\���\����
	///</summary>
	struct TableStructure : public Serializable
	{
		std::string table_name = "";
		std::unique_ptr<std::list<Db::Column>> columns = nullptr;
		std::unique_ptr<std::list<std::string>> primary_keys = nullptr;
	
		TABLE_STRUCTURE_API TableStructure(const std::string& table_name, std::list<Db::Column> columns);
		TABLE_STRUCTURE_API TableStructure(const Db::TableStructure& structure);
		TABLE_STRUCTURE_API virtual ~TableStructure();

		TABLE_STRUCTURE_API bool is_column_exists(const std::string& column_name) const;
		TABLE_STRUCTURE_API unsigned int get_column_index(const std::string& column_name) const;
		TABLE_STRUCTURE_API const std::list<std::string> get_column_name_list() const;
		TABLE_STRUCTURE_API const std::string to_string() const;
		TABLE_STRUCTURE_API bool add_primary_key(std::string primary_key);
	};
}


