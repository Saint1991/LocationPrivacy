#include "stdafx.h"
#include "CrossInfo.h"

namespace Evaluate
{

	///<summary>
	/// コンストラクタ
	///</summary>
	CrossInfo::CrossInfo()
	{
	}


	///<summary>
	/// コンストラクタ
	///</summary>
	CrossInfo::CrossInfo(int phase, const std::vector<Entity::entity_id>& crossing_entities) : phase(phase), crossing_entities(crossing_entities)
	{
	}


	///<summary>
	/// デストラクタ
	///</summary>
	CrossInfo::~CrossInfo()
	{
	}

}

