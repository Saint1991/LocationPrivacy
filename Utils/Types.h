#pragma once
#include "stdafx.h"


///<summary>
/// �^����Enum�ň������߂ɖ���������Ă���
/// C++�̌���d�l����������Ă������菑����悤�ɂȂ�̂����҂�����
///</summary>
class Types
{
public:
	
	enum BasicType
	{
		BOOL, 
		CHAR, 
		UCHAR, 
		SHORT, 
		USHORT, 
		INT, 
		UINT, 
		LONG, 
		ULONG, 
		FLOAT, 
		DOUBLE, 
		LDOUBLE, 
		LLONG,
		STRING,
		ULLONG,
		INVALID
	};

	static const type_info& convert_to_type_info(BasicType type);
	static const BasicType convert_to_enum(const type_info& type_info);
	
	template <typename VALUE_TYPE>
	static const bool is_basic_type(const VALUE_TYPE& value);
	
	template <typename VALUE_TYPE>
	static const bool is_type_of(const std::list<BasicType>& available_types, const VALUE_TYPE& value);

};

#include "Types.hpp"