#pragma once
#include "stdafx.h"
#include "BasicRequirement.h"
#include "TimeSlotManager.h"
#include "Timer.h"
#include "FileExporter.h"
#include "EntityManager.h"

namespace Framework
{


	///<summary>
	/// ��Ď�@����������ۂ̃e���v���[�g
	/// ������p�������N���X���쐬���C�������z�֐�����������
	///</summary>
	template <typename MAP_TYPE, typename USER_TYPE, typename DUMMY_TYPE, typename REQUIREMENT_TYPE, typename POSITION_TYPE = Geography::LatLng>
	class IProposedMethod
	{
	static_assert(std::is_base_of<Requirement::BasicRequirement, REQUIREMENT_TYPE>::value, "template type REQUIREMENT_TYPE must be derived from BasicRequirement");
	
	protected:
		std::shared_ptr<MAP_TYPE const> map;
		std::shared_ptr<REQUIREMENT_TYPE const> requirement;
		std::shared_ptr<Time::TimeSlotManager> time_manager;
		std::shared_ptr<Entity::EntityManager<DUMMY_TYPE, USER_TYPE, POSITION_TYPE>> entities;
		std::shared_ptr<Time::Timer> timer;
		

		///<summary>
		/// ���͂̉��H���̏���������
		///</summary>
		virtual void initialize() = 0;


		///<summary>
		/// ��L�̃t�B�[���h�Ɋ�Â��Ċe�t�F�[�Y�ɂ�����_�~�[�̈ʒu�����肷��
		///</summary>
		virtual void decide_dummy_positions() = 0;


		///<summary>
		/// ���肵���_�~�[�̈ʒu���Ɋ�Â��āCMTC�⓽���̈�̌v�Z���s��
		///</summary>
		virtual void evaluate() = 0;


		///<summary>
		/// �v�Z�����]���l�̃t�@�C���ւ̃G�N�X�|�[�g���s��
		///</summary>
		virtual void export_results() = 0;


		///<summary>
		/// �I������
		/// ����̓X�}�[�g�|�C���^��p���Ă���̂ŉ������ׂ����\�[�X���͂قڂȂ��͂�
		///</summary>
		virtual void terminate() = 0;

	public:
		IProposedMethod(std::shared_ptr<MAP_TYPE const> map, std::shared_ptr<USER_TYPE> user, std::shared_ptr<REQUIREMENT_TYPE const> requirement, std::shared_ptr<Time::TimeSlotManager> time_manager);
		virtual ~IProposedMethod();

		void run();
	};
}

#include "IProposedMethod.hpp"
