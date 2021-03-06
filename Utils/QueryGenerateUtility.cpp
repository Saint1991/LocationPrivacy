#include "stdafx.h"
#include "QueryGenerateUtility.h"


Db::QueryGenerateUtility::QueryGenerateUtility()
{
}


Db::QueryGenerateUtility::~QueryGenerateUtility()
{
}


///<summary>
/// ()で囲まれた部分を作成するユーティリティ
/// elementsが空の場合は空文字列を返す
///</summary>
std::string Db::QueryGenerateUtility::make_round_bracket_clause(const std::vector<std::string>& elements)
{
	if (elements.size() == 0) return "";
	std::stringstream stream;
	stream << "(";
	for (std::vector<std::string>::const_iterator iter = elements.begin(); iter != elements.end(); iter++) {
		stream << *iter << ", ";
	}
	std::string ret = stream.str();
	ret.replace(ret.end() - 2, ret.end(), ")");
	return ret;
}


///<summary>
/// ()で囲まれた部分を作成するユーティリティ
/// elementsが空の場合は空文字列を返す
///</summary>
std::string Db::QueryGenerateUtility::make_round_bracket_clause(const std::vector<std::shared_ptr<Serializable>>& elements)
{
	if (elements.size() == 0) return "";
	std::stringstream stream;
	stream << "(";
	for (std::vector<std::shared_ptr<Serializable>>::const_iterator iter = elements.begin(); iter != elements.end(); iter++) {
		stream << (*iter)->to_string() << ", ";
	}
	std::string ret = stream.str();
	ret.replace(ret.end() - 2, ret.end(), ")");
	return ret;
}



///<summary>
/// CREATE TABLEクエリを作成
/// カラムデータがセットされていない場合は空文字列を返す
///</summary>
const std::string Db::QueryGenerateUtility::make_create_table_query(const Db::TableStructure& table_info) 
{
	return table_info.to_string();
}

///<summary>
/// INSERTクエリをプレースホルダ付きで作成する
/// カラム名は最初に渡したリストの順になる
///</summary>
const std::string Db::QueryGenerateUtility::make_insert_query(const std::string& table_name, const std::vector<std::string>& columns)
{
	if (columns.size() == 0) return "";

	std::string column_clause = make_round_bracket_clause(columns);
	std::string value_clause = make_round_bracket_clause(std::vector<std::string>(columns.size(), "?"));

	std::stringstream query;
	query << "INSERT INTO " << table_name << " " << column_clause << " VALUES " << value_clause;

	return query.str();
}

///<summary>
/// INSERTクエリをプレースホルダ付きで作成する
/// カラム名は最初に渡したリストの順になる
///</summary>
const std::string Db::QueryGenerateUtility::make_insert_query(const Db::TableStructure& insert_columns)
{
	if (insert_columns.get_column_list() == nullptr) return "";
	return make_insert_query(insert_columns.table_name, insert_columns.get_column_names());
}


///<summary>
/// 基本的なSELECTクエリを作成する
///</summary>
const std::string Db::QueryGenerateUtility::make_select_query(const std::string& table_name, const std::vector<std::string>& columns, const std::string& where_clause)
{
	if (columns.size() == 0) return "";

	std::string column_clause = make_round_bracket_clause(columns);
	column_clause = column_clause.substr(1, column_clause.length() - 2);
	std::stringstream query;

	query << "SELECT " << column_clause << " FROM " << table_name;
	if (where_clause.length() != 0) query << " WHERE " + where_clause;
	return query.str();
}

///<summary>
/// 基本的なSELECTクエリを作成する
///</summary>
const std::string Db::QueryGenerateUtility::make_select_query(const Db::TableStructure& select_columns, const std::string& where_clause) 
{
	if (select_columns.get_column_list() == nullptr) return "";
	return make_select_query(select_columns.table_name, select_columns.get_column_names(), where_clause);
}



///<summary>
/// プレースホルダ付きでUpdateクエリを作成する
///</summary>
const std::string Db::QueryGenerateUtility::make_update_query(const std::string& table_name, const std::vector<std::string>& columns, const std::string& where_clause)
{

	if (columns.size() == 0) return "";

	std::stringstream query;
	query << "UPDATE " + table_name + " SET ";

	for (std::vector<std::string>::const_iterator iter = columns.begin(); iter != columns.end(); iter++) {
		query << *iter << "=?, ";
	}

	std::string ret = query.str();
	ret.erase(ret.end() - 2, ret.end());
	if (where_clause.length() != 0) ret += " WHERE " + where_clause;
	return ret;
}


///<summary>
/// プレースホルダ付きでUpdateクエリを作成する
///</summary>
const std::string Db::QueryGenerateUtility::make_update_query(const Db::TableStructure& update_columns, const std::string& where_clause) 
{
	if (update_columns.get_column_list() == nullptr) return "";
	return make_update_query(update_columns.table_name, update_columns.get_column_names(), where_clause);
}