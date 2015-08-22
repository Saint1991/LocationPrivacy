
namespace Collection
{
	///<summary>
	/// �R���X�g���N�^
	///</summary>
	template <typename T>
	IdentifiableCollection<T>::IdentifiableCollection() : 
		std::set < std::shared_ptr<T>, std::function<bool(const std::shared_ptr<T>, const std::shared_ptr<T>)> >([](const std::shared_ptr<T> a, const std::shared_ptr<T> b) {
			return a->get_id() < b->get_id();
		}
	){}


	///<summary>
	/// �f�X�g���N�^
	///</summary>
	template <typename T>
	IdentifiableCollection<T>::~IdentifiableCollection()
	{
	}


	///<summary>
	/// �w�肵��ID�����v�f��Ǐo����p�Ŏ擾����D
	/// ������Ȃ��ꍇ��nullptr��Ԃ��D
	/// �\�[�g�ς݂̃R���N�V�����ɑ΂��Ă�2���T��
	/// �\�[�g����Ă��Ȃ��R���N�V�����ɑ΂��Ă͑O����S�T������D
	///</summary>
	template <typename T>
	std::shared_ptr<T const> IdentifiableCollection<T>::read_by_id(const long &id) const
	{
		auto target_iter = find(std::make_shared<Identifiable>(id));
		if (target_iter == end()) {
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
	std::shared_ptr<T> IdentifiableCollection<T>::get_by_id(const long &id)
	{
		auto target_iter = find(id);
		if (target_iter == end()) {
			return nullptr;
		}
		return *target_iter;
	}

	///<summary>
	/// �w�肵��ID�����v�f��T�����C�폜����D
	/// �v�f�����݂��C�폜�ł����ꍇ��true���C����ȊO�̏ꍇ��false��Ԃ��D
	///</summary>
	template <typename T>
	bool IdentifiableCollection<T>::remove_by_id(const long &id)
	{
		auto target_iter =  find(id);

		// �v�f��������Ȃ��ꍇ��false��Ԃ��ďI��
		if (target_iter == end()) {
			return false;
		}

		//�v�f�����������ꍇ�͍폜����ture��Ԃ�
		 erase(target_iter);
		return true;
	}

	///<summary>
	/// �܂܂�Ă���ID�ꗗ���擾����
	/// �����͏����Ƀ\�[�g�����
	///</summary>
	template <typename T>
	const std::unique_ptr<std::vector<long>> IdentifiableCollection<T>::get_id_list() const
	{
		std::unique_ptr<std::vector<long>> id_list = std::make_unique<std::vector<long>>(size(), INVALID);
		for (std::set< std::shared_ptr<T>>::const_iterator iter = begin(), long index = 0; iter != end(); iter++, index++) {
			id_list->at(index) = (*iter)->get_id();
		}
		return std::move(id_list);
	}

	///<summary>
	/// �w�肵��ID�����v�f���܂�ł��邩�𔻒肷��
	///</summary>
	template <typename T>
	bool IdentifiableCollection<T>::contains(long id) const
	{
		bool is_contained = find(id) != end();
		return is_contained;
	}


	///<summary>
	/// �����ID�����v�f���܂�ł��邩�𔻒肷��
	///</summary>
	template <typename T>
	bool IdentifiableCollection<T>::contains(T id) const
	{
		return contains(id.get_id());
	}

	///<summary>
	/// �����ID�����v�f���܂�ł��邩�𔻒肷��
	///</summary>
	template <typename T>
	bool IdentifiableCollection<T>::contains(std::shared_ptr<T const> id) const
	{
		return contains(id->get_id());
	}


	///<summary>
	/// �R���N�V�����̖����Ɏw�肵���v�f���i�[����
	/// ID���d������ꍇ��DuplicatedIdException���X���[����D
	/// ����ID��max_size�𒴂���̈���w�肵���ꍇ��out_of_range���X���[����
	///</summary>
	template <typename T>
	bool IdentifiableCollection<T>::add(std::shared_ptr<T> val)
	{
		//����ID�̏ꍇ�͒ǉ����Ȃ�
		long id = val->get_id();
		if (contains(val)) {
			throw DuplicatedIdException(id);
			return false;
		}
	
		// ID�����̏ꍇ��v�f����max_size���傫���Ȃ�ꍇ��out_of_range���X���[����
		if (id < 0 || size() == max_size()) {
			throw std::out_of_range("ID must be a natural number");
			return false;
		}

		 insert(val);
		return true;
	}

	///<summary>
	/// �R���N�V�����̖����Ɏw�肵���v�f���i�[����
	/// ID���d������ꍇ��DuplicatedIdException���X���[����D
	/// ����ID��max_size�𒴂���̈���w�肵���ꍇ��out_of_range���X���[����
	///</summary>
	template <typename T>
	bool IdentifiableCollection<T>::add(T val)
	{
		return add(std::make_shared<T>(val));
	}

	///<summary>
	/// �e�v�f�ɂ���execute_function�����s����
	/// ���g�̕ύX���e������̂Œ���
	///</summary>
	template <typename T>
	void IdentifiableCollection<T>::foreach(const std::function<void(std::shared_ptr<T>)>& execute_function)
	{
		for (std::set<std::shared_ptr<T>>::const_iterator iter =  begin(); iter != end(); iter++) {
			execute_function(*iter);
		}
	}

	///<summary>
	/// �e�v�f�ɂ���execute_function�����s����
	///</summary>
	template <typename T>
	void IdentifiableCollection<T>::foreach(const std::function<void(std::shared_ptr<T const>)>& execute_function) const
	{
		for (std::set<std::shared_ptr<T const>>::const_iterator iter = begin(); iter != end(); iter++) {
			execute_function(*iter);
		}
	}

}
