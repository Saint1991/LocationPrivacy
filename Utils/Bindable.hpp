

///<summary>
/// 型をチェックしてコンパイルエラーを発生させる
/// 若干汚いがコンパイル時に計算されているはず
///</summary>
template <typename VALUE_TYPE>
constexpr const Bindable::AvailableValueType get_mysql_type_with_validate()
{
	//型チェック(基本の型 + stringが使用可能)
	constexpr type_info& type = typeid(VALUE_TYPE);
	constexpr bool is_valid = std::is_fundamental<VALUE_TYPE>::value && !std::is_void<VALUE_TYPE>::value || type == typeid(std::string);
	static_assert(is_valid, "Bind Value " + typeid(value).name() " is not Basic Type");

	// MySQLの型判定 
	constexpr bool is_bool = type == typeid(bool);
	constexpr bool is_string = type == typeid(char) || type == typeid(char16_t) || type == typeid(char32_t) || type == typeid(wchar_t) || type == typeid(signed char) || type == typeid(unsigned char);
	constexpr bool is_int = type == typeid(int) || type == typeid(short int);
	constexpr bool is_int64 == type == typeid(long int) || type == typeid(long long int);
	constexpr bool is_uint == type == typeid(unsigned int) || type == typeid(unsigned short int);
	constexpr bool is_uint64 = type == typeid(unsigned long int) || type == typeid(unsigned long long int);
	constexpr bool is_double = type == typeid(float) || type == typeid(double);
	constexpr bool is_long_double == type == typeid(long double);
	constexpr bool is_null = type == typeid(nullptr_t);

	if (is_bool) return Bindable::AvailableValueType::BOOL;
	else if (is_string) return Bindable::AvailableValueType::STRING;
	else if (is_int) return Bindable::AvailableValueType::INT;
	else if (is_int64) return Bindable::AvailableValueType::INT64;
	else if (is_uint) return Bindable::AvailableValueType::UINT;
	else if (is_uint64) return Bindable::AvailableValueType::UINT64;
	else if (is_double) return Bindable::AvailableValueType::DOUBLE;
	else if (is_long_double) return Bindable::AvailableValueType::LDOUBLE;
	else if (is_null) return Bindable::AvailableValueType::SQL_NULL;
	return Bindable::AvailableValueType::INVALID;
}

///<summary>
/// Bindableを実装するクラス内で使えるユーティリティ
/// static_ifが言語仕様に組み込まれたら下のswitch文をstatic_if等静的なチェックにに直して使うとよりよい
///</summary>
template <typename VALUE_TYPE>
void Bindable::bind_as(sql::PreparedStatement* statement, const Db::TableStructure& table_info, const std::string& column_name, const VALUE_TYPE& value)
{
	
	//型チェックとMySQLの型へのマッピング (コンパイル時のハズ)
	constexpr Bindable::AvailableValueType mysql_type = get_mysql_type_with_validate();	

	//カラムのインデックスを取得
	unsigned int column_index = table_info.get_column_index(column_name);
	if (column_index == -1) {
		std::cout << "column " << column_name << " is not exists in the table " table_info.table_name << std::endl;
		return;
	}
	
	switch (mysql_type) {
	case BOOL:
		statement->setBoolean(column_index, value);
		break;
	case STRING:
		statement->setString(column_index, value);
		break;
	case INT:
		statement->setInt(column_index, value);
		break;
	case INT64:
		statement->setInt64(column_index, value);
		break;
	case UINT:
		statement->setUInt(column_index, value);
		break;
	case UINT64:
		statement->setUInt64(column_index, value);
		break;
	case DOUBLE:
		statement->setDouble(column_index);
	case LDOUBLE:
		statement->setString(column_index, std::to_string(value));
		break;
	case SQL_NULL:
		statement->setNull(column_index, value);
		break;
	}
}

