#pragma once
#include "BasicMobileEntityState.h"
#include "LatLng.h"
#include "MobileEntity.h"

namespace Entity
{

	///<summary>
	/// ユーザの基本クラス
	/// デフォルトでユーザはID=0を持つ
	///</summary>
	template <typename STATE_TYPE>
	class BasicUser : public MobileEntity<Geography::LatLng, STATE_TYPE>
	{
	public:
		BasicUser(std::vector<std::shared_ptr<STATE_TYPE>> user_movement, entity_id id = 0U);
		~BasicUser();
	};
}

#include "BasicUser.hpp"


