#include "stdafx.h"
#include "SemanticEntityManager.h"

namespace Entity
{

	///<summary>
	/// �R���X�g���N�^
	///</summary>
	template <typename POSITION_TYPE>
	SemanticEntityManager<POSITION_TYPE>::SemanticEntityManager(std::shared_ptr<User::BasicUser<POSITION_TYPE>> user, int num_of_dummy, std::shared_ptr<Time::TimeSlotManager> timeslot) 
		: EntityManager<Dummy<POSITION_TYPE>, User::BasicUser<POSITION_TYPE>, POSITION_TYPE>(user, num_of_dummy, timeslot)
	{
	}


	///<summary>
	/// �f�X�g���N�^
	///</summary>
	template <typename POSITION_TYPE>
	SemanticEntityManager<POSITION_TYPE>::~SemanticEntityManager()
	{
	}


	///<summary>
	/// �w�肵��Phase�̃J�e�S����ݒ肷��
	/// crossing_point�ݒ肩�J�e�S����ݒ肵�����ꍇ��set_crossing_point_at�Ƃ�����ĂԕK�v������
	/// crossing_point�ȊO�̏ꍇ��set_state�n���g���������������悢
	///</summary>
	template <typename POSITION_TYPE>
	void SemanticEntityManager<POSITION_TYPE>::set_category_of_phase(entity_id id, const category_id& category, int phase)
	{
		if (id < 1 || dummies->size() <= id) {
			throw std::invalid_argument("INVALID dummy ID");
			return;
		}

		std::shared_ptr<Dummy<POSITION_TYPE>> dummy = dummies->at(id - 1);
		if (dummy != nullptr && dummy->get_id() == id) {
			dummy->set_category_of_phase(phase, category);
		}
	}


	///<summary>
	/// �w�肵�������̃J�e�S����ݒ肷��
	/// crossing_point�ݒ肩�J�e�S����ݒ肵�����ꍇ��set_crossing_point_at�Ƃ�����ĂԕK�v������
	/// crossing_point�ȊO�̏ꍇ��set_state�n���g���������������悢
	///</summary>
	template <typename POSITION_TYPE>
	void SemanticEntityManager<POSITION_TYPE>::set_category_at(entity_id id, const category_id& category, time_t time)
	{
		int phase = timeslot->find_phase_of_time(time);
		if (phase != INVALID) {
			set_category_of_phase(id, category, phase);
		}
	}


	///<summary>
	/// �w�肵��phase�ɂ�����ʒu�ƃJ�e�S����ݒ肷��
	///</summary>
	template <typename POSITION_TYPE>
	void SemanticEntityManager<POSITION_TYPE>::set_state_of_phase(entity_id id, const POSITION_TYPE& position, const category_id& category, int phase)
	{
		if (id < 1 || dummies->size() <= id) {
			throw std::invalid_argument("INVALID dummy ID");
			return;
		}

		std::shared_ptr<Dummy<POSITION_TYPE>> dummy = dummies->at(id - 1);
		if (dummy != nullptr && dummy->get_id() == id) {
			dummy->set_position_of_phase(phase, position);
			dummy->set_category_of_phase(phase, category);
		}
	}

	///<summary>
	/// �w�肵�������ɂ�����ʒu�ƃJ�e�S����ݒ肷��
	///</summary>
	template <typename POSITION_TYPE>
	void SemanticEntityManager<POSITION_TYPE>::set_state_at(entity_id id, const POSITION_TYPE& position, const category_id& category, time_t time)
	{
		int phase = timeslot->find_phase_of_time(time);
		if (phase != INVALID) {
			set_state_of_phase(id, position, category, time);
		}
	}
}

