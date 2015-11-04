#include "stdafx.h"
#include "KatoMasterMethod.h"

namespace Method
{

	///<summary>
	/// �R���X�g���N�^
	/// �����Simulator�ō쐬�����e����͂ւ̎Q�Ƃ�n��
	///</summary>
	KatoMasterMethod::KatoMasterMethod(std::shared_ptr<Map::BasicDbMap const> map, std::shared_ptr<Entity::PauseMobileEntity<Geography::LatLng>> user, std::shared_ptr<Requirement::KatoMethodRequirement const> requirement, std::shared_ptr<Time::TimeSlotManager> time_manager)
		:KatoBachelorMethod(map, user, requirement, time_manager),
		predicted_user(nullptr)
	{
	}

	///<summary>
	/// �f�X�g���N�^
	///</summary>
	KatoMasterMethod::~KatoMasterMethod()
	{
	}
	

	///<summary>
	/// �_�~�[�̍s���v�������C������
	///</summary>
	void KatoMasterMethod::revise_dummy_movement_plan(int phase_id)
	{

	}


	///<summary>
	/// �_�~�[�̒�~���Ԃ̏C��
	///</summary>
	void KatoMasterMethod::revise_dummy_pause_time(int phase_id)
	{
		/*
		//�O�̒l�̕ێ�
		time_t previous_pause_time = creating_dummy->get_pause_time(phase_id);
		time_t previous_arrive_time = time_manager->time_of_phase(phase_id + 1);

		if (std::abs(time_to_change) > requirement->max_variation_of_pause_time)
		{
			time_t new_pause_time = time_to_change > 0 ? previous_pause_time + requirement->max_variation_of_pause_time : previous_pause_time - requirement->max_variation_of_pause_time;
			creating_dummy->set_pause_time(phase_id, new_pause_time);
		}
		else
		{
			creating_dummy->set_pause_time(phase_id, previous_pause_time + time_to_change);
		}

		if (requirement->max_pause_time < time_manager->time_of_phase(phase_id)) creating_dummy->set_pause_time(phase_id, requirement->max_pause_time);
		if (requirement->min_pause_time > time_manager->time_of_phase(phase_id)) creating_dummy->set_pause_time(phase_id, requirement->min_pause_time);

		//��~���Ԃ̕ω��ʂ����߂�
		time_t variation_of_pause_time = creating_dummy->get_pause_time(phase_id) - previous_pause_time;

		for (int i = phase_id + 1; i <= time_manager->phase_count(); i++)
		{
			//revise_time_manager->time_of_phase(i) += variation_of_pause_time;
		}
		if (time_manager->time_of_phase(phase_id + 1) == previous_arrive_time + time_to_change) return;

		time_to_change -= variation_of_pause_time;

		*/
	}

	
	///<summary>
	/// �_�~�[�̈ړ��o�H�̏C��
	///</summary>
	void KatoMasterMethod::revise_dummy_trajectory(int phase_id)
	{
	/*
		time_t previous_arrive_time = time_manager->time_of_phase(phase_id + 1);
		time_t tempT = 10000000000000000000;

		while (�S�Ẵg���W�F�N�g�����`�F�b�N���I����܂�)
		{
		phase_id�̂Ƃ��̑��ݒn�_;
		time_manager->time_of_phase(phase_id + 1) = time_manager->time_of_phase(phase_id) + creating_dummy->get_pause_time(phase_id) + distance_of_Tri / creating_dummy->get_speed(phase_id);

		}
	*/
	}


	///<summary>
	/// �_�~�[�̍s�����x�̏C��
	///</summary>
	void KatoMasterMethod::revise_dummy_speed(int phase_id)
	{
		/*
		time_t previous_arrive_time = time_manager->time_of_phase(phase_id + 1);
		double previous_speed = creating_dummy->get_speed(phase_id);
		double distance = map->shortest_distance(creating_dummy->read_node_pos_info_of_phase(phase_id).first, creating_dummy->read_node_pos_info_of_phase(phase_id).first);
		time_t time = distance / (time_manager->time_of_phase(phase_id + 1) + time_to_change - time_manager->time_of_phase(phase_id));
		creating_dummy->set_speed(phase_id, time);

		if (std::abs(creating_dummy->get_speed(phase_id) - previous_speed) > requirement->max_variation_of_speed)
		{
			double new_speed = creating_dummy->get_speed(phase_id) - previous_speed > 0 ? previous_speed + requirement->max_variation_of_speed : previous_speed - requirement->max_variation_of_speed;
			creating_dummy->set_pause_time(phase_id, new_speed);
		}
		double max_speed = requirement->average_speed_of_dummy + 0.5* requirement->speed_range_of_dummy;
		double min_speed = requirement->average_speed_of_dummy - 0.5* requirement->speed_range_of_dummy;
		if (max_speed < creating_dummy->get_speed(phase_id)) creating_dummy->set_speed(phase_id, max_speed);
		if (min_speed > creating_dummy->get_speed(phase_id)) creating_dummy->set_speed(phase_id, min_speed);

		//time_manager->time_of_phase(phase_id + 1) = time_manager->time_of_phase(phase_id) + creating_dummy->get_pause_time(phase_id) + (time_t)(distance / creating_dummy->get_speed(phase_id));
		time_t variation_of_arrive_time = time_manager->time_of_phase(phase_id + 1) - previous_arrive_time;

		for (int i = phase_id + 1; i <= time_manager->phase_count(); i++)
		{
			//revise_time_manager->time_of_phase(i) += variation_of_pause_time;
		}
		if (time_manager->time_of_phase(phase_id + 1) == previous_arrive_time + time_to_change) return;
		*/
	}


