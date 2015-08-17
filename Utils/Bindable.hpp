

///<summary>
/// �^���`�F�b�N���ăR���p�C���G���[�𔭐�������
/// �኱�������R���p�C�����Ɍv�Z����Ă���͂�
///</summary>
template <typename VALUE_TYPE>
constexpr const Bindable::AvailableValueType get_mysql_type_with_validate()
{
	//�^�`�F�b�N(��{�̌^ + string���g�p�\)
	constexpr type_info& type = typeid(VALUE_TYPE);
	constexpr bool is_valid = std::is_fundamental<VALUE_TYPE>::value && !std::is_void<VALUE_TYPE>::value || type == typeid(std::string);
	static_assert(is_valid, "Bind Value " + typeid(value).name() " is not Basic Type");

	// MySQL�̌^���� 
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
/// Bindable����������N���X���Ŏg���郆�[�e�B���e�B
/// static_if������d�l�ɑg�ݍ��܂ꂽ�牺��switch����static_if���ÓI�ȃ`�F�b�N�ɂɒ����Ďg���Ƃ��悢
///</summary>
template <typename VALUE_TYPE>
void Bindable::bind_as(sql::PreparedStatement* statement, const Db::TableStructure& table_info, const std::string& column_name, const VALUE_TYPE& value)
{
	
	//�^�`�F�b�N��MySQL�̌^�ւ̃}�b�s���O (�R���p�C�����̃n�Y)
	constexpr Bindable::AvailableValueType mysql_type = get_mysql_type_with_validate();	

	//�J�����̃C���f�b�N�X���擾
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

