#pragma once
#include "TimeSlotManager.h"
#include "IEvaluationModule.h"

namespace Evaluation
{


	///<summary>
	/// �]���w�W�v�Z�̂��߂̃N���X
	///</summary>
	template <typename ENTITY_MANAGER, typename REQUIREMENT>
	class IEvaluator
	{
	protected:
		std::shared_ptr<ENTITY_MANAGER const> entities;
		std::shared_ptr<Time::TimeSlotManager> time_manager;
		std::shared_ptr<REQUIREMENT const> requirement;
		std::vector<std::shared_ptr<Evaluation::IEvaluationModule>> modules;
		
	public:

		double mtc = -1.0;
		double ar_count = -1.0;
		double ar_size = -1.0;

		IEvaluator(std::shared_ptr<ENTITY_MANAGER> entities, std::shared_ptr<REQUIREMENT> requirement, std::shared_ptr<Time::TimeSlotManager> time_manager, std::vector<std::shared_ptr<Evaluation::IEvaluationModule>> evaluation_modules)
			: entities(entities), requirement(requirement), time_manager(time_manager), modules(evaluation_modules) {}
		virtual ~IEvaluator();

		virtual void evaluate() = 0;
	};
}


