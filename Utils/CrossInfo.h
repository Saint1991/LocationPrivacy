#pragma once
#include "MobileEntity.h"

namespace Evaluate
{

	///<summary>
	/// ���������i�[����N���X
	///</summary>
	struct CrossInfo
	{
	public:
		int phase;
		std::vector<Entity::entity_id> crossing_entities;

		CrossInfo();
		~CrossInfo();
	};
}


