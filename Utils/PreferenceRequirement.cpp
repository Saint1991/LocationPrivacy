#include "stdafx.h"
#include "PreferenceRequirement.h"


namespace Requirement
{

	///<summary>
	/// コンストラクタ
	///</summary>
	PreferenceRequirement::PreferenceRequirement() : BasicRequirement()
	{
	}


	///<summary>
	/// コンストラクタ
	///</summary>
	PreferenceRequirement::PreferenceRequirement(double required_anonymous_area, size_t dummy_num, double required_preference_conservation, double average_speed, double speed_range)
		: BasicRequirement(required_anonymous_area, dummy_num, average_speed, speed_range), required_preference_conservation(required_preference_conservation)
	{

	}


	///<summary>
	/// デストラクタ
	///</summary>
	PreferenceRequirement::~PreferenceRequirement()
	{
	}
}

