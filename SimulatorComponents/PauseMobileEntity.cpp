#include "stdafx.h"
#include "PauseMobileEntity.h"


namespace Entity
{

	///<summary>
	/// コンストラクタ
	///</summary>
	template <typename POSITION_TYPE>
	PauseMobileEntity<POSITION_TYPE>::PauseMobileEntity(entity_id id, std::shared_ptr<Time::TimeSlotManager const> timeslot)
		: MobileEntity<POSITION_TYPE>(id, timeslot), pause_time_list(std::vector<time_t>(timeslot->phase_count(),0))
	{
	}

	///<summary>
	/// デストラクタ
	///</summary>
	template <typename POSITION_TYPE>
	PauseMobileEntity<POSITION_TYPE>::~PauseMobileEntity()
	{
	}


	///<summary>
	/// 停止時間を求める
	///</summary>
	template <typename POSITION_TYPE>
	time_t PauseMobileEntity<POSITION_TYPE>::get_pause_time(int phase) const
	{
		return pause_time_list.at(phase);
	}

	///<summary>
	/// 停止時間をsetする
	///</summary>
	template <typename POSITION_TYPE>
	void PauseMobileEntity<POSITION_TYPE>::set_pause_time(int phase, time_t pause_time)
	{
		pause_time_list.at(phase) = pause_time;
	}


}
