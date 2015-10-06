#include "stdafx.h"
#include "SemanticEntityManager.h"

namespace Entity
{

	///<summary>
	/// コンストラクタ
	///</summary>
	template <typename POSITION_TYPE>
	SemanticEntityManager<POSITION_TYPE>::SemanticEntityManager(std::shared_ptr<User::BasicUser<POSITION_TYPE>> user, int num_of_dummy, std::shared_ptr<Time::TimeSlotManager> timeslot) 
		: EntityManager<Dummy<POSITION_TYPE>, User::BasicUser<POSITION_TYPE>, POSITION_TYPE>(user, num_of_dummy, timeslot)
	{
	}


	///<summary>
	/// デストラクタ
	///</summary>
	template <typename POSITION_TYPE>
	SemanticEntityManager<POSITION_TYPE>::~SemanticEntityManager()
	{
	}

}

