#include "stdafx.h"
#include "KatoMethodRequirement.h"

namespace Requirement {
	
	///<summary>
	/// コンストラクタ
	///</summary>
	KatoMethodRequirement::KatoMethodRequirement()
	{
	}

	
	///<summary>
	/// コンストラクタ
	///</summary>
	KatoMethodRequirement::KatoMethodRequirement(double max_pause_time, double min_pause_time, time_t max_variation_of_pause_time, double max_variation_of_speed, int cycle_of_anonymous_area, time_t service_interval)
		:max_pause_time(max_pause_time),
		 min_pause_time(min_pause_time),
	 	 max_variation_of_pause_time(max_variation_of_pause_time),
	 	 max_variation_of_speed(max_variation_of_speed),
		 cycle_of_anonymous_area(cycle_of_anonymous_area),
		 service_interval(service_interval)
	{
	}

	///<summary>
	/// デストラクタ
	///</summary>
	KatoMethodRequirement::~KatoMethodRequirement()
	{
	}

}