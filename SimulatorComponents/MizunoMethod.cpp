#include "stdafx.h"
#include "MizunoMethod.h"

namespace Method
{

	MizunoMethod::MizunoMethod(std::shared_ptr<Map::BasicDbMap const> map, std::shared_ptr<User::BasicUser<Geography::LatLng>> user, std::shared_ptr<Requirement::PreferenceRequirement const> requirement, std::shared_ptr<Time::TimeSlotManager const> time_manager)
		: Framework::IProposedMethod<Map::BasicDbMap, User::BasicUser<Geography::LatLng>, Entity::Dummy<Geography::LatLng>, Requirement::PreferenceRequirement, Geography::LatLng, Graph::SemanticTrajectory<Geography::LatLng>>(map, user, requirement, time_manager)
	{
	}

	MizunoMethod::~MizunoMethod()
	{
	}


	///<summary>
	/// ‰Šú‰»
	/// “Á‚É‚â‚é‚±‚Æ‚Í‚È‚¢
	///</summary>
	void MizunoMethod::initialize()
	{

	}


	///<summary>
	/// ’ñˆÄè–@‚Ì“à—e
	///</summary>
	void MizunoMethod::decide_dummy_positions()
	{
		std::shared_ptr<User::BasicUser<>> user = entities->get_user();

	}
}

