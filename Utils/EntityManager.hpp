
namespace Entity
{


	///<summary>
	/// �R���X�g���N�^
	///</summary>
	template <typename DUMMY, typename USER, typename POSITION_TYPE>
	EntityManager<DUMMY, USER, POSITION_TYPE>::EntityManager(std::shared_ptr<USER> user, int num_of_dummy, std::shared_ptr<Time::TimeSlotManager> timeslot)
		: timeslot(timeslot), dummies(std::make_shared<std::vector<std::shared_ptr<DUMMY>>>(num_of_dummy)), user(user)
	{
		for (int id = 1; id <= num_of_dummy; id++) {
			dummies->at(id - 1) = std::make_shared<DUMMY>(id, timeslot);
		}
	}


	///<summary>
	/// �f�X�g���N�^
	///</summary>
	template <typename DUMMY, typename USER, typename POSITION_TYPE>
	EntityManager<DUMMY, USER, POSITION_TYPE>::~EntityManager()
	{
	}


	///<summary>
	/// �_�~�[�𐶐�����
	///</summary>
	template <typename DUMMY, typename USER, typename POSITION_TYPE>
	entity_id EntityManager<DUMMY, USER, POSITION_TYPE>::create_dummy()
	{
		entity_id new_id = dummies->size() + 1;
		dummies->push_back(std::make_shared<DUMMY>(new_id, timeslot));
		return new_id;
	}

	///<summary>
	/// ���[�U��ǂݐ�p�Ŏ擾
	///</summary>
	template <typename DUMMY, typename USER, typename POSITION_TYPE>
	std::shared_ptr<USER> EntityManager<DUMMY, USER, POSITION_TYPE>::get_user()
	{
		return user;
	}


	///<summary>
	/// �w�肵��ID�̃_�~�[���擾
	/// ���݂��Ȃ��ꍇ��nullptr���Ԃ�
	///</summary>
	template <typename DUMMY, typename USER, typename POSITION_TYPE>
	std::shared_ptr<DUMMY> EntityManager<DUMMY, USER, POSITION_TYPE>::get_dummy_by_id(entity_id id)
	{
		std::shared_ptr<DUMMY> dummy = dummies->at(id - 1);
		if (dummy->get_id() == id) {
			return dummy;
		}
		return nullptr;
	}


	///<summary>
	/// �����ɍ����_�~�[���擾
	///</summary>
	template <typename DUMMY, typename USER, typename POSITION_TYPE>
	std::shared_ptr<DUMMY> EntityManager<DUMMY, USER, POSITION_TYPE>::find_dummy_if(const std::function<bool(std::shared_ptr<DUMMY const>)>& compare)
	{
		for (std::vector<std::shared_ptr<DUMMY>>::const_iterator iter = dummies->begin(); iter != dummies->end(); iter++) {
			if (compare(*iter)) return *iter;
		}
		return nullptr;
	}


	///<summary>
	/// �����ɍ����_�~�[��S�Ď擾����
	///</summary>
	template <typename DUMMY, typename USER, typename POSITION_TYPE>
	std::vector<std::shared_ptr<DUMMY>> EntityManager<DUMMY, USER, POSITION_TYPE>::find_all_dummies_if(const std::function<bool(std::shared_ptr<DUMMY const>)>& compare)
	{
		std::vector<std::shared_ptr<DUMMY>> ret;
		for (std::vector<std::shared_ptr<DUMMY>>::const_iterator iter = dummies->begin(); iter != dummies->end(); iter++) {
			if (compare(*iter)) ret.push_back(*iter);
		}
		return ret;
	}


	///<summary>
	/// �w�肵��ID�̃_�~�[���擾
	/// ���݂��Ȃ��ꍇ��nullptr���Ԃ�
	///</summary>
	template <typename DUMMY, typename USER, typename POSITION_TYPE>
	std::shared_ptr<DUMMY const> EntityManager<DUMMY, USER, POSITION_TYPE>::read_dummy_by_id(entity_id id) const
	{
		std::shared_ptr<DUMMY const> dummy = dummies->at(id - 1);
		if (dummy->get_id() == id) {
			return dummy;
		}
		return nullptr;
	}

	
	///<summary>
	/// �����񐔂̏��������ɃG���e�B�e�BID���i�[�������X�g��Ԃ��܂�
	/// �������̃_�~�[��������0�Ƃ��Ċ܂܂��̂Œ���
	///</summary>
	template <typename DUMMY, typename USER, typename POSITION_TYPE>
	std::list<std::pair<entity_id, int>> EntityManager<DUMMY, USER, POSITION_TYPE>::get_entity_id_list_order_by_cross_count() const
	{
		std::list<std::pair<entity_id, int>> ret;
		for (std::vector<std::shared_ptr<DUMMY>>::const_iterator iter = dummies->begin(); iter != dummies->end(); iter++) {
			if (*iter == nullptr) continue;
			entity_id id = (*iter)->get_id();
			int cross_count = (*iter)->get_cross_count();
			ret.push_back(std::make_pair(id, cross_count));
		}
		ret.push_back(std::make_pair(0U, user->get_cross_count()));
		ret.sort([](const std::pair<entity_id, int>& pair1, const std::pair<entity_id, int>& pair2) {
			return pair1.second < pair2.second;
		});
		return ret;
	}

