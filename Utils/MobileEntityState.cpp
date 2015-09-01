#include "stdafx.h"
#include "MobileEntityState.h"

namespace Entity
{
	template <typename POSITION_TYPE>
	MobileEntityState<POSITION_TYPE>::MobileEntityState(const POSITION_TYPE& position, double user_probability, const std::string& category_id) 
		: BasicMobileEntityState<POSITION_TYPE>(position, user_probability)
	{
	}

	template <typename POSITION_TYPE>
	MobileEntityState<POSITION_TYPE>::~MobileEntityState()
	{
	}


	///<summary>
	/// ƒJƒeƒSƒŠID‚ðŽæ“¾‚·‚é
	///</summary>
	template <typename POSITION_TYPE>
	const std::string MobileEntityState<POSITION_TYPE>::get_category_id() const
	{
		return category_id;
	}
}
