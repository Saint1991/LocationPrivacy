#include "stdafx.h"
#include "RevisablePauseMobileEntity.h"

namespace Entity
{
	///<summary>
	/// コンストラクタ
	///</summary>
	template <typename POSITION_TYPE, typename TRAJECTORY_TYPE>
	RevisablePauseMobileEntity<POSITION_TYPE, TRAJECTORY_TYPE>::RevisablePauseMobileEntity(entity_id id, std::shared_ptr<Time::TimeSlotManager const> timeslot)
		: PauseMobileEntity<POSITION_TYPE, TRAJECTORY_TYPE>(id, timeslot)
	{
	}

	///<summary>
	/// デストラクタ
	///</summary>
	template <typename POSITION_TYPE, typename TRAJECTORY_TYPE>
	RevisablePauseMobileEntity<POSITION_TYPE, TRAJECTORY_TYPE>::~RevisablePauseMobileEntity()
	{
	}

	///<summary>
	/// 
	///</summary>
	template <typename POSITION_TYPE, typename TRAJECTORY_TYPE>
	void RevisablePauseMobileEntity<POSITION_TYPE, TRAJECTORY_TYPE>::revise_rest_pause_time(int phase_id, double new_pause_time)
	{
		rest_pause_time_list.at(phase_id) += new_pause_time;
	}

	///<summary>
	/// 
	///</summary>
	template <typename POSITION_TYPE, typename TRAJECTORY_TYPE>
	void RevisablePauseMobileEntity<POSITION_TYPE, TRAJECTORY_TYPE>::revise_pause_time()
	{
	}
}