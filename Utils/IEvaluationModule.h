#pragma once

namespace Evaluation
{

	template <typename ENTITY_MANAGER, typename REQUIREMENT>
	class IEvaluationModule 
	{
	protected:
		std::shared_ptr<ENTITY_MANAGER const> entities;
		std::shared_ptr<Time::TimeSlotManager> time_manager;
		std::shared_ptr<REQUIREMENT const> requirement;
	public:
		const std::string METRIC_NAME;
		IEvaluationModule(std::shared_ptr<ENTITY_MANAGER> entities, std::shared_ptr<REQUIREMENT> requirement, std::shared_ptr<Time::TimeSlotManager> time_manager, std::string metric_name)
			: entities(entities), requirement(requirement), time_manager(time_manager), METRIC_NAME(metric_name) {}
		
			virtual std::pair<std::string> caluculate_evaluation_metric() const = 0;
	};
}