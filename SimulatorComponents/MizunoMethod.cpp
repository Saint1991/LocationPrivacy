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
	/// ������
	/// ���ɂ�邱�Ƃ͂Ȃ�
	///</summary>
	void MizunoMethod::initialize()
	{

	}


	///<summary>
	/// ��Ď�@�̓��e
	///</summary>
	void MizunoMethod::decide_dummy_positions()
	{
		std::shared_ptr<User::BasicUser<>> user = entities->get_user();
		int num_of_group_a_dummy = requirement->required_preference_conservation * requirement->dummy_num;
		int num_of_group_b_dummy = requirement->dummy_num - num_of_group_b_dummy;

		decide_dummy_positions_of_group_a(num_of_group_a_dummy);
		decide_dummy_positions_of_group_b(num_of_group_b_dummy);
	}


	///<summary>
	/// �O���[�vA�̃_�~�[�̐���
	///</summary>
	void MizunoMethod::decide_dummy_positions_of_group_a(int num_of_group_a_dummy)
	{
		for (int current_dummy_id = 1; current_dummy_id <= num_of_group_a_dummy; current_dummy_id++) {
			std::shared_ptr<Entity::Dummy<Geography::LatLng>> current_dummy = entities->get_dummy_by_id(current_dummy_id);
		}
	}


	///<summary>
	/// �O���[�vB�̃_�~�[�̐���
	///</summary>
	void MizunoMethod::decide_dummy_positions_of_group_b(int num_of_group_b_dummy)
	{
		for (int current_dummy_id = requirement->dummy_num - num_of_group_b_dummy + 1; current_dummy_id++) {

		}
	}
}

