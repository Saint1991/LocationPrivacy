#pragma once
#include "Column.h"
#include "BaseQueryGenerator.h"
#include "TableStructure.h"

namespace Db
{

	///<summary>
	/// 基本的なクエリを作成するユーティリティクラス
	/// (副問い合わせのような複雑なクエリは作成できない)
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


