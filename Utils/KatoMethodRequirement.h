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
		int cycle_of_interval_of_base_num;//匿名領域確保のための基準地点を決定する間隔の周期

		int service_interval;

		KatoMethodRequirement();
		//加藤さん手法のコンストラクタ
		KatoMethodRequirement(
			double required_anonymous_area, 
			size_t dummy_num, 
			int service_interval, 
			int interval_of_base_phase, 
			int cycle_of_interval_of_base_phase, 
			int max_pause_time = 600, 
			int min_pause_time = 300,
			double average_speed = 1.5, 
			double speed_range = 0.5
		);
	
		virtual ~KatoMethodRequirement();
	
	};
}
