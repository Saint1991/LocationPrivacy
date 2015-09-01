#include "stdafx.h"
#include "MobileEntityState.h"

namespace Entity
{

	///<summary>
	/// コンストラクタ
	///</summary>
	template <typename POSITION_TYPE>
	MobileEntityState<POSITION_TYPE>::MobileEntityState(const POSITION_TYPE& position, double user_probability, const std::string& category_id) 
		: BasicMobileEntityState<POSITION_TYPE>(position, user_probability)
	{
	}


	///<summary>
	/// デストラクタ
	///</summary>
	template <typename POSITION_TYPE>
	MobileEntityState<POSITION_TYPE>::~MobileEntityState()
	{
	}


	///<summary>
	/// カテゴリIDを取得する
	///</summary>
	template <typename POSITION_TYPE>
	const std::string MobileEntityState<POSITION_TYPE>::get_category_id() const
	{
		return category_id;
	}
}
