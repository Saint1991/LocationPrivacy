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
		std::unique_ptr<std::vector<std::shared_ptr<T>>> collection;		

	public:
		IdentifiableCollection();
		virtual ~IdentifiableCollection();

		std::shared_ptr<T> find_by_id(const unsigned long& id);
		bool remove_by_id(const unsigned long& id);
		
		bool contains(Identifiable id);
		bool contains(unsigned long id);
		bool add(std::shared_ptr<T> val);
		bool add(T val);
		void sort();
		std::size_t size() const;
		void clear();
	};
}






///<summary>
/// �R���X�g���N�^
///</summary>
template <typename T>
Collection::IdentifiableCollection<T>::IdentifiableCollection() : collection(std::make_unique < std::vector<std::shared_ptr<T>>>())
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
/// �w�肵��ID�����v�f��Ԃ��D
/// ������Ȃ��ꍇ��nullptr��Ԃ��D
/// �\�[�g�ς݂̃R���N�V�����ɑ΂��Ă�2���T��
/// �\�[�g����Ă��Ȃ��R���N�V�����ɑ΂��Ă͑O����S�T������D
///</summary>
template <typename T>
std::shared_ptr<T> Collection::IdentifiableCollection<T>::find_by_id(const unsigned long &id)
{
	auto target_iter = is_sorted ? std::lower_bound(collection->begin(), collection->end(), id) : 	std::find(collection->begin(), collection->end(), id);
	if (target_iter == collection->end() || target_iter->get()->get_id() != id) {
		return nullptr;
	}
	return *target_iter;
}

///<summary>
/// �w�肵��ID�����v�f��T�����C�폜����D
/// �v�f�����݂��C�폜�ł����ꍇ��true���C����ȊO�̏ꍇ��false��Ԃ��D
///</summary>
template <typename T>
bool Collection::IdentifiableCollection<T>::remove_by_id(const unsigned long &id)
{
	auto target_iter = is_sorted ? std::lower_bound(collection->begin(), collection->end(), id) : std::find(collection->begin(), collection->end(), id);

	// �v�f��������Ȃ��ꍇ��false��Ԃ��ďI��
	if (target_iter->get()->get_id() != id) {
		return false;
	}

	//�v�f�����������ꍇ�͍폜����ture��Ԃ�
	collection->erase(target_iter);
	return true;
}

///<summary>
/// �w�肵��ID�����v�f���܂�ł��邩�𔻒肷��
///</summary>
template <typename T>
bool Collection::IdentifiableCollection<T>::contains(unsigned long id)
{
	bool is_contained = find_by_id(id) != nullptr;
	return is_contained;
}

///<summary>
/// �w�肵��ID�����v�f���܂�ł��邩�𔻒肷��
///</summary>
template <typename T>
bool Collection::IdentifiableCollection<T>::contains(Identifiable id)
{
	return contains(id.get_id());
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
/// ID���d������ꍇ��DuplicatedIdException���X���[����D
///</summary>
template <typename T>
bool Collection::IdentifiableCollection<T>::add(std::shared_ptr<T> val)
{
	
	//����ID�̏ꍇ�͒ǉ����Ȃ�
	unsigned long id = val->get_id();
	if (contains(id)) {
		throw DuplicatedIdException(id);
		return false;
	}
	
	collection->push_back(val);
	is_sorted = false;
	return true;
}

///<summary>
/// �R���N�V�����̖����Ɏw�肵���v�f���i�[����
/// ID���d������ꍇ��DuplicatedIdException���X���[����D
///</summary>
template <typename T>
bool Collection::IdentifiableCollection<T>::add(T val)
{
	return add(std::make_shared<T>(val));
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
std::size_t Collection::IdentifiableCollection<T>::size() const
{
	return collection->size();
}
