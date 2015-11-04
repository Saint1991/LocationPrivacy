#pragma once
#include "stdafx.h"
#include "Identifiable.h"

namespace Collection 
{

	///<summary>
	/// ID付きの要素を格納するコレクション
	/// IDは昇順にソートされる
	/// ID_TYPEはIdentifiableのType (ただし，operator< での比較可能性があるクラスのみ)
	/// DATA_TYPEにはIdentifiableを継承しているクラスのみ使用可能
	/// ID_TYPEはDATA_TYPEが持っているIDの型と一致していなくてはならないので注意
	///</summary>
	template <typename ID_TYPE, typename DATA_TYPE>
	class IdentifiableCollection : public std::set<std::shared_ptr<Identifiable<ID_TYPE>>, std::function<bool(const std::shared_ptr<Identifiable<ID_TYPE>>, const std::shared_ptr<Identifiable<ID_TYPE>>)>>
	{

	static_assert(std::is_integral<ID_TYPE>::value || std::is_same<std::string, ID_TYPE>::value, "Only Integral type and string are allowed for ID_TYPE");
	static_assert(std::is_base_of<Identifiable<ID_TYPE>, DATA_TYPE>::value, "Template type DATA_TYPE is not derived from Identifiable<ID_TYPE>");
	
	public:
		IdentifiableCollection();
		virtual ~IdentifiableCollection();

		std::shared_ptr<DATA_TYPE const> read_by_id(const ID_TYPE& id) const;
		std::shared_ptr<DATA_TYPE> get_by_id(const ID_TYPE& id);
		bool remove_by_id(const ID_TYPE& id);
		
		std::unique_ptr<std::vector<ID_TYPE>> get_id_list() const;
		bool contains(const DATA_TYPE& id) const;
		bool contains(std::shared_ptr<DATA_TYPE const> id) const;
		bool contains(const ID_TYPE& id) const;
		bool add(std::shared_ptr<DATA_TYPE> val);
		bool add(DATA_TYPE val);
		size_t size() const;
		void foreach(const std::function<void(std::shared_ptr<DATA_TYPE>)>& execute_function);		
		void foreach(const std::function<void(std::shared_ptr<DATA_TYPE const>)>& execute_function) const;
	};
}

#include "IdentifiableCollection.hpp"
