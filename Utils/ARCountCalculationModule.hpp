
namespace Evaluation
{


	///<summary>
	/// メトリック名の文字列
	///</summary>
	template <typename ENTITY_MANAGER, typename REQUIREMENT, typename POSITION_TYPE>
	const metric_name ARCountCalculationModule<ENTITY_MANAGER, REQUIREMENT, POSITION_TYPE>::AR_COUNT = "ARCount";

	///<summary>
	/// コンストラクタ
	///</summary>
	template <typename ENTITY_MANAGER, typename REQUIREMENT, typename POSITION_TYPE>
	ARCountCalculationModule<ENTITY_MANAGER, REQUIREMENT, POSITION_TYPE>::ARCountCalculationModule(std::shared_ptr<ENTITY_MANAGER> entities, std::shared_ptr<REQUIREMENT> requirement, std::shared_ptr<Time::TimeSlotManager> time_manager)
		: BaseEvaluationModule<ENTITY_MANAGER, REQUIREMENT>(entities, requirement, time_manager, AR_COUNT)
	{
	}


	///<summary>
	/// デストラクタ
	///</summary>
	template <typename ENTITY_MANAGER, typename REQUIREMENT, typename POSITION_TYPE>
	ARCountCalculationModule<ENTITY_MANAGER, REQUIREMENT, POSITION_TYPE>::~ARCountCalculationModule()
	{
	}


	///<summary>
	/// ARCountの計算 (LatLng)
	///</summary>
	template <typename ENTITY_MANAGER, typename REQUIREMENT, typename POSITION_TYPE>
	std::pair<metric_name, metric_value> ARCountCalculationModule<ENTITY_MANAGER, REQUIREMENT, POSITION_TYPE>::calculate_evaluation_metric() const
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


	///<summary>
	/// ARCountの計算 (Coordinate)
	///</summary>
	template <typename ENTITY_MANAGER, typename REQUIREMENT>
	std::pair<metric_name, metric_value> ARCountCalculationModule<ENTITY_MANAGER, REQUIREMENT, Graph::Coordinate>::calculate_evaluation_metric() const
	{
		metric_value result = -1.0;
		int achive_count = 0;
		entities->positions_for_each_phase([&](int phase, time_t time, const std::vector<std::shared_ptr<Graph::Coordinate const>>& positions) {
			double convex_hull_size = Graph::GraphUtility::calc_convex_hull_size(positions);
			double required = requirement->required_anonymous_area;
			if (convex_hull_size >= required) achive_count++;
		});
	
		result = (double)achive_count / time_manager->phase_count();
		return std::make_pair(METRIC_NAME, result);
	}
}



