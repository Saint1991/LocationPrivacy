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
		double average_speed;
		double range_of_speed;
		
		BasicRequirement();
		BasicRequirement(double required_anonymous_area, size_t dummy_num);
		virtual ~BasicRequirement();
	};
}


