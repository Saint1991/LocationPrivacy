#include "stdafx.h"
#include "BasicRequirement.h"

namespace Requirement
{


	///<summary>
	/// コンストラクタ
	///</summary>
	BasicRequirement::BasicRequirement()
	{
	}

	///<summary>
	/// コンストラクタ
	///</summary>
	BasicRequirement::BasicRequirement(double required_anonymous_area, size_t dummy_num, double average_speed, double speed_range)
		: required_anonymous_area(required_anonymous_area), dummy_num(dummy_num), average_speed_of_dummy(average_speed), speed_range_of_dummy(speed_range)
	{
	}


	///<summary>
	/// デストラクタ
	///</summary>
	BasicRequirement::~BasicRequirement()
	{
	}
}


