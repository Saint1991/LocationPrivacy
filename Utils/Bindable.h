#pragma once
#include "TableStructure.h"
#include "cppconn\prepared_statement.h"

///<summary>
/// PreparedStatementにバインドできるデータクラスを示すインタフェース
///</summary>
class Bindable
{

private:
	enum AvailableValueType { BOOL, STRING, INT, INT64, UINT, UINT64, DOUBLE, LDOUBLE, SQL_NULL, INVALID };

protected:
	template <typename VALUE_TYPE>
	void bind_as(sql::PreparedStatement* statement, const Db::TableStructure& table_info, const std::string& column_name, const VALUE_TYPE& value);

public:
	virtual void bind(sql::PreparedStatement* statement, const Db::TableStructure& table_info) const = 0;
	virtual void bind(sql::PreparedStatement* statement, const std::list<std::string>& columns) const = 0;
};

#include "Bindable.hpp"