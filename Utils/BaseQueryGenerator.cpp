#include "stdafx.h"
#include "BaseQueryGenerator.h"



///<summary>
/// �R���X�g���N�^
///</summary>
Db::BaseQueryGenerator::BaseQueryGenerator(Db::TableStructure table_info) : table_info(std::make_unique<Db::TableStructure>(table_info))
{
}


///<summary>
/// �f�X�g���N�^
///</summary>
Db::BaseQueryGenerator::~BaseQueryGenerator()
{
}


///<summary>
/// ()�ň͂܂ꂽ�������쐬���郆�[�e�B���e�B
///</summary>
std::string Db::BaseQueryGenerator::make_round_bracket_clause(const std::list<std::string>& elements)
{
	if (elements.size() == 0) return "";
	std::stringstream stream("(");
	for (std::list<std::string>::const_iterator iter = elements.begin(); iter != elements.end(); iter++) {
		stream << *iter << ", ";
	}
	std::string ret = stream.str();
	ret.replace(ret.end() - 2, ret.end(), ")");
	return ret;
}


///<summary>
/// ()�ň͂܂ꂽ�������쐬���郆�[�e�B���e�B
///</summary>
std::string Db::BaseQueryGenerator::make_round_bracket_clause(const std::list<std::shared_ptr<Serializable>>& elements)
{
	if (elements.size() == 0) return "";
	std::stringstream stream("(");
	for (std::list<std::shared_ptr<Serializable>>::const_iterator iter = elements.begin(); iter != elements.end(); iter++) {
		stream << (*iter)->to_string() << ", ";
	}
	std::string ret = stream.str();
	ret.replace(ret.end() - 2, ret.end(), ")");
	return ret;
}


