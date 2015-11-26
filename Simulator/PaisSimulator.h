#ifdef SIMULATOR_EXPORTS
#define PAIS_SIMULATOR_API __declspec(dllexport)
#else
#define PAIS_SIMULATOR_API __declspec(dllimport)
#endif

#pragma once
#include "BaseSimulator.h"
#include "MizunoMethod.h"
#include "MatsunoMethod.h"
#include "PreferenceRequirement.h"


namespace Simulation 
{

	///<summary>
	/// PAIS2016向けに作成したシミュレータ By Mizuno
	///</summary>
	class PAIS_SIMULATOR_API PaisSimulator : public BaseSimulator
	{
	private:
		static constexpr unsigned int USER_ID = 29U;
		static constexpr double TRAININGSET_PROPORTION = 0.4;
		static constexpr char* DB_NAME = "map_tokyo";
		
	protected:
		void make_requirement_list();
	
	public:
		PaisSimulator();
		~PaisSimulator();

		void run();
		void evaluate();
		void export_evaluation_result();
	};
}


