#include "stdafx.h"
#include "HayashidaBachelorMethod.h"

namespace Method
{

	///<summary>
	/// �R���X�g���N�^
	/// �����Simulator�ō쐬�����e����͂ւ̎Q�Ƃ�n��
	///</summary>
	HayashidaBachelorMethod::HayashidaBachelorMethod(std::shared_ptr<Map::BasicDbMap const> map, std::shared_ptr<Entity::PauseMobileEntity<Geography::LatLng>> user, std::shared_ptr<Requirement::KatoMethodRequirement const> requirement, std::shared_ptr<Time::TimeSlotManager> time_manager)
		: KatoBachelorMethod(map, user, requirement, time_manager),
		predicted_user(nullptr)
	{
	}

	///<summary>
	/// �f�X�g���N�^
	///</summary>
	HayashidaBachelorMethod::~HayashidaBachelorMethod()
	{
	}

	///<summary>
	/// ������ (����͓��ɂ�邱�Ƃ͂Ȃ�)
	///</summary>
	void HayashidaBachelorMethod::initialize()
	{
		//���[�U�̓����̕ύX���V�������D
	}


	///<summary>
	/// ��������Ď�@�̊j�ɂȂ镔��
	///</summary>
	void HayashidaBachelorMethod::decide_dummy_positions()
	{
		
		/*
		for (size_t dummy_id = 1; dummy_id <= entities->get_dummy_count(); dummy_id++)
		{
			creating_dummy = entities->get_dummy_by_id(dummy_id);

			decide_base_positions_and_arrive_time(dummy_id);// �������_�~�[(k�Ԗ�)�̊�n�_����ъ�n�_�������Ԃ̌���
			decide_share_positions_and_arrive_time(dummy_id);// �������_�~�[(k�Ԗ�)�̋��L�n�_����ы��L�n�_�������Ԃ̌���
			decide_destination_on_the_way(dummy_id);// �������_�~�[(k�Ԗ�)�̈ړ��o�H�̌���
		}
		*/
	}


	///<summary>
	/// ��������Ď�@�̊j�ɂȂ镔��.�_�~�[�̍s�����C������
	///</summary>
	void HayashidaBachelorMethod::adjust_dummy_positions()
	{
		/*
		time_t time_to_change = 0;// (�C�����)time_manager->time_of_phase(phase_id) - (�C���O��)time_manager->time_of_phase(phase_id);
		predict_user = predict_user_plan(entities->get_user());

		for (int phase_id = 0; phase_id < time_manager->phase_count(); phase_id++)
		{
			if (check_going_pause_position_in_plan()) {
				if (check_user_plan() != 0) {
					predict_user_next_pause_position_time(check_user_plan());//���̒�~�n�_�̓������Ԃ�\��
					revise_dummy_movement_plan(phase_id);
					update_user_plan();
				}
			}
		}
		*/
		
	}


	///<summary>
	/// ���肵���ʒu�����MTC���e��]���l���Z�o����
	///</summary>
	void HayashidaBachelorMethod::evaluate()
	{

	}


	///<summary>
	/// ���ʂ̃t�@�C���ւ̃G�N�X�|�[�g
	///</summary>
	void HayashidaBachelorMethod::export_results()
	{

	}


	///<summary>
	/// �I������ (����̓X�}�[�g�|�C���^�𗘗p���Ă���̂ŁC���ɂ�邱�Ƃ͂Ȃ�)
	///</summary>
	void HayashidaBachelorMethod::terminate()
	{

	}


	void HayashidaBachelorMethod::run()
	{
		//�����Ŏ��s���Ԃ̌v�����J�n
		timer->start();

		//������
		initialize();

		//���������s����(�e�����̃_�~�[�ʒu���v�Z����)
		decide_dummy_positions();

		//�����Ń��[�U�̍s���̗\����_�~�[�̍s�����C������
		adjust_dummy_positions();

		//�����Ōv�����I��
		timer->end();

		//�ݒ肵���_�~�[�C���[�U�̈ʒu�����MTC�Ȃǂ̕]���w�W���v�Z����
		evaluate();

		//���s���ԈȊO�̃G�N�X�|�[�g
		export_results();

		//�I������
		terminate();
	}


}
