#pragma once
#include "Column.h"
#include "BaseQueryGenerator.h"
#include "TableStructure.h"

namespace Db
{

	///<summary>
	/// ��{�I�ȃN�G�����쐬���郆�[�e�B���e�B�N���X
	/// (���₢���킹�̂悤�ȕ��G�ȃN�G���͍쐬�ł��Ȃ�)
	///</summary>
	class QueryGenerator : public Db::BaseQueryGenerator
	{	

	public:
		QueryGenerator(Db::TableStructure table_info);
		~QueryGenerator();
		
		const std::string make_create_table_query() const;
		const std::string make_insert_query() const;
		const std::string make_select_query(const std::string& where_clause = "") const;
		const std::string make_update_query(const std::string&where_clause = "") const;
	};
}


