#pragma once
#include "Identifiable.h"

namespace Collection 
{

	///<summary>
	/// �\�[�g�ς݂����Ď����C�K�؂ȃA���S���Y���ŒT������R���N�V�����e���v���[�g
	/// ���ꉻ�ɂ�Identifiable���p�����Ă���N���X�̂ݎg�p�\
	///</summary>
	template <typename T>
	class IdentifiableCollection
	{
		
	//T��Identifiable���p�����Ă��邩����
	static_assert(std::is_base_of<Identifiable, T>::value, "Template Type T is not Derived from Identifiable at IdentifiableCollection.h");

	private:
		std::unique_ptr < std::set < std::shared_ptr<T>, std::function<bool (const std::shared_ptr<T>, const std::shared_ptr<T>)> > > collection;

	public:
		IdentifiableCollection();
		virtual ~IdentifiableCollection();

		std::shared_ptr<T const> read_by_id(const unsigned long& id) const;
		std::shared_ptr<T> get_by_id(const unsigned long& id);
		bool remove_by_id(const unsigned long& id);
		
		bool contains(Identifiable id) const;
		bool contains(unsigned long id) const;
		bool add(std::shared_ptr<T> val);
		bool add(T val);
		std::size_t size() const;
		void clear();
	};
}

#include "IdentifiableCollection.hpp"
