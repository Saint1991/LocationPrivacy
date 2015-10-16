#pragma once
#include "IEvaluationModule.h"

namespace Evaluation
{

	typedef std::string metric_name;

	class Evaluator
	{
	protected:		
		std::unordered_map<metric_name, double> results;
		std::vector <std::shared_ptr<IEvaluationModule>> modules;

	public:
		Evaluator();
		~Evaluator();

		void evaluate();
		void add_module(std::shared_ptr<IEvaluationModule> module);
		void add_modules(const std::vector<std::shared_ptr<IEvaluationModule>>& modules);
		const std::unordered_map<metric_name, double> get_results() const;
	};
}


