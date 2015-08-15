#include "stdafx.h"
#include "QueryGenerator.h"



///<summary>
/// �R���X�g���N�^
///</summary>
Db::QueryGenerator::QueryGenerator(Db::TableStructure table_info) : BaseQueryGenerator(table_info)
{
}


///<summary>
/// �f�X�g���N�^
///</summary>
Db::QueryGenerator::~QueryGenerator()
{
}


///<summary>
/// CREATE TABLE�N�G�����쐬
/// �J�����f�[�^���Z�b�g����Ă��Ȃ��ꍇ�͋󕶎����Ԃ�
///</summary>
const std::string Db::QueryGenerator::make_create_table_query() const
{
	if (table_info->columns == nullptr || table_info->columns->size() == 0) return "";
	
	std::stringstream query;
	query << "CREATE TABLE " << table_info->table_name << " ";
	std::string column_clause = " " + make_round_bracket_clause(table_info->get_column_name_list());
	
	if (table_info->primary_keys != nullptr && table_info->primary_keys->size() > 0) {
		std::string primary_key = ", PRIMARY KEY" + make_round_bracket_clause(*table_info->primary_keys) + ")";
		column_clause.replace(column_clause.end() - 1, column_clause.end(), primary_key);
	}

	query << column_clause << " CHARACTER SET UTF8;";
	return query.str();
}


///<summary>
/// INSERT�N�G�����v���[�X�z���_�t���ō쐬����
/// �J�������͍ŏ��ɓn�������X�g�̏��ɂȂ�
///</summary>
const std::string Db::QueryGenerator::make_insert_query() const
{
	if (table_info->columns == nullptr || table_info->columns->size() == 0) return "";

	std::string column_clause = make_round_bracket_clause(table_info->get_column_name_list());
	std::string value_clause = make_round_bracket_clause(std::list<std::string>(table_info->columns->size(), "?"));
	
	std::stringstream query;
	query << "INSERT INTO " << table_info->table_name << " " << column_clause << " VALUES " << value_clause;

	return query.str();
}


///<summary>
/// ��{�I��SELECT�N�G�����쐬����
///</summary>
const std::string Db::QueryGenerator::make_select_query(const std::string& where_clause) const
{
	if (table_info->columns == nullptr || table_info->columns->size() == 0) return "";

	std::string column_clause = make_round_bracket_clause(table_info->get_column_name_list());
	std::stringstream query;
	
	query << "SELECT " << column_clause << " FROM " << table_info->table_name;
	if (where_clause.length() != 0) query << " WHERE " + where_clause;
	return query.str();
}



///<summary>
/// �v���[�X�z���_�t����Update�N�G�����쐬����
///</summary>
const std::string Db::QueryGenerator::make_update_query(const std::string& where_clause) const
{

	if (table_info->columns == nullptr || table_info->columns->size() == 0) return "";

	std::stringstream query;
	query << "UPDATE " + table_info->table_name + " SET ";

	std::list<std::string> column_list = table_info->get_column_name_list();
	for (std::list<std::string>::const_iterator iter = column_list.begin(); iter != column_list.end(); iter++) {
		query << *iter << "=?, ";
	}

	std::string ret = query.str();
	ret.erase(ret.end() - 2, ret.end());
	if (where_clause.length() != 0) ret += " WHERE " + where_clause;
	return ret;
}
