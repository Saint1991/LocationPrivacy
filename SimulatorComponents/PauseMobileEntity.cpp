#include "stdafx.h"
#include "PauseMobileEntity.h"


namespace Entity
{

	///<summary>
	/// �R���X�g���N�^
	///</summary>
	template <typename POSITION_TYPE>
	PauseMobileEntity<POSITION_TYPE>::PauseMobileEntity(entity_id id, std::shared_ptr<Time::TimeSlotManager const> timeslot)
		: MobileEntity<POSITION_TYPE>(id, timeslot), pause_time_list(std::vector<time_t>(timeslot->phase_count(),0)), speed_list(std::vector<double>(timeslot->phase_count(), 0))//,max_pause_time(max_pause_time),min_pause_time(min_pause_time)
	{
	}

	///<summary>
	/// �f�X�g���N�^
	///</summary>
	template <typename POSITION_TYPE>
	PauseMobileEntity<POSITION_TYPE>::~PauseMobileEntity()
	{
	}


	///<summary>
	/// ��~���Ԃ����߂�
	///</summary>
	template <typename POSITION_TYPE>
	time_t PauseMobileEntity<POSITION_TYPE>::get_pause_time(int phase) const
	{
		return pause_time_list.at(phase);
	}

	///<summary>
	/// ����̒l���~���ԂƂ���set����
	///</summary>
	template <typename POSITION_TYPE>
	void PauseMobileEntity<POSITION_TYPE>::set_pause_time(int phase, time_t pause_time)
	{
		pause_time_list.at(phase) = pause_time;
	}

	/*
	///<summary>
	/// �����_���Ȓl���~���ԂƂ���set����
	///</summary>
	template <typename POSITION_TYPE>
	void PauseMobileEntity<POSITION_TYPE>::set_random_pause_time(int phase)
	{
		Math::Probability generator;
		double min = (double)requirement->min_pause_time;
		double max = (double)requirement->max_pause_time;
		double pause_time = generator.uniform_distribution(min, max);

		pause_time_list.at(phase) = (time_t)pause_time;
	}

	///<summary>
	/// �ړ����x�����߂�
	///</summary>
	template <typename POSITION_TYPE>
	double PauseMobileEntity<POSITION_TYPE>::get_speed(int phase) const
	{
		return speed_list.at(phase);
	}

	///<summary>
	/// ����̒l�̈ړ����x��set����
	///</summary>
	template <typename POSITION_TYPE>
	void PauseMobileEntity<POSITION_TYPE>::set_speed(int phase, double speed)
	{
		speed_list.at(phase) = speed;
	}

	

	///<summary>
	/// �����_���Ȓl�̈ړ����x��set����
	///</summary>
	template <typename POSITION_TYPE>
	void PauseMobileEntity<POSITION_TYPE>::set_random_speed(int phase)
	{
		Math::Probability generator;
		double min = requirement->average_speed_of_dummy - requirement->speed_range_of_dummy;
		double max = requirement->average_speed_of_dummy + requirement->speed_range_of_dummy;
		double dummy_speed = generator.uniform_distribution(min, max);

		speed_list.at(phase) = dummy_speed;
	}
	*/

}
