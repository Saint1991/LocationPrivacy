#ifdef UTILS_EXPORTS
#define CROSS_INFO_API __declspec(dllexport)
#else
#define CROSS_INFO_API __declspec(dllimport)
#endif
#pragma once
#include "MobileEntity.h"

namespace Evaluate
{

	///<summary>
	/// Œğ·î•ñ‚ğŠi”[‚·‚éƒNƒ‰ƒX
	///</summary>
	struct CROSS_INFO_API CrossInfo
	{
	public:
		int phase;
		Entity::entity_id id;
		std::vector<Entity::entity_id> crossing_entities;

		CrossInfo();
		CrossInfo(int phase, Entity::entity_id id, const std::vector<Entity::entity_id>& crossing_entities);
		~CrossInfo();
	};
}


