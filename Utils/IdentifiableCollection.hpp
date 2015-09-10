
namespace Collection
{
	///<summary>
	/// �R���X�g���N�^
	///</summary>
	template <typename ID_TYPE, typename DATA_TYPE>
	IdentifiableCollection<ID_TYPE, DATA_TYPE>::IdentifiableCollection() : 
		std::set < std::shared_ptr<Identifiable<ID_TYPE>>, 
		std::function<bool(const std::shared_ptr<Identifiable<ID_TYPE>>, const std::shared_ptr<Identifiable<ID_TYPE>>)> 
	>([](const std::shared_ptr<Identifiable<ID_TYPE>> a, const std::shared_ptr<Identifiable<ID_TYPE>> b) {		
			return a->get_id() < b->get_id();
		}
	){}


	///<summary>
	/// �f�X�g���N�^
	///</summary>
	template <typename ID_TYPE, typename DATA_TYPE>
	IdentifiableCollection<ID_TYPE, DATA_TYPE>::~IdentifiableCollection()
	{
	}


	///<summary>
	/// �w�肵��ID�����v�f��Ǐo����p�Ŏ擾����D
	/// ������Ȃ��ꍇ��nullptr��Ԃ��D
	/// �\�[�g�ς݂̃R���N�V�����ɑ΂��Ă�2���T��
	/// �\�[�g����Ă��Ȃ��R���N�V�����ɑ΂��Ă͑O����S�T������D
	///</summary>
	template <typename ID_TYPE, typename DATA_TYPE>
	std::shared_ptr<DATA_TYPE const> IdentifiableCollection<ID_TYPE, DATA_TYPE>::read_by_id(const ID_TYPE& id) const
	{
		auto target_iter = find(std::make_shared<Identifiable<ID_TYPE>>(id));
		if (target_iter == end()) {
			return nullptr;
		}
		return std::dynamic_pointer_cast<DATA_TYPE const>(*target_iter);
	}

	///<summary>
	/// �w�肵��ID�����v�f��ύX�\�ȏ�ԂŎ擾����D
	/// ������Ȃ��ꍇ��nullptr��Ԃ��D
	/// �\�[�g�ς݂̃R���N�V�����ɑ΂��Ă�2���T��
	/// �\�[�g����Ă��Ȃ��R���N�V�����ɑ΂��Ă͑O����S�T������D
	///</summary>
	template <typename ID_TYPE, typename DATA_TYPE>
	std::shared_ptr<DATA_TYPE> IdentifiableCollection<ID_TYPE, DATA_TYPE>::get_by_id(const ID_TYPE& id)
	{
		auto target_iter = find(std::make_shared<Identifiable<ID_TYPE>>(id));
		if (target_iter == end()) {
			return nullptr;
		}
		return std::dynamic_pointer_cast<DATA_TYPE>(*target_iter);
	}
	
