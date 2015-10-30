#ifdef SIMULATOR_EXPORTS
#define PAIS_SIMULATOR_API __declspec(dllexport)
#else
#define PAIS_SIMULATOR_API __declspec(dllimport)
#endif

#pragma once
#include "BaseSimulator.h"


namespace Simulation 
{

	///<summary>
	/// PAIS2016向けに作成したシミュレータ By Mizuno
	///</summary>
	class PAIS_SIMULATOR_API PaisSimulator : public BaseSimulator
	{
	public:
		PaisSimulator();
		~PaisSimulator();

		void set_comparative_methods();
		void make_requirement_list();
		void run();
		void evaluate();
		void export_evaluation_result();
	};
}


