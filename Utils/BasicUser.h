#pragma once
#include "BasicMobileEntityState.h"
#include "LatLng.h"
#include "MobileEntity.h"

namespace Entity
{

	///<summary>
	/// ���[�U�̊�{�N���X
	/// �f�t�H���g�Ń��[�U��ID=0������
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


