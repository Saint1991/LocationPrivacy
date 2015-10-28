#ifdef SIMULATOR_EXPORTS
#define BASE_SIMULATOR_API __declspec(dllexport)
#else
#define BASE_SIMULATOR_API __declspec(dllimport)
#endif

#pragma once
#include "BasicDbMap.h"
#include "LatLng.h"
#include "BasicUser.h"
#include "Dummy.h"
#include "BasicRequirement.h"
#include "ISimulator.h"
#include "WarshallFloyd.h"
#include "DbBasicUserLoader.h"

namespace Simulation
{

	///<summary>
	/// シミュレータ実装の一例
	/// create_userのuser_idはDB上でのユーザのID
	///</summary>
	class BASE_SIMULATOR_API BaseSimulator
		: public ISimulator<Map::BasicDbMap, User::BasicUser<Geography::LatLng>, Entity::Dummy<Geography::LatLng>, Requirement::BasicRequirement>
	{
	public:
		BaseSimulator();
		~BaseSimulator();

		void build_map();
		void create_user(unsigned int user_id);
		virtual void set_comparative_methods() = 0;
		virtual void make_requirement_list() = 0;
		virtual void run() = 0;
	};
}


