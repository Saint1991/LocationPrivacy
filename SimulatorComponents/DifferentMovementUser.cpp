#include "stdafx.h"
#include "DifferentMovementUser.h"

namespace Entity
{
	template <typename POSITION_TYPE, typename TRAJECTORY_TYPE>
	DifferentMovementUser<POSITION_TYPE, TRAJECTORY_TYPE>::DifferentMovementUser(entity_id id, std::shared_ptr<Time::TimeSlotManager const> timeslot)
		: RevisablePauseMobileEntity<POSITION_TYPE, TRAJECTORY_TYPE>(id, timeslot)
	{

	}

	template <typename POSITION_TYPE, typename TRAJECTORY_TYPE>
	DifferentMovementUser<POSITION_TYPE, TRAJECTORY_TYPE>::~DifferentMovementUser()
	{

	}
}
