#include "stdafx.h"
#include "MatsunoMethod.h"

namespace Method
{

	///<summary>
	/// �R���X�g���N�^
	/// �����Simulator�ō쐬�����e����͂ւ̎Q�Ƃ�n��
	///</summary>
	MatsunoMethod::MatsunoMethod(std::shared_ptr<Map::BasicDbMap const> map, std::shared_ptr<User::BasicUser<Geography::LatLng>> user, std::shared_ptr<Requirement::BasicRequirement const> requirement, std::shared_ptr<Time::TimeSlotManager> time_manager)
		: Framework::IProposedMethod<Map::BasicDbMap, User::BasicUser<Geography::LatLng>, Entity::Dummy<Geography::LatLng>, Requirement::BasicRequirement>(map, user, requirement, time_manager)
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
		Entity::entity_id cross_target = entities->get_min_cross_entity_id();
		int target_phase = -1;

		//���[�U�ɑ΂������
		if (cross_target == 0) {
			std::shared_ptr<User::BasicUser<Geography::LatLng> const> user = entities->get_user();
			target_phase = user->randomly_pick_cross_not_set_phase();
		}
		//�_�~�[�ɑ΂������
		else {
			std::shared_ptr<Entity::Dummy<Geography::LatLng>> dummy = entities->get_dummy_by_id(cross_target);
			target_phase = dummy->randomly_pick_cross_not_set_phase();
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


	

	///<summary>
	/// ���肵���ʒu�����MTC���e��]���l���Z�o����
	///</summary>
	void MatsunoMethod::evaluate()
	{

	}


	///<summary>
	/// ���ʂ̃t�@�C���ւ̃G�N�X�|�[�g
	///</summary>
	void MatsunoMethod::export_results()
	{

	}


	///<summary>
	/// �I������ (����̓X�}�[�g�|�C���^�𗘗p���Ă���̂ŁC���ɂ�邱�Ƃ͂Ȃ�)
	///</summary>
	void MatsunoMethod::terminate()
	{

	}
}

