#include "stdafx.h"
#include "IdentifiableCollection.h"


Collection::IdentifiableCollection::IdentifiableCollection()
{
}


Collection::IdentifiableCollection::~IdentifiableCollection()
{
}

///<summary>
/// �w�肵��ID�����v�f�̃C�e���[�^�Q�Ƃ�Ԃ��D
/// �\�[�g�ς݂̃R���N�V�����ɑ΂��Ă�2���T��
/// �\�[�g����Ă��Ȃ��R���N�V�����ɑ΂��Ă͑O����S�T������D
///</summary>
std::_Vector_iterator<std::_Vector_val<std::_Simple_types<std::shared_ptr<Identifiable>>>> Collection::IdentifiableCollection::find_iter_by_id(const unsigned int &id)
{
	std::_Vector_iterator<std::_Vector_val<std::_Simple_types<std::shared_ptr<Identifiable>>>> found_iter = is_sorted ?
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
std::shared_ptr<Identifiable> Collection::IdentifiableCollection::find_by_id(const unsigned int &id)
{
	std::_Vector_iterator<std::_Vector_val<std::_Simple_types<std::shared_ptr<Identifiable>>>> target_iter = find_iter_by_id(id);
	if (target_iter->get()->get_id() != id) {
		return nullptr;
	}
	return *target_iter;
}

///<summary>
/// �w�肵��ID�����v�f��T�����C�폜����D
/// �v�f�����݂��C�폜�ł����ꍇ��true���C����ȊO�̏ꍇ��false��Ԃ��D
///</summary>
bool Collection::IdentifiableCollection::remove_by_id(const unsigned int &id)
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
void Collection::IdentifiableCollection::sort()
{
	std::sort(collection->begin(), collection->end(), [](std::shared_ptr<Identifiable> item1, std::shared_ptr<Identifiable> item2) { return item1->get_id() < item2->get_id(); });
	is_sorted = true;
}

///<summary>
/// �R���N�V�����̖����Ɏw�肵���v�f���i�[����
///</summary>
void Collection::IdentifiableCollection::add(const::std::shared_ptr<Identifiable> &val)
{
	collection->push_back(val);
	is_sorted = false;
}

///<summary>
/// �R���N�V�������̗v�f��S�č폜
///</summary>
void Collection::IdentifiableCollection::clear()
{
	collection->clear();
}


///<summary>
/// �R���N�V�����̌��݊m�ۂ���Ă��郁�����e��(�v�f��)���擾����
///</summary>
std::vector<std::shared_ptr<Identifiable>>::size_type Collection::IdentifiableCollection::capacity()
{
	return collection->capacity();
}

///<summary>
/// �R���N�V�����ɑ΂��Ċm�ۂł���ő�e��(�v�f��)���擾����
///</summary>
std::vector<std::shared_ptr<Identifiable>>::size_type Collection::IdentifiableCollection::max_size()
{
	return collection->max_size();
}

///<summary>
/// �R���N�V�������̗v�f�����擾����
///</summary>
std::size_t Collection::IdentifiableCollection::size()
{
	return collection->size();
}

