#pragma once
#include "MobileEntity.h"

namespace Evaluate
{

	///<summary>
	/// Œğ·î•ñ‚ğŠi”[‚·‚éƒNƒ‰ƒX
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


