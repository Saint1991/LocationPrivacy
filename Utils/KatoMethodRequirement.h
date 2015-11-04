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
		//加藤さん卒論手法・修論手法に共通なパラメータ
		int max_pause_time;
		int min_pause_time;
				
		int interval_of_base_phase;//匿名領域確保のための基準地点を決定する間隔
		int cycle_of_interval_of_base_phase;//匿名領域確保のための基準地点を決定する間隔の周期

		int service_interval;
		

		//加藤さん修論手法のみに必要なパラメータ
		int max_variation_of_pause_time;
		double max_variation_of_speed;

		
		KatoMethodRequirement();
		
		//加藤さん卒論手法のコンストラクタ
		KatoMethodRequirement(
			double required_anonymous_area, 
			size_t dummy_num, 
			int service_interval, 
			int interval_of_base_phase, 
			int cycle_of_interval_of_base_phase, 
			int max_pause_time = 600, 
			int min_pause_time = 60, 
			double average_speed = 3.0, 
			double speed_range = 2.0
		);
		
		//加藤さん修論手法のコンストラクタ
		KatoMethodRequirement(
			double required_anonymous_area,
			size_t dummy_num,
			int service_interval,
			int max_variation_of_pause_time,
			double max_variation_of_speed,
			int interval_of_base_phase,
			int cycle_of_interval_of_base_phase,
			int max_pause_time = 600,
			int min_pause_time = 60,
			double average_speed = 3.0,
			double speed_range = 2.0
		);

		virtual ~KatoMethodRequirement();
	
	};
}
