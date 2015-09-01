#pragma once
#include "stdafx.h"
#include "BasicRequirement.h"

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
		std::unique_ptr<std::vector<DUMMY_TYPE>> dummies;
	
	public:
		IProposedMethod(std::shared_ptr<MAP_TYPE const> map, std::shared_ptr<USER_TYPE const> user, std::shared_ptr<REQUIREMENT_TYPE const> requirement);
		virtual ~IProposedMethod();

		virtual void initialize() = 0;
		virtual void run() = 0;
		virtual void export_results() = 0;
		virtual void terminate() = 0;
	};
}

#include "IProposedMethod.hpp"
