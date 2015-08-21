#pragma once
#include "stdafx.h"
#include "Identifiable.h"
#include <set>
#include <iterator>

namespace Collection 
{

	///<summary>
	/// ID付きの要素を格納するコレクション
	/// IDは昇順にソートされる
	/// 特殊化にはIdentifiableを継承しているクラスのみ使用可能
	///</summary>
	template <typename T>
	class IdentifiableCollection : std::set<std::shared_ptr<T>, std::function<bool(const std::shared_ptr<T>, const std::shared_ptr<T>)>>
	{
		
	//TがIdentifiableを継承しているか判定
	static_assert(std::is_base_of<Identifiable, T>::value, "Template type T is not Derived from Identifiable");

	public:
		IdentifiableCollection();
		virtual ~IdentifiableCollection();

		std::shared_ptr<T const> read_by_id(const long& id) const;
		std::shared_ptr<T> get_by_id(const long& id);
		bool remove_by_id(const long& id);
		
		const std::vector<long> get_id_list() const;
		bool contains(Identifiable id) const;
		bool contains(long id) const;
		bool add(std::shared_ptr<T> val);
		bool add(T val);
		void foreach(const std::function<void(std::shared_ptr<T>)>& execute_function);		
	};
}

#include "IdentifiableCollection.hpp"
