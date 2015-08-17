
///<summary>
/// valueの型がavailable_typesかチェックし，違う場合にコンパイルエラーを発生させる
///</summary>
template <typename VALUE_TYPE>
void Type::TypeChecker::check_statically(const std::list<Types::BasicType>& available_types, VALUE_TYPE value)
{
	const bool is_available_type = Types::is_type_of(available_types, value);
	std::string validation_error_message = "type " + typeid(value).name() + " is not available";
	static_assert(is_available_type, validation_error_message);
}
