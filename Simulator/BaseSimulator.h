#ifdef SIMULATOR_EXPORTS
#define BASE_SIMULATOR_API __declspec(dllexport)
#else
#define BASE_SIMULATOR_API __declspec(dllimport)
#endif

#pragma once
#include "BasicDbMap.h"
#include "LatLng.h"
#include "BasicUser.h"
#include "BasicRequirement.h"
#include "ISimulator.h"

namespace Simulation
{

	///<summary>
	/// シミュレータ実装の一例
	///</summary>
	class BASE_SIMULATOR_API BaseSimulator : public ISimulator<Map::BasicDbMap, User::BasicUser<Geography::LatLng>, Requirement::BasicRequirement>
	{
	public:
		BaseSimulator();
		~BaseSimulator();

		void build_map();
		void create_user();
		void make_requirement_list();
		void run();
	};
}


