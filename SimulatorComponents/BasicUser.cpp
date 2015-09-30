#include "stdafx.h"
#include "BasicUser.h"

namespace User
{

	///<summary>
	/// コンストラクタ
	///</summary>
	template <typename POSITION_TYPE>
	BasicUser<POSITION_TYPE>::BasicUser(std::shared_ptr<Time::TimeSlotManager const> timeslot, std::shared_ptr<PreferenceTree> preference_tree, Entity::entity_id id)
		: Entity::Dummy<POSITION_TYPE>(id, timeslot), preference(preference_tree)
	{
	}


	///<summary>
	/// デストラクタ
	///</summary>
	template <typename POSITION_TYPE>
	BasicUser<POSITION_TYPE>::~BasicUser()
	{
	}
}

