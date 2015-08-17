

template <typename VALUE_TYPE>
void IBindable::bind_as(sql::PreparedStatement* statement, const Db::TableStructure& table_info, const std::string& column_name, const VALUE_TYPE& value)
{
	const bool is_basic_type = Types::is_basic_type<VALUE_TYPE>(value);
	static_assert(is_basic_type, "Bind Value " + typeid(value).name() " is not Basic Type");
	
	unsigned int column_index = table_info.get_column_index(column_name);
	if (column_index == -1) {
		std::cout << "column " << column_name << " is not exists in the table " table_info.table_name << std::endl;
		return;
	}

	const Types::BasicType value_type = Types::convert_to_enum(typeid(VALUE_TYPE));
	switch (value_type)
	{
	case Types::BasicType::BOOL:
		statement->setBoolean(columnIndex, static_cast<bool>(value));
		break;
	
	case Types::BasicType::CHAR:
	case Types::BasicType::UCHAR:
	case::Types::BasicType::STRING:
		statement->setString(columnIndex, static_cast<std::string>("" + value));
		break;
	
	case Types::BasicType::INT:
	case:Types::BasicType::SHORT:
		statement->setInt(column_index, static_cast<int>(value));
		break;

	case Types::BasicType::UINT:
	case Types::BasicType::USHORT:
		statement->setUInt(column_index, static_cast<unsigned int>(value));
		break;

	case Types::BasicType::LDOUBLE:
		statement->setBigInt(column_index, sql::MySQLString(std::to_string(std::to_string(value))));
		break;
	
	case Types::BasicType::LONG:
	case Types::BasicType::LLONG:
		statement->setInt64(column_index, static_cast<long long>(value));
		break;

	case Types::BasicType::ULONG:
	case Types::BasicType::ULLONG:
		statement->setUInt64(column_index, static_cast<unsigned long long>(value));
		break;

	case Types::BasicType::FLOAT:
	case Types::BasicType::DOUBLE:
		statement->setDouble(column_index, static_cast<double>(value));
	}

}