	///<summary>
	/// �_�~�[�̒�~�ʒu�̏C��
	///</summary>
	void KatoMasterMethod::revise_dummy_pose_position(int phase_id)
	{

	}

	///<summary>
	/// ���[�U�̍s���v�����Ɋ܂܂���~�n�_�Ɍ������Ă��邩�ǂ������`�F�b�N
	///</summary>
	bool KatoMasterMethod::check_going_pause_position_in_plan()
	{
		return true;
	}

	///<summary>
	/// ���[�U�̍s���v�����ύX�̔��f
	/// real_user��predict_user�̈Ⴂ�Ō���
	/// ���[�U���v�����ʂ�ɍs�����Ă���ꍇ��0�����^�[��
	///</summary>
	int KatoMasterMethod::check_user_plan()
	{
		if (!check_user_pause_time()) { return 1; }
		else if (!check_user_speed()) { return 2; }
		else if (!check_user_path()) { return 3; }
		else if (!check_user_position()) { return 4; }
		else { return 0; }
	}

	///<summary>
	/// ���[�U�̍s���v�����ύX�̔��f
	/// real_user��predict_user�̈Ⴂ�Ō���
	/// pause_time�̃`�F�b�N
	///</summary>
	bool KatoMasterMethod::check_user_pause_time()
	{
		return true;
	}

	///<summary>
	/// ���[�U�̍s���v�����ύX�̔��f
	/// real_user��predict_user�̈Ⴂ�Ō���
	/// ���x�̃`�F�b�N
	///</summary>
	bool KatoMasterMethod::check_user_speed()
	{
		return true;
	}

	///<summary>
	/// ���[�U�̍s���v�����ύX�̔��f
	/// real_user��predict_user�̈Ⴂ�Ō���
	/// path�̃`�F�b�N
	///</summary>
	bool KatoMasterMethod::check_user_path()
	{
		return true;
	}


	///<summary>
	/// ���[�U�̍s���v�����ύX�̔��f
	/// real_user��predict_user�̈Ⴂ�Ō���
	/// ��~�ʒu�̃`�F�b�N
	///</summary>
	bool KatoMasterMethod::check_user_position()
	{
		return true;
	}

	///<summary>
	/// input_user_plan���烆�[�U�̍s����\�����C���̃��[�U��Ԃ��D
	///</summary>
	std::shared_ptr<Entity::PauseMobileEntity<Geography::LatLng>> KatoMasterMethod::predict_user_plan(std::shared_ptr<Entity::PauseMobileEntity<Geography::LatLng>> input_user_plan)
	{


		return nullptr;
	}


	///<summary>
	/// ���[�U�̎��̒�~�n�_�̓������Ԃ�\������
	///</summary>
	void KatoMasterMethod::predict_user_next_pause_position_time(int check_num)
	{

	}
	///<summary>
	/// ���[�U�̍s���v�������A�b�v�f�[�g����
	///</summary>
	void KatoMasterMethod::update_user_plan()
	{

	}



	///<summary>
	/// ������ (����͓��ɂ�邱�Ƃ͂Ȃ�)
	///</summary>
	void KatoMasterMethod::initialize()
	{
		//���[�U�̓����̕ύX���V�������D
	}


	///<summary>
	/// ��������Ď�@�̊j�ɂȂ镔��
	///</summary>
	void KatoMasterMethod::decide_dummy_positions()
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
	void KatoMasterMethod::revise_dummy_positions()
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
	void KatoMasterMethod::evaluate()
	{

	}


	///<summary>
	/// ���ʂ̃t�@�C���ւ̃G�N�X�|�[�g
	///</summary>
	void KatoMasterMethod::export_results()
	{

	}


	///<summary>
	/// �I������ (����̓X�}�[�g�|�C���^�𗘗p���Ă���̂ŁC���ɂ�邱�Ƃ͂Ȃ�)
	///</summary>
	void KatoMasterMethod::terminate()
	{

	}


	void KatoMasterMethod::run()
	{
		//�����Ŏ��s���Ԃ̌v�����J�n
		timer->start();

		//������
		initialize();

		//���������s����(�e�����̃_�~�[�ʒu���v�Z����)
		decide_dummy_positions();

		//�����Ń��[�U�̍s���̗\����_�~�[�̍s�����C������
		revise_dummy_positions();

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