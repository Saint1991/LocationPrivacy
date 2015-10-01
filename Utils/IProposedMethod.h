#pragma once
#include "stdafx.h"
#include "BasicRequirement.h"
#include "TimeSlotManager.h"
#include "Timer.h"
#include "FileExporter.h"

namespace Framework
{
	template <typename MAP_TYPE, typename USER_TYPE, typename DUMMY_TYPE, typename REQUIREMENT_TYPE>
	class IProposedMethod
	{
	static_assert(std::is_base_of<Requirement::BasicRequirement, REQUIREMENT_TYPE>::value, "template type REQUIREMENT_TYPE must be derived from BasicRequirement");
	
	protected:
		std::shared_ptr<MAP_TYPE const> map;
		std::shared_ptr<USER_TYPE const> user;
		std::shared_ptr<REQUIREMENT_TYPE const> requirement;
		std::shared_ptr<Time::TimeSlotManager> time_manager;
		std::shared_ptr<std::vector<std::shared_ptr<DUMMY_TYPE>>> dummies;
		std::shared_ptr<Time::Timer> timer;
		
		virtual void initialize() = 0;
		virtual void decide_dummy_positions() = 0;
		virtual void evaluate = 0;
		virtual void export_results() = 0;
		virtual void terminate() = 0;

		void for_each_dummy(const std::function<void(int, std::shared_ptr<DUMMY_TYPE>)>& execute_function);

	public:
		IProposedMethod(std::shared_ptr<MAP_TYPE const> map, std::shared_ptr<USER_TYPE const> user, std::shared_ptr<REQUIREMENT_TYPE const> requirement, std::shared_ptr<Time::TimeSlotManager> time_manager);
		virtual ~IProposedMethod();

		void run();
	};
}

#include "IProposedMethod.hpp"
