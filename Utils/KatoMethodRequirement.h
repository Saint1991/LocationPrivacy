#ifdef UTILS_EXPORTS
#define KATOMETHODREQUIREMENT_API __declspec(dllexport)
#else
#define KATOMETHODREQUIREMENT_API __declspec(dllimport)
#endif

#pragma once
#include "BasicRequirement.h"

namespace Requirement {

	///<summary>
	/// 加藤手法に必要なRequirement
	///</summary>
	class KATOMETHODREQUIREMENT_API KatoMethodRequirement : public Requirement::BasicRequirement
	{
	public:
		time_t max_pause_time;
		time_t min_pause_time;

		time_t max_variation_of_pause_time;
		double max_variation_of_speed;

		int phase_interval;//グリッドを計算する周期
		int cycle_of_anonymous_area;//匿名領域を決定する時の周期

		time_t service_interval;
		

		KatoMethodRequirement();
		KatoMethodRequirement(double max_pause_time, double min_pause_time, time_t max_variation_of_pause_time, double max_variation_of_speed, int cycle_of_anonymous_area, time_t service_interval);
		virtual ~KatoMethodRequirement();
	};
}
