#include "stdafx.h"
#include "PauseMobileEntity.h"


namespace Entity
{

	///<summary>
	/// �R���X�g���N�^
	///</summary>
	template <typename POSITION_TYPE>
	PauseMobileEntity<POSITION_TYPE>::PauseMobileEntity(entity_id id, std::shared_ptr<Time::TimeSlotManager const> timeslot)
		: MobileEntity<POSITION_TYPE>(id, timeslot), pause_time_list(std::vector<time_t>(timeslot->phase_count(),0))
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
	/// ��~���Ԃ�set����
	///</summary>
	template <typename POSITION_TYPE>
	void PauseMobileEntity<POSITION_TYPE>::set_pause_time(int phase, time_t pause_time)
	{
		pause_time_list.at(phase) = pause_time;
	}


}
