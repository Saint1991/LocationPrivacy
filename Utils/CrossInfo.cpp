#include "stdafx.h"
#include "CrossInfo.h"

namespace Evaluate
{

	///<summary>
	/// �R���X�g���N�^
	///</summary>
	CrossInfo::CrossInfo()
	{
	}


	///<summary>
	/// �R���X�g���N�^
	///</summary>
	CrossInfo::CrossInfo(int phase, const std::vector<Entity::entity_id>& crossing_entities) : phase(phase), crossing_entities(crossing_entities)
	{
	}


	///<summary>
	/// �f�X�g���N�^
	///</summary>
	CrossInfo::~CrossInfo()
	{
	}

}