	///<summary>
	/// �_�~�[�̑������擾
	///</summary>
	template <typename DUMMY, typename USER, typename POSITION_TYPE>
	size_t EntityManager<DUMMY, USER, POSITION_TYPE>::get_dummy_count() const
	{
		return dummies->size();
	}

	///<summary>
	/// �e�_�~�[�ɂ���execute_function�����s
	///</summary>
	template <typename DUMMY, typename USER, typename POSITION_TYPE>
	void EntityManager<DUMMY, USER, POSITION_TYPE>::for_each_dummy(const std::function<void(entity_id, std::shared_ptr<DUMMY>)>& execute_function)
	{
		for (std::vector<std::shared_ptr<DUMMY>>::iterator iter = dummies->begin(); iter != dummies->end(); iter++) {
			if (*iter != nullptr) execute_function((*iter)->get_id(), *iter);
		}
	}

	///<summary>
	/// �e�_�~�[�ɂ���execute_function�����s
	///</summary>
	template <typename DUMMY, typename USER, typename POSITION_TYPE>
	void EntityManager<DUMMY, USER, POSITION_TYPE>::for_each_dummy(const std::function<void(entity_id, std::shared_ptr<DUMMY const>)>& execute_function) const
	{
		for (std::vector<std::shared_ptr<DUMMY>>::const_iterator iter = dummies->begin(); iter != dummies->end(); iter++) {
			if (*iter != nullptr) {
				std::shared_ptr<DUMMY const> dummy = *iter;
				execute_function(dummy->get_id(), dummy);
			}
		}
	}

	///<summary>
	/// �w�肵��Phase�ɂ�����ʒu�m��ς݃G���e�B�e�B�̕��ψʒu���擾����
	///</summary>
	template <typename DUMMY, typename USER, typename POSITION_TYPE>
	std::shared_ptr<POSITION_TYPE const> EntityManager<DUMMY, USER, POSITION_TYPE>::get_average_position_of_phase(int phase) const
	{
		double x = 0.0;
		double y = 0.0;

		std::shared_ptr<POSITION_TYPE const> position = user->read_position_of_phase(phase);
		x += position->x();
		y += position->y();
		int fixed_count = 1;

		for (std::vector<std::shared_ptr<DUMMY>>::const_iterator iter = dummies->begin(); iter != dummies->end(); iter++) {
			position = (*iter)->read_position_of_phase(phase);
			if (position != nullptr) {
				fixed_count++;
				x += position->x();
				y += position->y();
			}
		}

		x /= fixed_count;
		y /= fixed_count;
		return std::move(std::make_shared<POSITION_TYPE const>(x, y));
	}


	///<summary>
	/// ����time�ɂ����Ĉʒu���m�肵�Ă���G���e�B�e�B�̕��ψʒu���擾���܂�
	/// �s���Ȏ����̏ꍇ��nullptr��Ԃ��܂��D
	///</summary>
	template <typename DUMMY, typename USER, typename POSITION_TYPE>
	std::shared_ptr<POSITION_TYPE const> EntityManager<DUMMY, USER, POSITION_TYPE>::get_average_position_at(time_t time) const
	{
		int phase = timeslot->find_phase_of_time(time);
		if (phase != INVALID) {
			return get_average_position_of_phase(phase);
		}
		return nullptr;
	}

	
	///<summary>
	/// �ephase�ɂ�����Z���ɑ��݂��郆�[�U����ѐ����ς݃_�~�[�̐����v�Z���܂�
	///</summary>
	template <typename DUMMY, typename USER, typename POSITION_TYPE>
	int EntityManager<DUMMY, USER, POSITION_TYPE>::get_entity_count_within_boundary(int phase, const Graph::Rectangle<POSITION_TYPE>& boundary) const 
	{
		
		int counter = 0;

		//���[�U���̈���ɑ��݂��邩�m�F
		const std::shared_ptr<POSITION_TYPE const> user_position = user->read_position_of_phase(phase);
		if (user_position != nullptr && boundary.contains(*user_position)) counter++;

		//�̈���̃_�~�[���̊m�F
		for_each_dummy([&boundary, &counter, phase](entity_id id, std::shared_ptr<DUMMY const> dummy) {
			const std::shared_ptr<POSITION_TYPE const> dummy_position = dummy->read_position_of_phase(phase);
			if (dummy_position != nullptr && boundary.contains(*dummy_position)) counter++;
		});

		return counter;
	}

	///<summary>
	/// �ephase�ɂ�����Z���ɑ��݂��郆�[�U����ѐ����ς݃_�~�[�̐����v�Z���܂�
	///</summary>
	template <typename DUMMY, typename USER, typename POSITION_TYPE>
	int EntityManager<DUMMY, USER, POSITION_TYPE>::get_entity_count_within_boundary(int phase, double top, double left, double bottom, double right) const
	{
		return get_entity_count_within_boundary(phase, Graph::Rectangle<POSITION_TYPE>(top, left, bottom, right));
	}

	///<summary>
	/// �S�ẴG���e�B�e�B�̌����񐔂̍��v���v�Z���܂�
	///</summary>
	template <typename DUMMY, typename USER, typename POSITION_TYPE>
	int EntityManager<DUMMY, USER, POSITION_TYPE>::get_all_entities_total_crossing_count() const
	{
		int counter = 0;
		
		counter += user->get_cross_count();

		for_each_dummy([&counter](Entity::entity_id id, std::shared_ptr<DUMMY const> dummy) {
			counter += dummy->get_cross_count();
		});
		
		return counter;
	}
	
}


