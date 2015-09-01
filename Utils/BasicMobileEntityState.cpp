#include "stdafx.h"
#include "BasicMobileEntityState.h"

namespace Entity 
{

	///<summary>
	/// コンストラクタ
	///</summary>
	template <typename POSITION_TYPE>
	BasicMobileEntityState<POSITION_TYPE>::BasicMobileEntityState(const POSITION_TYPE& position, double probability) 
		: position(std::make_shared<POSITION_TYPE>(position)), user_probability(probability)
	{
	}


	///<summary>
	/// デストラクタ
	///</summary>
	template <typename POSITION_TYPE>
	BasicMobileEntityState<POSITION_TYPE>::~BasicMobileEntityState()
	{
	}


	///<summary>
	/// 位置を取得する
	///</summary>
	template <typename POSITION_TYPE>
	const std::shared_ptr<POSITION_TYPE const> BasicMobileEntityState<POSITION_TYPE>::read_position() const
	{
		return position;
	}

	///<summary>
	/// ユーザ確率の取得
	///</summary>
	template <typename POSITION_TYPE>
	double BasicMobileEntityState<POSITION_TYPE>::get_user_probability() const
	{
		return user_probability;
	}


	#pragma region Export
	template <typename POSITION_TYPE>
	const std::string BasicMobileEntityState<POSITION_TYPE>::USER_PROBABILITY = "User Probability";

	template <typename POSITION_TYPE>
	std::unordered_map<std::string, std::string> BasicMobileEntityState<POSITION_TYPE>::get_export_data() const
	{
		std::unordered_map<std::string, std::string> ret = position->get_export_data();
		ret.insert(std::make_pair(USER_PROBABILITY, std::to_string(user_probability)));
		return ret;
	}
	#pragma endregion Export関連
}

