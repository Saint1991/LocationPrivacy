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
		IdentifiableCollection();
		virtual ~IdentifiableCollection();

		std::shared_ptr<T> find_by_id(const unsigned int& id);
		bool remove_by_id(const unsigned int& id);

		std::size_t size();
		void sort();
		void add(const std::shared_ptr<T> &val);
		void clear();
	};
}






///<summary>
/// �R���X�g���N�^
///</summary>
template <typename T>
Collection::IdentifiableCollection<T>::IdentifiableCollection()
{
}


///<summary>
/// �f�X�g���N�^
///</summary>
template <typename T>
Collection::IdentifiableCollection<T>::~IdentifiableCollection()
{
}

///<summary>
/// �w�肵��ID�����v�f�̃C�e���[�^�Q�Ƃ�Ԃ��D
/// �\�[�g�ς݂̃R���N�V�����ɑ΂��Ă�2���T��
/// �\�[�g����Ă��Ȃ��R���N�V�����ɑ΂��Ă͑O����S�T������D
///</summary>
template <typename T>
std::_Vector_iterator<std::_Vector_val<std::_Simple_types<std::shared_ptr<T>>>> Collection::IdentifiableCollection<T>::find_iter_by_id(const unsigned int &id)
{
	std::_Vector_iterator<std::_Vector_val<std::_Simple_types<std::shared_ptr<T>>>> found_iter = is_sorted ?
		std::lower_bound(collection->begin(), collection->end(), id) :
		std::find(collection->begin(), collection->end(), id);
	return found_iter;
}

///<summary>
/// �w�肵��ID�����v�f��Ԃ��D
/// ������Ȃ��ꍇ��nullptr��Ԃ��D
/// �\�[�g�ς݂̃R���N�V�����ɑ΂��Ă�2���T��
/// �\�[�g����Ă��Ȃ��R���N�V�����ɑ΂��Ă͑O����S�T������D
///</summary>
template <typename T>
std::shared_ptr<T> Collection::IdentifiableCollection<T>::find_by_id(const unsigned int &id)
{
	std::_Vector_iterator<std::_Vector_val<std::_Simple_types<std::shared_ptr<T>>>> target_iter = find_iter_by_id(id);
	if (target_iter->get()->get_id() != id) {
		return nullptr;
	}
	return *target_iter;
}

///<summary>
/// �w�肵��ID�����v�f��T�����C�폜����D
/// �v�f�����݂��C�폜�ł����ꍇ��true���C����ȊO�̏ꍇ��false��Ԃ��D
///</summary>
template <typename T>
bool Collection::IdentifiableCollection<T>::remove_by_id(const unsigned int &id)
{
	std::_Vector_iterator<std::_Vector_val<std::_Simple_types<std::shared_ptr<Identifiable>>>> target_iter = find_iter_by_id(id);

	// �v�f��������Ȃ��ꍇ��false��Ԃ��ďI��
	if (target_iter->get()->get_id() != id) {
		return false;
	}

	//�v�f�����������ꍇ�͍폜����ture��Ԃ�
	collection->erase(target_iter);
	return true;
}

///<summary>
/// �v�f��ID�ŏ����Ƀ\�[�g����D
///</summary>
template <typename T>
void Collection::IdentifiableCollection<T>::sort()
{
	std::sort(collection->begin(), collection->end(), [](std::shared_ptr<T> item1, std::shared_ptr<Identifiable> item2) { return item1->get_id() < item2->get_id(); });
	is_sorted = true;
}

///<summary>
/// �R���N�V�����̖����Ɏw�肵���v�f���i�[����
///</summary>
template <typename T>
void Collection::IdentifiableCollection<T>::add(const::std::shared_ptr<T> &val)
{
	collection->push_back(val);
	is_sorted = false;
}

///<summary>
/// �R���N�V�������̗v�f��S�č폜
///</summary>
template <typename T>
void Collection::IdentifiableCollection<T>::clear()
{
	collection->clear();
}


///<summary>
/// �R���N�V�������̗v�f�����擾����
///</summary>
template <typename T>
std::size_t Collection::IdentifiableCollection<T>::size()
{
	return collection->size();
}


