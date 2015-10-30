#pragma once
#include "BasicDbMap.h"
#include "LatLng.h"
#include "BasicUser.h"
#include "Dummy.h"
#include "BasicRequirement.h"
#include "IProposedMethod.h"
#include "SemanticTrajectory.h"

namespace Method
{

	///<summary>
	/// ����̒�Ď�@�̎���
	///</summary>
	class MatsunoMethod 
		: public Framework::IProposedMethod<Map::BasicDbMap, User::BasicUser<Geography::LatLng>, Entity::Dummy<Geography::LatLng>, Requirement::BasicRequirement, Geography::LatLng, Graph::SemanticTrajectory<Geography::LatLng>>
	{
	private:
		//�����̃p�^�[���t���O PATTERN1=>���炷�����C PATTERN2=>���Ă����
		enum cross_pattern {PATTERN1, PATTERN2};
		void set_crossing_points(Entity::entity_id id, std::shared_ptr<Entity::Dummy<Geography::LatLng>> dummy);
		void set_expanding_points(Entity::entity_id id, std::shared_ptr<Entity::Dummy<Geography::LatLng>> dummy);

	protected:
		void initialize();
		void decide_dummy_positions();

	public:
		MatsunoMethod(std::shared_ptr<Map::BasicDbMap const> map, std::shared_ptr<User::BasicUser<Geography::LatLng>> user, std::shared_ptr<Requirement::BasicRequirement const> requirement, std::shared_ptr<Time::TimeSlotManager> time_manager);
		~MatsunoMethod();
	};
}


