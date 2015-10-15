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
		const double AVERAGE_SPEED = 5.0;
		const double RANGE_OF_SPEED = 2.5;
		
		BasicRequirement();
		BasicRequirement(double required_anonymous_area, size_t dummy_num);
		virtual ~BasicRequirement();
	};
}


