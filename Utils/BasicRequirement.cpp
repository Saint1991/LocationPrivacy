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
	BasicRequirement::BasicRequirement(double required_anonymous_area, size_t dummy_num) : required_anonymous_area(required_anonymous_area), dummy_num(dummy_num)
	{
	}


	///<summary>
	/// デストラクタ
	///</summary>
	BasicRequirement::~BasicRequirement()
	{
	}
}


