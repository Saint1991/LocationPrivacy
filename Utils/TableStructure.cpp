#include "stdafx.h"
#include "TableStructure.h"


namespace Db
{
	///<summary>
	/// �R���X�g���N�^
	///</summary>
	 TableStructure::TableStructure(const std::string& table_name, std::list<Column> columns)
		: table_name(table_name), columns(std::make_unique<std::list<Column>>(columns))
	{
		for (std::list<Column>::iterator iter = columns.begin(); iter != columns.end(); iter++) {
			auto target = std::find(iter->options->begin(), iter->options->end(), "PRIMARY KEY");
			if (target != iter->options->end()) {
				add_primary_key(iter->column_name);
				iter->options->erase(target);
			}
		}
	}


	///<summary>
	/// �R�s�[�R���X�g���N�^
	///</summary>
	 TableStructure::TableStructure(const  TableStructure& structure) : table_name(structure.table_name), columns(std::make_unique<std::list< Column>>())
	{
		for (std::list<Column>::const_iterator iter = structure.columns->begin(); iter != structure.columns->end(); iter++) {
			columns->push_back(*iter);
		}
		if (structure.primary_keys != nullptr) {
			primary_keys = std::make_unique<std::list<std::string>>();
			for (std::list<std::string>::const_iterator iter = structure.primary_keys->begin(); iter != structure.primary_keys->end(); iter++) {
				primary_keys->push_back(*iter);
			}
		}
	}

	///<summary>
	/// �f�X�g���N�^
	///</summary>
	 TableStructure::~TableStructure()
	{
	}


	///<summary>
	/// �w�肳�ꂽ�J�������̃J���������݂���ꍇtrue��Ԃ��܂�
	///</summary>
	bool  TableStructure::is_column_exists(const std::string& column_name) const
	{
		auto iter = std::find_if(columns->begin(), columns->end(), [column_name]( Column column) -> bool {
			return column_name == column.column_name;
		});
		return iter != columns->end();
		return false;
	}


	///<summary>
	/// �J�����̃C���f�b�N�X���擾����
	/// ������Ȃ��ꍇ��0��Ԃ��D
	/// INSERT���ł̃o�C���h���Ɏg�p�ł���
	///</summary>
	unsigned int  TableStructure::get_column_index(const std::string& column_name) const
	{
		int index = 1;
		for (std::list< Column>::const_iterator iter = columns->begin(); iter != columns->end(); iter++) {
			if (iter->column_name == column_name) break;
			index++;
		}
		return index > columns->size() ? 0 : index;
	}



	///<summary>
	/// �J�������̈ꗗ���擾����
	///</summary>
	const std::list<std::string>  TableStructure::get_column_name_list() const
	{
		std::list<std::string> column_names;
		for (std::list< Column>::const_iterator iter = columns->begin(); iter != columns->end(); iter++) {
			column_names.push_back(iter->column_name);
		}
		return column_names;
	}


	///<summary>
	/// to_string�ł�CREATE TABLE �N�G����Ԃ�
	/// �J�����������w�肳��Ă��Ȃ��ꍇ�͋󕶎����Ԃ�
	///</summary>
	const std::string TableStructure::to_string() const
	{
		if (columns == nullptr || columns->size() == 0 || table_name.length() == 0) return "";

		std::stringstream query;
		query << "CREATE TABLE " << table_name << " (";

		for (std::list<Column>::const_iterator iter = columns->begin(); iter != columns->end(); iter++) {
			query << iter->to_string() << ", ";
		}

		if (primary_keys != nullptr && primary_keys->size() > 0) {
			std::stringstream primary_key;
			primary_key << "PRIMARY KEY (";
			for (std::list<std::string>::const_iterator iter = primary_keys->begin(); iter != primary_keys->end(); iter++) {
				primary_key << *iter << ", ";
			}
			std::string primary_key_clause = primary_key.str();
			primary_key_clause.replace(primary_key_clause.end() - 2, primary_key_clause.end(), "), ");
			query << primary_key_clause;
		}

		std::string ret = query.str();
		ret.replace(ret.end() - 2, ret.end(), ") CHARACTER SET UTF8;");
		return ret;
	}

	///<summary>
	/// PRIMARY KEY�̐ݒ��ǉ����܂�
	/// ���݂��Ȃ��J�������w�肵���ꍇ��false��Ԃ��܂��D
	///</summary>
	bool  TableStructure::add_primary_key(std::string primary_key)
	{
		if (is_column_exists(primary_key)) {
			if (primary_keys == nullptr) primary_keys = std::make_unique<std::list<std::string>>();
			primary_keys->push_back(primary_key);
			return true;
		}
		return false;
	}
}

