#ifdef SIMULATORCOMPONENTS_EXPORTS
#define DIFFERENT_MOVEMENT_USER_API __declspec(dllexport)
#else
#define DIFFERENT_MOVEMENT_USER_API __declspec(dllimport)
#endif

#pragma once
#include "RevisablePauseMobileEntity.h"

namespace Entity
{
	template <typename POSITION_TYPE = Geography::LatLng, typename TRAJECTORY_TYPE = Graph::RevisableTrajectory<POSITION_TYPE>>
	class DIFFERENT_MOVEMENT_USER_API DifferentMovementUser : public RevisablePauseMobileEntity<POSITION_TYPE, TRAJECTORY_TYPE>
	{
	protected:
		//メンバ変数
		//実際のユーザ用
		std::shared_ptr<TRAJECTORY_TYPE> different_trajectory;
		std::shared_ptr<std::vector<bool>> different_cross_flg;
		int different_total_cross_count;
		
	public:
		DifferentMovementUser(entity_id id, std::shared_ptr<Time::TimeSlotManager const> timeslot);
		~DifferentMovementUser();
		
	};

	//明示的特殊化
	template class DifferentMovementUser<Graph::Coordinate>;
	template class DifferentMovementUser<Geography::LatLng>;
}