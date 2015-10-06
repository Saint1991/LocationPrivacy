#include "stdafx.h"
#include "BaseSimulator.h"

namespace Simulation
{


	BaseSimulator::BaseSimulator() : ISimulator<Map::BasicDbMap, User::BasicUser<Geography::LatLng>, Requirement::BasicRequirement>()
	{
	}


	BaseSimulator::~BaseSimulator()
	{
	}

	void BaseSimulator::build_map()
	{

	}

	void BaseSimulator::create_user()
	{

	}

	void BaseSimulator::make_requirement_list()
	{

	}

	void BaseSimulator::run()
	{

	}
}

