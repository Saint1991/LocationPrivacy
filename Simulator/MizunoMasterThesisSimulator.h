#ifdef SIMULATOR_EXPORTS
#define MIZUNO_MASTER_SIMULATOR_API __declspec(dllexport)
#else
#define  MIZUNO_MASTER_SIMULATOR_API __declspec(dllimport)
#endif

#pragma once
#include "ISimulator.h"

namespace Simulation
{
	class  MIZUNO_MASTER_SIMULATOR_API MizunoMasterThesisSimulator
	{
	public:
		MizunoMasterThesisSimulator();
		~MizunoMasterThesisSimulator();
	};
}


