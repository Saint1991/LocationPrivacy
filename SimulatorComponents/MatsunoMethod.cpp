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
	/// ��������Ď�@�̊j�ɂȂ镔��
	///</summary>
	void MatsunoMethod::decide_dummy_positions()
	{

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

