#include "stdafx.h"
#include "TableStructure.h"


///<summary>
/// コンストラクタ
///</summary>
Db::TableStructure::TableStructure(std::string table_name, std::list<Db::Column> columns) 
	: table_name(table_name), columns(std::make_unique<std::list<Db::Column>>(columns))
{
}


///<summary>
/// コピーコンストラクタ
///</summary>
Db::TableStructure::TableStructure(const Db::TableStructure& structure) : table_name(structure.table_name), columns(std::make_unique<std::list<Db::Column>>())
{
	for (std::list<Db::Column>::const_iterator iter = structure.columns->begin(); iter != structure.columns->end(); iter++) {
		columns->push_back(*iter);
	}
	if (structure.primary_keys != nullptr) primary_keys = std::make_unique<std::list<std::string>>();
	for (std::list<std::string>::const_iterator iter = structure.primary_keys->begin(); iter != structure.primary_keys->end(); iter++) {
		primary_keys->push_back(*iter);
	}
}

///<summary>
/// デストラクタ
///</summary>
Db::TableStructure::~TableStructure()
{
}


///<summary>
/// 指定されたカラム名のカラムが存在する場合trueを返します
///</summary>
bool Db::TableStructure::is_column_exists(const std::string& column_name) const
{
	auto iter = std::find_if(columns->begin(), columns->end(), [column_name](Db::Column column) -> bool {
		return column_name == column.column_name;
	});
	return iter != columns->end();
	return false;
}


///<summary>
/// カラムのインデックスを取得する
/// 見つからない場合は0を返す．
/// INSERT等でのバインド時に使用できる
///</summary>
const int Db::TableStructure::get_column_index(const std::string& column_name) const
{
	int index = 1;
	for (std::list<Db::Column>::const_iterator iter = columns->begin(); iter != columns->end(); iter++) {
		if (iter->column_name == column_name) break;
		index++;
	}
	return index > columns->size() ? 0 : index;
}



///<summary>
/// カラム名の一覧を取得する
///</summary>
const std::list<std::string> Db::TableStructure::get_column_name_list() const
{
	std::list<std::string> column_names;
	for (std::list<Db::Column>::const_iterator iter = columns->begin(); iter != columns->end(); iter++) {
		column_names.push_back(iter->column_name);
	}
	return column_names;
}


///<summary>
/// PRIMARY KEYの設定を追加します
/// 存在しないカラムを指定した場合はfalseを返します．
///</summary>
bool Db::TableStructure::add_primary_key(std::string primary_key)
{
	if (is_column_exists(primary_key)) {
		if (primary_keys == nullptr) primary_keys = std::make_unique<std::list<std::string>>();
		primary_keys->push_back(primary_key);
		return true;
	}
	return false;
}

