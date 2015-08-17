

///<summary>
/// 基本型のtype_infoを返すユーティリティ
///</summary>
const type_info& Types::convert_to_type_info(Types::BasicType type)
{
	const type_info& ret = typeid(nullptr);
	switch (type) {
	case BOOL:
		return typeid(bool);
	case CHAR:
		return typeid(char);
	case UCHAR:
		return typeid(unsigned char);
	case SHORT:
		return typeid(short);
	case USHORT:
		return typeid(unsigned short);
	case INT:
		return typeid(int);
	case 	UINT:
		return typeid(unsigned int);
	case 	LONG:
		return typeid(long);
	case ULONG:
		return typeid(unsigned long);
	case FLOAT:
		return typeid(float);
	case DOUBLE:
		return typeid(double);
	case LDOUBLE:
		return typeid(long double);
	case LLONG:
		return typeid(long long);
	case STRING:
		return typeid(std::string);
	case ULLONG:
		return typeid(unsigned long long);
	}
	return ret;
}


///<summary>
/// type_infoからBasicTypeのenumに変換する
///</summary>
const Types::BasicType Types::convert_to_enum(const type_info& type_info)
{
	Types::BasicType ret = Types::BasicType::INVALID;
	if (type_info == typeid(bool)) return Types::BasicType::BOOL;
	else if (type_info == typeid(char)) return Types::BasicType::CHAR;
	else if (type_info == typeid(unsigned char)) return Types::BasicType::UCHAR;
	else if (type_info == typeid(short)) return Types::BasicType::SHORT;
	else if (type_info == typeid(unsigned short)) return Types::BasicType::USHORT;
	else if (type_info == typeid(int)) return Types::BasicType::INT;
	else if (type_info == typeid(unsigned int)) return Types::BasicType::UINT;
	else if (type_info == typeid(long)) return Types::BasicType::LONG;
	else if (type_info == typeid(unsigned long)) return Types::BasicType::ULONG;
	else if (type_info == typeid(float)) return Types::BasicType::FLOAT;
	else if (type_info == typeid(double)) return Types::BasicType::DOUBLE;
	else if (type_info == typeid(long double)) return Types::BasicType::LDOUBLE;
	else if (type_info == typeid(long long)) return Types::BasicType::LLONG;
	else if (type_info == typeid(std::string)) return Types::BasicType::STRING;
	else if (type_info == typeid(unsigned long long)) return Types::BasicType::ULLONG;
	return ret;
}


///<summary>
/// 規定したBasicTypesかどうか判定する
///</summary>
template <typename VALUE_TYPE>
const bool Types::is_basic_type(const VALUE_TYPE& value)
{
	BasicType type = convert_to_enum(typeid(VALUE_TYPE));
	return type != INVALID;
}


///<summary>
/// リストで受け取ったタイプのいずれかであるかを判定する
///</summary>
template <typename VALUE_TYPE>
const bool Types::is_type_of(const std::list<BasicType>& available_types, const VALUE_TYPE& value)
{
	BasicType type = convert_to_enum(typeid(VALUE_TYPE));
	if (type == INVALID) return false;
	return std::find(available_types.begin(), available_types.end(), value) != available_types.end();
}