	///<summary>
	/// �w�肵��ID�����v�f��T�����C�폜����D
	/// �v�f�����݂��C�폜�ł����ꍇ��true���C����ȊO�̏ꍇ��false��Ԃ��D
	///</summary>
	template <typename ID_TYPE, typename DATA_TYPE>
	bool IdentifiableCollection<ID_TYPE, DATA_TYPE>::remove_by_id(const ID_TYPE &id)
	{
		auto target_iter =  find(std::make_shared<Identifiable<ID_TYPE>>(id));

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
	template <typename ID_TYPE, typename DATA_TYPE>
	std::unique_ptr<std::vector<ID_TYPE>> IdentifiableCollection<ID_TYPE, DATA_TYPE>::get_id_list() const
	{
		std::unique_ptr<std::vector<ID_TYPE>> id_list = std::make_unique<std::vector<ID_TYPE>>(size());
		int index = 0;
		for (std::set<std::shared_ptr<Identifiable<ID_TYPE>>>::const_iterator iter = begin(); iter != end(); iter++, index++) {
			id_list->at(index) = (*iter)->get_id();
		}
		return std::move(id_list);
	}

	///<summary>
	/// �w�肵��ID�����v�f���܂�ł��邩�𔻒肷��
	///</summary>
	template <typename ID_TYPE, typename DATA_TYPE>
	bool IdentifiableCollection<ID_TYPE, DATA_TYPE>::contains(const ID_TYPE& id) const
	{
		bool is_contained = find(std::make_shared<Identifiable<ID_TYPE>>(id)) != end();
		return is_contained;
	}


	///<summary>
	/// �����ID�����v�f���܂�ł��邩�𔻒肷��
	///</summary>
	template <typename ID_TYPE, typename DATA_TYPE>
	bool IdentifiableCollection<ID_TYPE, DATA_TYPE>::contains(const DATA_TYPE& id) const
	{
		return contains(id.get_id());
	}

	///<summary>
	/// �����ID�����v�f���܂�ł��邩�𔻒肷��
	///</summary>
	template <typename ID_TYPE, typename DATA_TYPE>
	bool IdentifiableCollection<ID_TYPE, DATA_TYPE>::contains(std::shared_ptr<DATA_TYPE const> id) const
	{
		return contains(id->get_id());
	}


	///<summary>
	/// �R���N�V�����̖����Ɏw�肵���v�f���i�[����
	/// ID���d������ꍇ��DuplicatedIdException���X���[����D
	/// max_size�𒴂���̈���w�肵���ꍇ��out_of_range���X���[����
	///</summary>
	template <typename ID_TYPE, typename DATA_TYPE>
	bool IdentifiableCollection<ID_TYPE, DATA_TYPE>::add(std::shared_ptr<DATA_TYPE> val)
	{
		//����ID�̏ꍇ�͒ǉ����Ȃ�
		ID_TYPE id = val->get_id();
		if (contains(val)) {
			throw DuplicatedIdException<ID_TYPE>();
			return false;
		}

		// ID�����̏ꍇ��v�f����max_size���傫���Ȃ�ꍇ��out_of_range���X���[����
		if (size() == max_size()) {
			throw std::out_of_range("The number of elements reached to the upper limitation");
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
	template <typename ID_TYPE, typename DATA_TYPE>
	bool IdentifiableCollection<ID_TYPE, DATA_TYPE>::add(DATA_TYPE val)
	{
		return add(std::make_shared<DATA_TYPE>(val));
	}


	///<summary>
	///�v�f����Ԃ�
	///</summary>
	template <typename ID_TYPE, typename DATA_TYPE>
	size_t IdentifiableCollection<ID_TYPE, DATA_TYPE>::size() const
	{
		return std::set<std::shared_ptr<Identifiable<ID_TYPE>>, std::function<bool(const std::shared_ptr<Identifiable<ID_TYPE>>, const std::shared_ptr<Identifiable<ID_TYPE>>)>>::size();
	}

	///<summary>
	/// �e�v�f�ɂ���execute_function�����s����
	/// ���g�̕ύX���e������̂Œ���
	///</summary>
	template <typename ID_TYPE, typename DATA_TYPE>
	void IdentifiableCollection<ID_TYPE, DATA_TYPE>::foreach(const std::function<void(std::shared_ptr<DATA_TYPE>)>& execute_function)
	{
		for (std::set<std::shared_ptr<Identifiable<ID_TYPE>>>::const_iterator iter =  begin(); iter != end(); iter++) {
			std::shared_ptr<DATA_TYPE> element = std::dynamic_pointer_cast<DATA_TYPE>(*iter);
			execute_function(element);
		}
	}

	///<summary>
	/// �e�v�f�ɂ���execute_function�����s����
	///</summary>
	template <typename ID_TYPE, typename DATA_TYPE>
	void IdentifiableCollection<ID_TYPE, DATA_TYPE>::foreach(const std::function<void(std::shared_ptr<DATA_TYPE const>)>& execute_function) const
	{
		for (std::set<std::shared_ptr<Identifiable<ID_TYPE> const>>::const_iterator iter = begin(); iter != end(); iter++) {
			std::shared_ptr<DATA_TYPE const> element = std::dynamic_pointer_cast<DATA_TYPE const>(*iter);
			execute_function(element);
		}
	}

}
