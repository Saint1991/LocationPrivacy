#include "stdafx.h"
#include "PaisSimulator.h"

namespace Simulation
{

	PaisSimulator::PaisSimulator(int user_id, double testset_proportion) : BaseSimulator(user_id, testset_proportion)
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

	void PaisSimulator::evaluate()
	{

	}

	void PaisSimulator::export_evaluation_result(const std::string& export_path)
	{

	}
}


