#ifdef SIMULATOR_EXPORTS
#define PAIS_SIMULATOR_API __declspec(dllexport)
#else
#define PAIS_SIMULATOR_API __declspec(dllimport)
#endif

#pragma once
#include "BaseSimulator.h"
#include "MizunoMethod.h"
#include "MatsunoMethod.h"


namespace Simulation 
{

	///<summary>
	/// PAIS2016向けに作成したシミュレータ By Mizuno
	///</summary>
	class PAIS_SIMULATOR_API PaisSimulator : public BaseSimulator
	{
	public:
		PaisSimulator(int user_id, double testset_proportion);
		~PaisSimulator();

		void make_requirement_list();
		void evaluate();
		void export_evaluation_result(const std::string& export_path);
	};
}


