#pragma once
#include "stdafx.h"
#include "Identifiable.h"
#include <set>
#include <iterator>

namespace Collection 
{

	///<summary>
	/// ID�t���̗v�f���i�[����R���N�V����
	/// ID�͏����Ƀ\�[�g�����
	/// ���ꉻ�ɂ�Identifiable���p�����Ă���N���X�̂ݎg�p�\
	///</summary>
	template <typename T>
	class IdentifiableCollection : std::set<std::shared_ptr<T>, std::function<bool(const std::shared_ptr<T>, const std::shared_ptr<T>)>>
	{
		
	//T��Identifiable���p�����Ă��邩����
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
