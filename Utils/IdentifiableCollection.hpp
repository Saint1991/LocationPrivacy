
///<summary>
/// �R���X�g���N�^
///</summary>
template <typename T>
Collection::IdentifiableCollection<T>::IdentifiableCollection() 
{
	collection = std::make_unique< std::set < std::shared_ptr<T>, std::function<bool(const std::shared_ptr<T>, const std::shared_ptr<T>)>>>([](const std::shared_ptr<T> a, const std::shared_ptr<T> b) {
		return a->get_id() < b->get_id();
	});
}


///<summary>
/// �f�X�g���N�^
///</summary>
template <typename T>
Collection::IdentifiableCollection<T>::~IdentifiableCollection()
{
}


///<summary>
/// �w�肵��ID�����v�f��Ǐo����p�Ŏ擾����D
/// ������Ȃ��ꍇ��nullptr��Ԃ��D
/// �\�[�g�ς݂̃R���N�V�����ɑ΂��Ă�2���T��
/// �\�[�g����Ă��Ȃ��R���N�V�����ɑ΂��Ă͑O����S�T������D
///</summary>
template <typename T>
std::shared_ptr<T const> Collection::IdentifiableCollection<T>::read_by_id(const unsigned long &id) const
{
	auto target_iter = collection->find(std::make_shared<T>(id));
	if (target_iter == collection->end()) {
		return nullptr;
	}
	return *target_iter;
}

///<summary>
/// �w�肵��ID�����v�f��ύX�\�ȏ�ԂŎ擾����D
/// ������Ȃ��ꍇ��nullptr��Ԃ��D
/// �\�[�g�ς݂̃R���N�V�����ɑ΂��Ă�2���T��
/// �\�[�g����Ă��Ȃ��R���N�V�����ɑ΂��Ă͑O����S�T������D
///</summary>
template <typename T>
std::shared_ptr<T> Collection::IdentifiableCollection<T>::get_by_id(const unsigned long &id)
{
	auto target_iter = collection->find(std::make_shared<T>(id));
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
	auto target_iter = collection->find(std::make_shared<T>(id));

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
bool Collection::IdentifiableCollection<T>::contains(unsigned long id) const
{
	bool is_contained = read_by_id(id) != nullptr;
	return is_contained;
}

///<summary>
/// �w�肵��ID�����v�f���܂�ł��邩�𔻒肷��
///</summary>
template <typename T>
bool Collection::IdentifiableCollection<T>::contains(Identifiable id) const
{
	return contains(id.get_id());
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

	collection->insert(val);
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
