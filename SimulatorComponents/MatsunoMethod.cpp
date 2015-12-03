#include "stdafx.h"
#include "MatsunoMethod.h"

namespace Method
{

	///<summary>
	/// �R���X�g���N�^
	/// �����Simulator�ō쐬�����e����͂ւ̎Q�Ƃ�n��
	///</summary>
	MatsunoMethod::MatsunoMethod(std::shared_ptr<Map::BasicDbMap const> map, std::shared_ptr<User::BasicUser<Geography::LatLng>> user, std::shared_ptr<Requirement::BasicRequirement const> requirement, std::shared_ptr<Time::TimeSlotManager> time_manager)
		: Framework::IProposedMethod<Map::BasicDbMap, User::BasicUser<Geography::LatLng>, Entity::Dummy<Geography::LatLng>, Requirement::BasicRequirement, Geography::LatLng, Graph::SemanticTrajectory<Geography::LatLng>>(map, user, requirement, time_manager)
	{
	}

	///<summary>
	/// �f�X�g���N�^
	///</summary>
	MatsunoMethod::~MatsunoMethod()
	{
	}


	///<summary>
	/// ������ (����͓��ɂ�邱�Ƃ͂Ȃ�)
	///</summary>
	void MatsunoMethod::initialize()
	{

	}

	///<summary>
	/// ID��id�̃_�~�[�ɑ΂��鋤�L�n�_(����)�̐ݒ�
	///</summary>
	void MatsunoMethod::set_crossing_points(Entity::entity_id id, std::shared_ptr<Entity::Dummy<Geography::LatLng>> dummy)
	{
		
		//�����񐔂����Ȃ��G���e�B�e�B��D��I�Ɍ����Ώۂɂ��ČJ��Ԃ�
		std::vector<std::pair<Entity::entity_id, int>> entity_list_order_by_cross = entities->get_entity_id_list_order_by_cross_count();
		for (std::vector<std::pair<Entity::entity_id, int>>::const_iterator iter = entity_list_order_by_cross.begin(); iter != entity_list_order_by_cross.end(); iter++) {
			
			//�������̃_�~�[�̂��̂̓X�L�b�v
			if (iter->first > id) continue;

			//�����Ώۂ�ID
			Entity::entity_id cross_target = iter->second;
			int target_phase = INVALID;

			//BasicUser��Dummy���p�����Ă���̂Ń|�C���^����͉\
			std::shared_ptr<Entity::Dummy<Geography::LatLng>> target = cross_target == 0 ? entities->get_user() : entities->get_dummy_by_id(cross_target);
			std::vector<int> target_phases = target->find_cross_not_set_phases();
			
			//�������ݒ�̎������烉���_���ɋ��L�n�_�ݒ�����݂�
			std::random_device device;
			std::mt19937_64 generator(device());
			std::shuffle(target_phases.begin(), target_phases.end(), generator);

			for (std::vector<int>::const_iterator target_phase = target_phases.begin(); target_phase != target_phases.end(); target_phase++) {
				if (*target_phase == 0) continue;


			}
		}


		
	}


	///<summary>
	/// ID��id�̃_�~�[�ɑ΂����n�_(�����̈�m��)�̐ݒ�
	///</summary>
	void MatsunoMethod::set_expanding_points(Entity::entity_id id, std::shared_ptr<Entity::Dummy<Geography::LatLng>> dummy)
	{

	}


	///<summary>
	/// ��������Ď�@�̊j�ɂȂ镔��
	///</summary>
	void MatsunoMethod::decide_dummy_positions()
	{
		entities->for_each_dummy([&](Entity::entity_id id, std::shared_ptr<Entity::Dummy<Geography::LatLng>> dummy) {
			set_crossing_points(id, dummy);
			set_expanding_points(id, dummy);
		});
	}
}

