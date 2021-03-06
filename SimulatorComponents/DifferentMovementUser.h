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
		std::shared_ptr<PauseMobileEntity<POSITION_TYPE, TRAJECTORY_TYPE>> real_user;
		std::shared_ptr<RevisablePauseMobileEntity<POSITION_TYPE, TRAJECTORY_TYPE>> predicted_user;
		
	public:
		DifferentMovementUser(entity_id id, std::shared_ptr<Time::TimeSlotManager const> timeslot);
		DifferentMovementUser(entity_id id, std::shared_ptr<Time::TimeSlotManager const> timeslot, std::shared_ptr<PauseMobileEntity<POSITION_TYPE, TRAJECTORY_TYPE>> real_user, std::shared_ptr<RevisablePauseMobileEntity<POSITION_TYPE, TRAJECTORY_TYPE>> predicted_user);
		~DifferentMovementUser();
		
		std::shared_ptr<PauseMobileEntity<POSITION_TYPE, TRAJECTORY_TYPE>> get_real_user();
		std::shared_ptr<RevisablePauseMobileEntity<POSITION_TYPE, TRAJECTORY_TYPE>> get_predicted_user();
	};

	//明示的特殊化
	template class DifferentMovementUser<Graph::Coordinate>;
	template class DifferentMovementUser<Geography::LatLng>;
}