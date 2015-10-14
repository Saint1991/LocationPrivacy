#include "stdafx.h"
#include "PauseMobileEntity.h"


namespace Entity
{

	///<summary>
	/// コンストラクタ
	///</summary>
	template <typename POSITION_TYPE>
	PauseMobileEntity<POSITION_TYPE>::PauseMobileEntity(entity_id id, std::shared_ptr<Time::TimeSlotManager const> timeslot)
		: MobileEntity<POSITION_TYPE>(id, timeslot)
	{
	}

	///<summary>
	/// デストラクタ
	///</summary>
	template <typename POSITION_TYPE>
	PauseMobileEntity<POSITION_TYPE>::~PauseMobileEntity()
	{
	}

}
