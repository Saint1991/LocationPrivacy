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
	CrossInfo::CrossInfo(int phase, Entity::entity_id id, const std::vector<Entity::entity_id>& crossing_entities) : phase(phase), id(id), crossing_entities(crossing_entities)
	{
	}


	///<summary>
	/// �f�X�g���N�^
	///</summary>
	CrossInfo::~CrossInfo()
	{
	}

}

