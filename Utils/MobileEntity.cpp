#include "stdafx.h"
#include "MobileEntity.h"

namespace Entity
{

	///<summary>
	/// �R���X�g���N�^
	///</summary>
	template <typename POSITION_TYPE>
	MobileEntity<POSITION_TYPE>::MobileEntity(entity_id id, std::shared_ptr<Time::TimeSlotManager const> timeslot)
		: Identifiable<entity_id>(id), timeslot(timeslot), positions(std::make_shared<std::vector<std::shared_ptr<POSITION_TYPE>>>(timeslot->phase_count())), cross_flg(std::make_shared<std::vector<bool>>(timeslot->phase_count(), false)), total_cross_count(0)
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
	/// ����t�ɂ�����ʒu��ݒ肷��
	///</summary>
	template <typename POSITION_TYPE>
	void MobileEntity<POSITION_TYPE>::set_position_at(time_t time, POSITION_TYPE position)
	{
		int phase = timeslot->find_phase_of_time(time);
		set_position_of_phase(phase, position);
	}


	///<summary>
	/// ���L�n�_�̐ݒ������
	///</summary>
	template <typename POSITION_TYPE>
	void MobileEntity<POSITION_TYPE>::set_crossing_position_of_phase(int phase, POSITION_TYPE position)
	{
		cross_flg->at(phase) = true;
		total_cross_count++;
		set_position_of_phase(phase, position);
	}

	///<summary>
	/// ���L�n�_�̐ݒ������
	///</summary>
	template <typename POSITION_TYPE>
	void MobileEntity<POSITION_TYPE>::set_crossing_position_at(time_t time, POSITION_TYPE position)
	{
		int phase = timeslot->find_phase_of_time(time);
		set_crossing_position_of_phase(phase, position);
	}


	///<summary>
	/// ���v�����񐔂��擾����
	///</summary>
	template <typename POSITION_TYPE>
	int MobileEntity<POSITION_TYPE>::get_cross_count() const
	{
		return total_cross_count;
	}


	///<summary>
	/// �w�莞���Ɍ������ݒ�ς��ǂ����`�F�b�N����
	///</summary>
	template <typename POSITION_TYPE>
	bool MobileEntity<POSITION_TYPE>::is_cross_set_at_phase(int phase) const
	{
		return cross_flg->at(phase);
	}


	///<summary>
	/// �w�莞���Ɍ������ݒ�ς��ǂ����`�F�b�N����
	///</summary>
	template <typename POSITION_TYPE>
	bool MobileEntity<POSITION_TYPE>::is_cross_set_at(time_t time) const
	{
		int phase = timeslot->find_phase_of_time(time);
		return cross_flg->at(phase);
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

