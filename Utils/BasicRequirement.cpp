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
	BasicRequirement::BasicRequirement(unsigned int simulation_time, unsigned int service_interval, double required_anonymous_area, size_t dummy_num)
		: simulation_time(simulation_time), service_interval(service_interval), required_anonymous_area(required_anonymous_area), dummy_num(dummy_num)
	{
	}


	///<summary>
	/// デストラクタ
	///</summary>
	BasicRequirement::~BasicRequirement()
	{
	}
}


