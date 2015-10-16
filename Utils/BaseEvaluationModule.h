#pragma once
#include "IEvaluationModule.h"
#include "TimeSlotManager.h"

namespace Evaluation
{

	template <typename ENTITY_MANAGER, typename REQUIREMENT>
	class BaseEvaluationModule : public IEvaluationModule
	{
	protected:
		const metric_name METRIC_NAME;
		std::shared_ptr<Time::TimeSlotManager> time_manager;
		std::shared_ptr<REQUIREMENT const> requirement;
		std::shared_ptr<ENTITY_MANAGER const> entities;
		
	public:
		BaseEvaluationModule(std::shared_ptr<ENTITY_MANAGER> entities, std::shared_ptr<REQUIREMENT> requirement, std::shared_ptr<Time::TimeSlotManager> time_manager, const metric_name& metric_name)
			: entities(entities), requirement(requirement), time_manager(time_manager), METRIC_NAME(metric_name) {}
		virtual std::pair<metric_name, metric_value> calculate_evaluation_metric() const = 0;
	};
}