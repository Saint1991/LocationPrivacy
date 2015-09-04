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
		unsigned int simulation_time;
		unsigned int service_interval;
		double required_anonymous_area;
		size_t dummy_num;
		
		BasicRequirement();
		BasicRequirement(unsigned int simulation_time, unsigned int service_interval, double required_anonymous_area, size_t dummy_num);
		~BasicRequirement();
	};
}


