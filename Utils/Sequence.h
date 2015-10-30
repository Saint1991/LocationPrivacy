#pragma once
#include "stdafx.h"

namespace Collection
{
	template <typename T>
	class Sequence : public std::vector<T>
	{
	public:
		Sequence();
		Sequence(size_t n);
		Sequence(const std::vector<T>& sequence);
		~Sequence();

		Sequence<T> subsequence(int from, int to) const;
		void for_each_gram(const std::function<void(iterator, iterator)>& execute_function);
		void for_each_gram(const std::function<void(const_iterator, const_iterator)>& execute_function) const;
		void for_each_prefix(const std::function<void(iterator, iterator)>& execute_function);
		void for_each_prefix(const std::function<void(const_iterator,const_iterator)>& execute_function) const;
	};
}

#include "Sequence.hpp"
