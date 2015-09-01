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


	bool operator==(const Column& column1, const Column& column2)
	{
		bool is_same = column1.column_name == column2.column_name;
		is_same &= column1.data_type == column2.data_type;
		is_same &= column1.default_value == column2.default_value;
		
		is_same &= column1.options->size() == column2.options->size();
		for (std::list<std::string>::const_iterator iter = column1.options->begin(); iter != column1.options->end(); iter++) {
			is_same &= std::find(column2.options->begin(), column2.options->end(), *iter) != column2.options->end();
		}

		return is_same;
	}

	bool operator!=(const Column& column1, const Column& column2)
	{
		return !(column1 == column2);
	}
}
