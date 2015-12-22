#include "stdafx.h"
#include "DifferentMovementUser.h"

namespace Entity
{
	///<summary>
	/// �R���X�g���N�^
	///</summary>
	template <typename POSITION_TYPE, typename TRAJECTORY_TYPE>
	DifferentMovementUser<POSITION_TYPE, TRAJECTORY_TYPE>::DifferentMovementUser(entity_id id, std::shared_ptr<Time::TimeSlotManager const> timeslot)
		: RevisablePauseMobileEntity<POSITION_TYPE, TRAJECTORY_TYPE>(id, timeslot)
	{
		real_user = std::make_shared<Entity::PauseMobileEntity<Geography::LatLng>>(0, timeslot);;
		predicted_user = std::make_shared<Entity::PauseMobileEntity<Geography::LatLng>>(0, timeslot);;
	}

	///<summary>
	/// �f�X�g���N�^
	///</summary>
	template <typename POSITION_TYPE, typename TRAJECTORY_TYPE>
	DifferentMovementUser<POSITION_TYPE, TRAJECTORY_TYPE>::~DifferentMovementUser()
	{

	}


	///<summary>
	/// real_user���擾����
	///</summary>
	template <typename POSITION_TYPE, typename TRAJECTORY_TYPE>
	std::shared_ptr<PauseMobileEntity<>> DifferentMovementUser<POSITION_TYPE, TRAJECTORY_TYPE>::get_real_user()
	{
		return real_user;
	}
	///<summary>
	/// prediceted_user���擾����
	///</summary>
	template <typename POSITION_TYPE, typename TRAJECTORY_TYPE>
	std::shared_ptr<PauseMobileEntity<>> DifferentMovementUser<POSITION_TYPE, TRAJECTORY_TYPE>::get_predicted_user()
	{
		return predicted_user;
	}
	
}
