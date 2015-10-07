#pragma once
#include "BasicDbMap.h"
#include "LatLng.h"
#include "BasicUser.h"
#include "Dummy.h"
#include "BasicRequirement.h"
#include "IProposedMethod.h"

namespace Method
{

	///<summary>
	/// ¼–ì‚Ì’ñˆÄè–@‚ÌÀ‘•
	///</summary>
	class MatsunoMethod 
		: public Framework::IProposedMethod<Map::BasicDbMap, User::BasicUser<Geography::LatLng>, Entity::Dummy<Geography::LatLng>, Requirement::BasicRequirement>
	{
	protected:
		void initialize();
		void decide_dummy_positions();
		void evaluate();
		void export_results();
		void terminate();

	public:
		MatsunoMethod(std::shared_ptr<Map::BasicDbMap const> map, std::shared_ptr<User::BasicUser<Geography::LatLng>> user, std::shared_ptr<Requirement::BasicRequirement const> requirement, std::shared_ptr<Time::TimeSlotManager> time_manager);
		~MatsunoMethod();
	};
}


