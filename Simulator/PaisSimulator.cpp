#include "stdafx.h"
#include "PaisSimulator.h"

namespace Simulation
{

	PaisSimulator::PaisSimulator() : BaseSimulator(USER_ID, TRAININGSET_PROPORTION)
	{
	}


	PaisSimulator::~PaisSimulator()
	{
	}

	///<summary>
	/// シミュレーションのパラメータリスト
	///</summary>
	void PaisSimulator::make_requirement_list()
	{
		requirements = {
			std::make_shared<Requirement::BasicRequirement>(std::pow(1000.0, 2), 9, 1.5, 0.5),
			std::make_shared<Requirement::BasicRequirement>(std::pow(1200.0, 2), 9, 1.5, 0.5),
			std::make_shared<Requirement::BasicRequirement>(std::pow(1400.0, 2), 9, 1.5, 0.5),
			std::make_shared<Requirement::BasicRequirement>(std::pow(1600.0, 2), 9, 1.5, 0.5),
			std::make_shared<Requirement::BasicRequirement>(std::pow(1800.0, 2), 9, 1.5, 0.5),
			std::make_shared<Requirement::BasicRequirement>(std::pow(2000.0, 2), 9, 1.5, 0.5),
			std::make_shared<Requirement::BasicRequirement>(std::pow(1000.0, 2), 16, 1.5, 0.5),
			std::make_shared<Requirement::BasicRequirement>(std::pow(1200.0, 2), 16, 1.5, 0.5),
			std::make_shared<Requirement::BasicRequirement>(std::pow(1400.0, 2), 16, 1.5, 0.5),
			std::make_shared<Requirement::BasicRequirement>(std::pow(1600.0, 2), 16, 1.5, 0.5),
			std::make_shared<Requirement::BasicRequirement>(std::pow(1800.0, 2), 16, 1.5, 0.5),
			std::make_shared<Requirement::BasicRequirement>(std::pow(2000.0, 2), 16, 1.5, 0.5),
		};
	}

	void PaisSimulator::run()
	{

	}

	void PaisSimulator::evaluate()
	{

	}

	void PaisSimulator::export_evaluation_result()
	{

	}
}


