#pragma once
#include "Types.h"
#include "TableStructure.h"
#include "cppconn\prepared_statement.h"

///<summary>
/// PreparedStatementにバインドできるデータクラスを示すインタフェース
///</summary>
class IBindable
{

typedef Types::BasicType TYPE;

protected:
	template <typename VALUE_TYPE>
	void bind_as(sql::PreparedStatement* statement, const Db::TableStructure& table_info, const std::string& column_name, const VALUE_TYPE& value);

public:
	virtual void bind(sql::PreparedStatement* statement, const Db::TableStructure& table_info) const = 0;
	virtual void bind(sql::PreparedStatement* statement, const std::list<std::string>& columns) const = 0;
};

#include "IBindable.hpp"