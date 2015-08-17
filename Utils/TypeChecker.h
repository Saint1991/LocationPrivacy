#pragma once
#include "stdafx.h"

namespace Type
{
	class TypeChecker
	{
	public:
		template <typename VALUE_TYPE>
		static void check_statically(const std::list<Types::BasicType>& available_types, VALUE_TYPE value);
	};
}

#include "TypeChecker.hpp"
