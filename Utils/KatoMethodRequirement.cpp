#include "stdafx.h"
#include "KatoMethodRequirement.h"

namespace Requirement {
	
	///<summary>
	/// 引数なしコンストラクタ
	///</summary>
	KatoMethodRequirement::KatoMethodRequirement()
	{
	}

	
	///<summary>
	/// 加藤さん卒論手法のコンストラクタ
	///</summary>
	KatoMethodRequirement::KatoMethodRequirement(
		double required_anonymous_area,
		size_t dummy_num,
		int service_interval,
		int interval_of_base_phase,
		int cycle_of_interval_of_base_num,
		int max_pause_time,
		int min_pause_time,
		double average_speed,
		double speed_range
		) : BasicRequirement(required_anonymous_area, dummy_num, average_speed, speed_range),
		service_interval(service_interval),
		interval_of_base_phase(interval_of_base_phase),
		cycle_of_interval_of_base_num(cycle_of_interval_of_base_num),
		max_pause_time(max_pause_time),
		min_pause_time(min_pause_time)
	{
	}

	///<summary>
	/// 加藤さん修論手法のコンストラクタ
	///</summary>
	KatoMethodRequirement::KatoMethodRequirement(
		double required_anonymous_area,
		size_t dummy_num,
		int service_interval,
		double max_variation_of_pause_time,
		double max_variation_of_speed,
		int interval_of_base_phase,
		int cycle_of_interval_of_base_num,
		int max_pause_time,
		int min_pause_time,
		double average_speed,
		double speed_range
		) :BasicRequirement(required_anonymous_area, dummy_num, average_speed, speed_range),
		service_interval(service_interval),
		max_variation_of_pause_time(max_variation_of_pause_time),
		max_variation_of_speed(max_variation_of_pause_time),
		interval_of_base_phase(interval_of_base_phase),
		cycle_of_interval_of_base_num(cycle_of_interval_of_base_num),
		max_pause_time(max_pause_time),
		min_pause_time(min_pause_time)
	{
	}


	///<summary>
	/// デストラクタ
	///</summary>
	KatoMethodRequirement::~KatoMethodRequirement()
	{
	}



}