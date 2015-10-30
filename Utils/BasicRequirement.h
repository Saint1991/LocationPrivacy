#ifdef UTILS_EXPORTS
#define BASIC_REQUIREMENT_API __declspec(dllexport)
#else
#define BASIC_REQUIREMENT_API __declspec(dllimport)
#endif
#pragma once

namespace Requirement
{

	///<summary>
	/// シミュレーションのパラメータ
	///</summary>
	struct BASIC_REQUIREMENT_API BasicRequirement
	{
		double required_anonymous_area;
		size_t dummy_num;
		double average_speed_of_dummy;
		double speed_range_of_dummy;
		
		BasicRequirement();
		BasicRequirement(double required_anonymous_area, size_t dummy_num, double average_speed = 1.5, double speed_range = 0.5);
		virtual ~BasicRequirement();
	};
}


