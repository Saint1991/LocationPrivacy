#include "stdafx.h"
#include "DifferentMovementUser.h"

namespace Entity
{
	///<summary>
	/// コンストラクタ
	///</summary>
	template <typename POSITION_TYPE, typename TRAJECTORY_TYPE>
	DifferentMovementUser<POSITION_TYPE, TRAJECTORY_TYPE>::DifferentMovementUser(entity_id id, std::shared_ptr<Time::TimeSlotManager const> timeslot) 
		: RevisablePauseMobileEntity<POSITION_TYPE, TRAJECTORY_TYPE>(id, timeslot)
	{
		//real_user = std::make_shared<Entity::PauseMobileEntity<Geography::LatLng>>(0, timeslot);;
		//predicted_user = std::make_shared<Entity::PauseMobileEntity<Geography::LatLng>>(0, timeslot);
	}

	///<summary>
	/// コンストラクタ
	///</summary>
	template <typename POSITION_TYPE, typename TRAJECTORY_TYPE>
	DifferentMovementUser<POSITION_TYPE, TRAJECTORY_TYPE>::DifferentMovementUser(
		entity_id id, 
		std::shared_ptr<Time::TimeSlotManager const> timeslot,
		std::shared_ptr<PauseMobileEntity<POSITION_TYPE, TRAJECTORY_TYPE>> real_user, 
		std::shared_ptr<RevisablePauseMobileEntity<POSITION_TYPE, TRAJECTORY_TYPE>> predicted_user
	) : RevisablePauseMobileEntity<POSITION_TYPE, TRAJECTORY_TYPE>(id, timeslot), real_user(real_user), predicted_user(predicted_user)
	{
	}

	///<summary>
	/// デストラクタ
	///</summary>
	template <typename POSITION_TYPE, typename TRAJECTORY_TYPE>
	DifferentMovementUser<POSITION_TYPE, TRAJECTORY_TYPE>::~DifferentMovementUser()
	{

	}


	///<summary>
	/// real_userを取得する
	///</summary>
	template <typename POSITION_TYPE, typename TRAJECTORY_TYPE>
	std::shared_ptr<PauseMobileEntity<POSITION_TYPE, TRAJECTORY_TYPE>> DifferentMovementUser<POSITION_TYPE, TRAJECTORY_TYPE>::get_real_user()
	{
		return real_user;
	}
	///<summary>
	/// prediceted_userを取得する
	///</summary>
	template <typename POSITION_TYPE, typename TRAJECTORY_TYPE>
	std::shared_ptr<RevisablePauseMobileEntity<POSITION_TYPE, TRAJECTORY_TYPE>> DifferentMovementUser<POSITION_TYPE, TRAJECTORY_TYPE>::get_predicted_user()
	{
		return predicted_user;
	}
	
}
