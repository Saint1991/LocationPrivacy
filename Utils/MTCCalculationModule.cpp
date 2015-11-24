#include "stdafx.h"
#include "MTCCalculationModule.h"

namespace Evaluation {

	///<summary>
	/// メトリック名の文字列
	///</summary>
	template <typename ENTITY_MANAGER, typename REQUIREMENT, typename POSITION_TYPE>
	const metric_name MTCCalculationModule<ENTITY_MANAGER, REQUIREMENT, POSITION_TYPE>::MTC = "MTC";

	///<summary>
	/// コンストラクタ
	///</summary>
	template <typename ENTITY_MANAGER, typename REQUIREMENT, typename POSITION_TYPE>
	MTCCalculationModule<ENTITY_MANAGER, REQUIREMENT, POSITION_TYPE>::MTCCalculationModule(std::shared_ptr<ENTITY_MANAGER> entities, std::shared_ptr<REQUIREMENT> requirement, std::shared_ptr<Time::TimeSlotManager> time_manager)
		: BaseEvaluationModule<ENTITY_MANAGER, REQUIREMENT>(entities, requirement, time_manager, MTC)
	{
	}
	

	///<summary>
	/// デストラクタ
	///</summary>
	template <typename ENTITY_MANAGER, typename REQUIREMENT, typename POSITION_TYPE>
	MTCCalculationModule<ENTITY_MANAGER, REQUIREMENT, POSITION_TYPE>::~MTCCalculationModule()
	{
	}

	///<summary>
	/// MTCの計算 (LatLng)
	///</summary>
	template <typename ENTITY_MANAGER, typename REQUIREMENT, typename POSITION_TYPE>
	std::pair<metric_name, metric_value> MTCCalculationModule<ENTITY_MANAGER, REQUIREMENT, POSITION_TYPE>::calculate_evaluation_metric() const
	{
		metric_value result = -1.0;
		int achive_count = 0;
		entities->positions_for_each_phase([&](int phase, time_t time, const std::vector<std::shared_ptr<POSITION_TYPE const>>& positions) {
			double convex_hull_size = Geography::GeoCalculation::calc_convex_hull_size(positions);
			double required = requirement->required_anonymous_area;
			if (convex_hull_size >= required) achive_count++;
		});

		result = (double)achive_count / time_manager->phase_count();
		return std::make_pair(METRIC_NAME, result);
	}

}
