#include "stdafx.h"
#include "Column.h"


namespace Db
{

	///<summary>
	/// コンストラクタ
	///</summary>
	 Column::Column(const std::string& column_name, const std::string& type, std::list<std::string> options, const std::string& default_value) :
		column_name(column_name), data_type(type), options(std::make_unique<std::list<std::string>>(options)), default_value(default_value)
	{
	}


	///<summary>
	/// コピーコンストラクタ
	///</summary>
	 Column::Column(const  Column& column) : column_name(column.column_name), data_type(column.data_type), default_value(column.default_value), options(std::make_unique<std::list<std::string>>())
	{
		for (std::list<std::string>::const_iterator iter = column.options->begin(); iter != column.options->end(); iter++) {
			options->push_back(*iter);
		}
	}


	///<summary>
	/// デストラクタ
	///</summary>
	 Column::~Column()
	{
	}


	///<summary>
	/// 設定された項目を基にクエリ文字列を生成する
	///</summary>
	const std::string  Column::to_string() const
	{
		std::stringstream column;
		column << column_name << " " << data_type << " ";
		if (default_value != "") column << "DEFAULT " << default_value << " ";
		for (std::list<std::string>::iterator iter = options->begin(); iter != options->end(); iter++) {
			column << *iter << " ";
		}
		return column.str();
	}
}
