#ifdef UTILS_EXPORTS
#define IDENTIFIABLE_COLLECTION_API __declspec(dllexport)
#else
#define IDENTIFIABLE_COLLECTION_API __declspec(dllimport)
#endif

#pragma once
#include "Identifiable.h"

namespace Collection {

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
		bool is_sorted;
		std::_Vector_iterator<std::_Vector_val<std::_Simple_types<std::shared_ptr<T>>>> find_iter_by_id(const unsigned int &id);
		std::unique_ptr<std::vector<std::shared_ptr<T>>> collection;		

	public:
		IDENTIFIABLE_COLLECTION_API IdentifiableCollection();
		IDENTIFIABLE_COLLECTION_API virtual ~IdentifiableCollection();

		IDENTIFIABLE_COLLECTION_API std::shared_ptr<T> find_by_id(const unsigned int& id);
		IDENTIFIABLE_COLLECTION_API bool remove_by_id(const unsigned int& id);

		IDENTIFIABLE_COLLECTION_API std::size_t size();
		IDENTIFIABLE_COLLECTION_API void sort();
		IDENTIFIABLE_COLLECTION_API void add(const std::shared_ptr<T> &val);
		IDENTIFIABLE_COLLECTION_API void clear();
	};
}


