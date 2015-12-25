#include "stdafx.h"
#include "HayashidaBachelorMethod.h"

namespace Method
{

	///<summary>
	/// �R���X�g���N�^
	/// �����Simulator�ō쐬�����e����͂ւ̎Q�Ƃ�n��
	///</summary>
	HayashidaBachelorMethod::HayashidaBachelorMethod(std::shared_ptr<Map::HayashidaDbMap const> map, std::shared_ptr<Entity::DifferentMovementUser<Geography::LatLng>> user, std::shared_ptr<Requirement::KatoMethodRequirement const> requirement, std::shared_ptr<Time::TimeSlotManager> time_manager)
		: KatoMasterMethod(map, user, requirement, time_manager)
	{
	}

	///<summary>
	/// �f�X�g���N�^
	///</summary>
	HayashidaBachelorMethod::~HayashidaBachelorMethod()
	{
	}

	///<summary>
	/// ���[�U���K��\��POI�Ɍ������Ă��邩�ǂ������`�F�b�N����
	///</summary>
	bool HayashidaBachelorMethod::check_user_going_to_sheduled_POI()
	{
		return true;
	}

	///<summary>
	/// ���[�U�̍s�����ė\������
	///</summary>
	void HayashidaBachelorMethod::repredicted_user_trajectory()
	{
		
	}

	///<summary>
	/// ��Ď�@�̊j�ɂȂ镔��
	/// ���[�U�̌������Đݒ肷��
	///</summary>
	void HayashidaBachelorMethod::re_setting_of_user_cross()
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
	/// ��������Ď�@�̊j�ɂȂ镔��.�_�~�[�̍s�����C������
	///</summary>
	void HayashidaBachelorMethod::adjust_dummy_positions()
	{
		clear_visited_pois_info_list_id_of_users();//users��visited_pois_info_list_id�̃N���A
		for (int phase_id = 0; phase_id < time_manager->phase_count(); phase_id++) {
			for (size_t dummy_id = 1; dummy_id <= entities->get_dummy_count(); dummy_id++) {
				revising_dummy = entities->get_dummy_by_id(dummy_id);
				//�_�~�[��visited_pois_info_list_id�̍X�V
				if ((phase_id - 1) == revising_dummy->get_pause_phases().back()) revising_dummy->increment_visited_pois_info_list_id();
				//���[�U��visited_pois_info_list_id�̍X�V
				increment_visited_pois_info_list_id_of_users(phase_id);
				
				if (!check_user_going_to_sheduled_POI()) {
					repredicted_user_trajectory();
					re_setting_of_user_cross();
				}
				revise_dummy_positions(phase_id, dummy_id);
			}
		}
	}

	/*
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
	*/

	void HayashidaBachelorMethod::run()
	{
		//�����Ŏ��s���Ԃ̌v�����J�n
		timer->start();

		//������
		Method::KatoMasterMethod::initialize();

		//���������s����(�������񑲘_��@[Kato 13])
		Method::KatoBachelorMethod::decide_dummy_positions();

		//�����Ń��[�U�̍s���̗\����_�~�[�̍s�����C������(�ѓc���񑲘_��@[Hayashida 14])
		adjust_dummy_positions();
		
		//�����Ōv�����I��
		timer->end();

		//�I������
		terminate();
	}


}
