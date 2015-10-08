
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
	/// 2�G���e�B�e�B�Ԃ̋��L�n�_��ݒ肷��
	/// ������ĂԂ��тɊe�_�~�[�̌����񐔂�+1�����̂Œ���
	/// �����ݒ�ɕ����_�̐ݒ肪�K�v�ȏꍇ�͈�x����������Ăяo���āC���Ƃ�set_position�̕�����������
	/// �_�~�[ID��ID=1����_�~�[�����̘A�ԂɂȂ��Ă���̂�z�肵�Ă���
	///</summary>
	template <typename DUMMY, typename USER, typename POSITION_TYPE>
	void EntityManager<DUMMY, USER, POSITION_TYPE>::set_crossing_point_of_phase(entity_id id1, entity_id id2, POSITION_TYPE position1, POSITION_TYPE position2, int phase)
	{

		if (id1 < 1 || id2 < 1 || dummies->size() <= id1 || dummies->size() <= id2) {
			throw std::invalid_argument("INVALID dummy ID");
			return;
		}

		std::shared_ptr<DUMMY> dummy1 = dummies->at(id1 - 1);
		std::shared_ptr<DUMMY> dummy2 = dummies->at(id2 - 1);
		if (dummy1 != nullptr && dummy2 != nullptr && dummy1->get_id() == id1 && dummy2->get_id() == id2) {
			dummy1->set_crossing_position_of_phase(phase, position1);
			dummy2->set_crossing_position_of_phase(phase, position2);
		}
	}

	///<summary>
	/// 2�G���e�B�e�B�Ԃ̋��L�n�_��ݒ肷��
	/// ������ĂԂ��тɊe�_�~�[�̌����񐔂�+1�����̂Œ���
	/// �����ݒ�ɕ����_�̐ݒ肪�K�v�ȏꍇ�͈�x����������Ăяo���āC���Ƃ�set_position�̕�����������
	/// �_�~�[ID��ID=1����_�~�[�����̘A�ԂɂȂ��Ă���̂�z�肵�Ă���
	///</summary>
	template <typename DUMMY, typename USER, typename POSITION_TYPE>
	void EntityManager<DUMMY, USER, POSITION_TYPE>::set_crossing_point_at(entity_id id1, entity_id id2, POSITION_TYPE position1, POSITION_TYPE position2, time_t time)
	{
		int phase = timeslot->find_phase_of_time(time);
		if (phase != INVALID) {
			set_crossing_point_of_phase(id1, id2, position1, position2, phase);
		}
	}


	///<summary>
	/// �_�~�[�ɒn�_�̐ݒ������
	///</summary>
	template <typename DUMMY, typename USER, typename POSITION_TYPE>
	void EntityManager<DUMMY, USER, POSITION_TYPE>::set_point_of_phase(entity_id id, POSITION_TYPE position, int phase)
	{
		if (id < 1 || dummies->size() <= id) {
			throw std::invalid_argument("INVALID dummy ID");
			return;
		}

		std::shared_ptr<DUMMY> dummy = dummies->at(id - 1);
		if (dummy != nullptr && dummy->get_id() == id) {
			dummy->set_position_of_phase(phase, position);
		}
	}

	///<summary>
	/// �_�~�[�ɒn�_�̐ݒ������
	///</summary>
	template <typename DUMMY, typename USER, typename POSITION_TYPE>
	void EntityManager<DUMMY, USER, POSITION_TYPE>::set_point_at(entity_id id, POSITION_TYPE type, time_t time)
	{
		int phase = timeslot->find_phase_of_time(time);
		if (phase != INVALID) {
			set_point_of_phase(id, type, phase);
		}
	}


	///<summary>
	/// ���[�U��ǂݐ�p�Ŏ擾
	///</summary>
	template <typename DUMMY, typename USER, typename POSITION_TYPE>
	std::shared_ptr<USER const> EntityManager<DUMMY, USER, POSITION_TYPE>::get_user() const
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
	/// �����񐔂��ŏ��̃G���e�B�e�B��ID���擾
	/// 0�̏ꍇ�̓��[�U�C1�ȏ�Ȃ�_�~�[
	///</summary>
	template <typename DUMMY, typename USER, typename POSITION_TYPE>
	entity_id EntityManager<DUMMY, USER, POSITION_TYPE>::get_min_cross_entity_id() const
	{
		entity_id ret = user->get_id();
		int min = user->get_cross_count();
		for (std::vector<std::shared_ptr<DUMMY>>::const_iterator iter = dummies->begin(); iter != dummies->end(); iter++) {
			if ((*iter)->get_cross_count() < min) {
				ret = (*iter)->get_id();
			}
		}
		return ret;
	}


	///<summary>
	/// �w�肵��Phase�ɂ�����ʒu�m��ς݃G���e�B�e�B�̕��ψʒu���擾����
	///</summary>
	template <typename DUMMY, typename USER, typename POSITION_TYPE>
	std::shared_ptr<POSITION_TYPE const> EntityManager<DUMMY, USER, POSITION_TYPE>::get_average_position_of_phase(int phase)
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
	std::shared_ptr<POSITION_TYPE const> EntityManager<DUMMY, USER, POSITION_TYPE>::get_average_position_at(time_t time)
	{
		int phase = timeslot->find_phase_of_time(time);
		if (phase != INVALID) {
			return get_average_position_of_phase(phase);
		}
		return nullptr;
	}
}


