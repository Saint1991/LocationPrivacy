#include "stdafx.h"
#include "MobileEntity.h"

namespace Entity
{

	///<summary>
	/// �R���X�g���N�^
	///</summary>
	template <typename POSITION_TYPE>
	MobileEntity<POSITION_TYPE>::MobileEntity(entity_id id, std::shared_ptr<Time::TimeSlotManager const> timeslot)
		: Identifiable<entity_id>(id), timeslot(timeslot), positions(std::make_shared<std::vector<std::shared_ptr<POSITION_TYPE>>>(timeslot->phase_count()))
	{
	}


	///<summary>
	/// �f�X�g���N�^
	///</summary>
	template <typename POSITION_TYPE>
	MobileEntity<POSITION_TYPE>::~MobileEntity()
	{
	}


	///<summary>
	/// �w�肵��Phase�ɂ�����ʒu��ݒ肷��
	///</summary>
	template <typename POSITION_TYPE>
	void MobileEntity<POSITION_TYPE>::set_position_of_phase(int phase, POSITION_TYPE position)
	{
		positions->at(phase) = std::make_shared<POSITION_TYPE>(position);
	}

	///<summary>
	/// �^����ꂽstate��ۑ�����
	///</summary>
	template <typename POSITION_TYPE>
	void MobileEntity<POSITION_TYPE>::set_position_at(time_t time, POSITION_TYPE position)
	{
		int phase = timeslot->find_phase_of_time(time);
		set_position_of_phase(phase, position);
	}

	///<summary>
	/// phase���̈ʒu���w�肵�܂�
	/// ���݂��Ȃ��ꍇ��nullptr���Ԃ�܂�
	///</summary>
	template <typename POSITION_TYPE>
	const std::shared_ptr<POSITION_TYPE const> MobileEntity<POSITION_TYPE>::read_position_of_phase(int phase) const
	{
		if (positions->size() == 0) return nullptr;
		return positions->at(phase);
	}


	///<summary>
	/// �w�肵�������ɂ�����ʒu��Ԃ�
	/// �Y������G���g�����Ȃ��ꍇ��nullptr��Ԃ�
	///</summary>
	template <typename POSITION_TYPE>
	const std::shared_ptr<POSITION_TYPE const> MobileEntity<POSITION_TYPE>::read_position_at(time_t time) const
	{
		int phase = timeslot->find_phase_of_time(time);
		if (phase == INVALID) return nullptr;
		return read_position_of_phase(phase);
	}
	
}

