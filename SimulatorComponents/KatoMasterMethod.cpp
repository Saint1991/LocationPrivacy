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
		real_user(nullptr), predicted_user(nullptr)
	{
		input_user = entities->get_user();
	}

	///<summary>
	/// �f�X�g���N�^
	///</summary>
	KatoMasterMethod::~KatoMasterMethod()
	{
	}
	
	///<summary>
	/// input_user_plan���烆�[�U�̍s����\�����C���̃��[�U��Ԃ��D
	/// ���������@�̏ꍇ�́C�e�v�f�̐����m���ɂ���Ċ��������߂�D
	/// ���̊����ɏ]���āC���ۂ̃��[�U���쐬����
	///</summary>
	std::shared_ptr<Entity::PauseMobileEntity<Geography::LatLng>> KatoMasterMethod::make_predicted_user_plan(std::shared_ptr<Entity::PauseMobileEntity<Geography::LatLng>> input_user)
	{
		std::shared_ptr<std::vector<std::shared_ptr<Geography::LatLng>>> input_user_positions = input_user->read_trajectory()->read_positions();
		std::shared_ptr<std::vector<std::shared_ptr<Geography::LatLng>>> predicted_user_positions;

		std::copy(input_user_positions->begin(), input_user_positions->end(), predicted_user_positions->begin());

		return predicted_user;
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
	/// ���[�U���v�����ʂ�ɍs�����Ă���ꍇ��NO_CHANGE�����^�[��
	///</summary>
	std::pair<KatoMasterMethod::ChangeParameter, double> KatoMasterMethod::check_user_plan(int now_phase)
	{
		//�~�܂��Ă��遨��~���Ԃ̕ύXor��~�n�_�̕ύX
		//�����Е��ł���~���Ă����灨��~���ԕύX�̔��f
		if (predicted_user->check_pause_flag() || real_user->check_pause_flag()) {
			return check_user_pause_time(now_phase);
		}
		//�����Ă�����C
		else {
			if (/*�������`��Ԃ������̏�Ɉʒu���Ă�����C*/true) {
				return check_user_speed(now_phase);
			}
			else {
				return check_user_path(now_phase);
			}
		}
	}
	
		///<summary>
		/// ���[�U�̍s���v�����ύX�̔��f
		/// real_user��predicted_user�̈Ⴂ�Ō���
		/// pause_time�̃`�F�b�N
		///</summary>
		std::pair<KatoMasterMethod::ChangeParameter, double> KatoMasterMethod::check_user_pause_time(int now_phase)
		{
			double change_time = 0.0;
			//�����v�����ʂ��~���Ă�����(���҂̒�~�t���O���P)�CNO_CHANGE
			if (predicted_user->check_pause_flag() && real_user->check_pause_flag()) {
				return std::make_pair(NO_CHANGE, change_time);
			}
			else if(predicted_user->check_pause_flag() || real_user->check_pause_flag()){
				//����real�̂ق�����~�t���O��0��(�o�����Ă���)�Cpredict��1�Ȃ�,�\���葁���o��
				if (predicted_user->check_pause_flag() == true && real_user->check_pause_flag() == false) {
					//change_time�̍��������߂�
					change_time -= requirement->service_interval;
					return std::make_pair(SHORTER_PAUSE_TIME, change_time);
				}
				else {
					//change_time�̍��������߂�
					change_time += requirement->service_interval;
					return std::make_pair(LONGER_PAUSE_TIME, change_time);
				}
			}
			//�����t���O��0�Ȃ�(���x�ύX����W�����v���Ĕ��ł����ꍇ)�C���x�ύX�łȂ��Crest_time������������m
			else {
				//�ړ�������real�̂ق����傫���Ȃ�
				if (true) {
					//change_time�̍��������߂�
					return std::make_pair(SHORTER_PAUSE_TIME, change_time);
				}
				else {
					//change_time�̍��������߂�
					return std::make_pair(LONGER_PAUSE_TIME, change_time);
				}
			}
		}
	

		///<summary>
		/// ���[�U�̍s���v�����ύX�̔��f
		/// real_user��predicted_user�̈Ⴂ�Ō���
		/// ���x�̃`�F�b�N
		///</summary>
		std::pair<KatoMasterMethod::ChangeParameter, double> KatoMasterMethod::check_user_speed(int now_phase)
		{
			Geography::LatLng previous_real_user_position = *real_user->read_node_pos_info_of_phase(now_phase - 1).second;
			Geography::LatLng previous_predicted_user_position = *predicted_user->read_node_pos_info_of_phase(now_phase - 1).second;
			Geography::LatLng now_real_user_position = *real_user->read_node_pos_info_of_phase(now_phase).second;
			Geography::LatLng now_predicted_user_position = *predicted_user->read_node_pos_info_of_phase(now_phase).second;
			
			double change_time = 0.0;
			//���������ꏊ�ɂ�����CNO_CHANGE
			if (now_predicted_user_position == now_real_user_position) {
				return std::make_pair(NO_CHANGE, change_time);
			}
			else {
				if (/*����rest_time�����l�����āC�t�Z���āC���x�������Ȃ�*/true) {
					return check_user_pause_time(now_phase);
				}
				else {
					if (/*�ړ�������real�̂ق����傫���Ȃ�*/true) {
						//change_time�̍��������߂�
						return std::make_pair(SLOER_SPEED, change_time);
					}
					else {
						//change_time�̍��������߂�
						return std::make_pair(FASTER_SPEED, change_time);
					}
				}
			}
		}

		///<summary>
		/// ���[�U�̍s���v�����ύX�̔��f
		/// real_user��predicted_user�̈Ⴂ�Ō���
		/// path�̃`�F�b�N
		///</summary>
		std::pair<KatoMasterMethod::ChangeParameter, double> KatoMasterMethod::check_user_path(int now_phase)
		{
			double change_time = 0.0;
			//change_time�����߂�
			return std::make_pair(PATH, change_time);
		}


		///<summary>
		/// ���[�U�̍s���v�����ύX�̔��f
		/// real_user��predicted_user�̈Ⴂ�Ō���
		/// ��~�ʒu�̃`�F�b�N
		///</summary>
		std::pair<KatoMasterMethod::ChangeParameter, double> KatoMasterMethod::check_user_position(int now_phase)
		{
			return std::make_pair(POSITION, 0.0);
		}

		

	///<summary>
	/// ���[�U�̎��̒�~�n�_�̓������Ԃ�\������
	///</summary>
	void KatoMasterMethod::update_user_plan(std::pair<ChangeParameter, double> check_parameter, int phase_id)
	{
		
	}

	///<summary>
	/// ���[�U�̒�~���Ԃ̏C��
	///</summary>
	void KatoMasterMethod::update_user_pause_time(std::pair<ChangeParameter, double> check_parameter, int phase_id)
	{
		/*
		if (check_parameter.first == SHORTER_PAUSE_TIME) {
			//���O�܂Œ�~���Ă�����~�n�_����C
			//���ۂ̃��[�U�̈ʒu�܂ŁC�s���v�����ʂ�̈ړ����x�ƈړ��o�H�ňړ��������̂Ƃ݂Ȃ��C�ړ����Ԃ����߁C���ۂ̒�~���Ԃ��t�Z���ċ��߂�D
		}
		else() {

		}
		Geography::LatLng previous_real_user_position = *real_user->read_node_pos_info_of_phase(phase_id - 1).second;
		Geography::LatLng previous_predicted_user_position = *predicted_user->read_node_pos_info_of_phase(phase_id - 1).second;
		Geography::LatLng now_real_user_position = *real_user->read_node_pos_info_of_phase(phase_id).second;
		Geography::LatLng now_predicted_user_position = *predicted_user->read_node_pos_info_of_phase(phase_id).second;

		//user���v������蒷���~�܂��Ă���ꍇ
		//��predicted_user������phase�܂Œ�~���Ă���Ƃ���
		if (previous_real_user_position == previous_predicted_user_position) {
			//predicted_user->get_trajectory();
			//trajectory���C������D

		}
		else {
			
		}
		*/
	}

	///<summary>
	/// ���[�U�̃p�X�̏C��
	///</summary>
	void KatoMasterMethod::update_user_path(std::pair<ChangeParameter, double> check_parameter, int phase_id)
	{

	}

	///<summary>
	/// ���[�U�̈ړ����x�̏C��
	///</summary>
	void KatoMasterMethod::update_user_speed(std::pair<ChangeParameter, double> check_parameter, int phase_id)
	{

	}

	///<summary>
	/// ���[�U�̒�~�ʒu�̏C��
	///</summary>
	void KatoMasterMethod::update_user_pause_position(std::pair<ChangeParameter, double> check_parameter, int phase_id)
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

		//�O�̒l�̕ێ�
		double previous_pause_time = revising_dummy->get_pause_time(phase_id);
		double previous_arrive_time = time_manager->time_of_phase(phase_id + 1);

		if (std::abs(changes_in_arrival_time) > requirement->max_variation_of_pause_time)
		{
			double new_pause_time = changes_in_arrival_time > 0 ? previous_pause_time + requirement->max_variation_of_pause_time : previous_pause_time - requirement->max_variation_of_pause_time;
			revising_dummy->set_pause_time(phase_id, new_pause_time);
		}
		else
		{
			revising_dummy->set_pause_time(phase_id, previous_pause_time + changes_in_arrival_time);
		}

		if (requirement->max_pause_time < time_manager->time_of_phase(phase_id)) revising_dummy->set_pause_time(phase_id, requirement->max_pause_time);
		if (requirement->min_pause_time > time_manager->time_of_phase(phase_id)) revising_dummy->set_pause_time(phase_id, requirement->min_pause_time);

		//��~���Ԃ̕ω��ʂ����߂�
		int variation_of_pause_time = revising_dummy->get_pause_time(phase_id) - previous_pause_time;

		for (int i = phase_id + 1; i <= time_manager->phase_count(); i++)
		{
			//revise_time_manager->time_of_phase(i) += variation_of_pause_time;
		}
		if (time_manager->time_of_phase(phase_id + 1) == previous_arrive_time + changes_in_arrival_time) return;

		changes_in_arrival_time -= variation_of_pause_time;


	}


	///<summary>
	/// �_�~�[�̈ړ��o�H�̏C��
	///</summary>
	void KatoMasterMethod::revise_dummy_path(int phase_id)
	{
		/*
		time_t previous_arrive_time = time_manager->time_of_phase(phase_id + 1);
		time_t tempT = 10000000000000000000;

		while (�S�Ẵg���W�F�N�g�����`�F�b�N���I����܂�)
		{
		phase_id�̂Ƃ��̑��ݒn�_;
		time_manager->time_of_phase(phase_id + 1) = time_manager->time_of_phase(phase_id) + revising_dummy->get_pause_time(phase_id) + distance_of_Tri / revising_dummy->get_speed(phase_id);

		}
		*/
	}


	///<summary>
	/// �_�~�[�̍s�����x�̏C��
	///</summary>
	void KatoMasterMethod::revise_dummy_speed(int phase_id)
	{

		int previous_arrive_time = time_manager->time_of_phase(phase_id + 1);
		double previous_speed = revising_dummy->get_speed(phase_id);
		double distance = map->shortest_distance(revising_dummy->read_node_pos_info_of_phase(phase_id).first, revising_dummy->read_node_pos_info_of_phase(phase_id).first);
		int time = distance / (time_manager->time_of_phase(phase_id + 1) + changes_in_arrival_time - time_manager->time_of_phase(phase_id));
		revising_dummy->set_speed(phase_id, time);

		if (std::abs(revising_dummy->get_speed(phase_id) - previous_speed) > requirement->max_variation_of_speed)
		{
			double new_speed = revising_dummy->get_speed(phase_id) - previous_speed > 0 ? previous_speed + requirement->max_variation_of_speed : previous_speed - requirement->max_variation_of_speed;
			revising_dummy->set_pause_time(phase_id, new_speed);
		}
		double max_speed = requirement->average_speed_of_dummy + 0.5* requirement->speed_range_of_dummy;
		double min_speed = requirement->average_speed_of_dummy - 0.5* requirement->speed_range_of_dummy;
		if (max_speed < revising_dummy->get_speed(phase_id)) revising_dummy->set_speed(phase_id, max_speed);
		if (min_speed > revising_dummy->get_speed(phase_id)) revising_dummy->set_speed(phase_id, min_speed);

		//time_manager->time_of_phase(phase_id + 1) = time_manager->time_of_phase(phase_id) + revising_dummy->get_pause_time(phase_id) + (time_t)(distance / revising_dummy->get_speed(phase_id));
		time_t variation_of_arrive_time = time_manager->time_of_phase(phase_id + 1) - previous_arrive_time;

		for (int i = phase_id + 1; i <= time_manager->phase_count(); i++)
		{
			//revise_time_manager->time_of_phase(i) += variation_of_pause_time;
		}
		if (time_manager->time_of_phase(phase_id + 1) == previous_arrive_time + changes_in_arrival_time) return;

	}


	///<summary>
	/// �_�~�[�̒�~�ʒu�̏C��
	///</summary>
	void KatoMasterMethod::revise_dummy_pause_position(int phase_id)
	{

	}




	///<summary>
	/// ������
	///</summary>
	void KatoMasterMethod::initialize()
	{
		//���[�U�̓����̕ύX���V�������D
		predicted_user = make_predicted_user_plan(entities->get_user());
	}


	///<summary>
	/// ��������Ď�@�̊j�ɂȂ镔��.�_�~�[�̍s�����C������
	///</summary>
	void KatoMasterMethod::revise_dummy_positions()
	{
		for (size_t dummy_id = 1; dummy_id <= entities->get_dummy_count(); dummy_id++)
		{
			revising_dummy = entities->get_dummy_by_id(dummy_id);
			for (int phase_id = 0; phase_id < time_manager->phase_count(); phase_id++)
			{
				if (check_going_pause_position_in_plan()) {
					if (check_user_plan(phase_id).second != 0.0) {
						update_user_plan(check_user_plan(phase_id), phase_id);//���̒�~�n�_�̓������Ԃ�\��
						revise_dummy_movement_plan(phase_id);//dummy�̍s���v�����̍X�V
						//update_user_plan();//���[�U�̍s���v�������X�V
					}
				}
			}
		}
	}

	/*
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
	*/

	void KatoMasterMethod::run()
	{
		//�����Ŏ��s���Ԃ̌v�����J�n
		timer->start();

		//������.�s���ύX��̃��[�U�̐���
		initialize();

		//���������s����(�e�����̃_�~�[�ʒu���v�Z����)(�������񑲘_��@)[Kato 13]
		decide_dummy_positions();

		//�����Ń��[�U�̍s���̗\����_�~�[�̍s�����C������(��������C�_��@)[Kato 14]
		revise_dummy_positions();

		//�����Ōv�����I��
		timer->end();

		//�I������
		terminate();
	}

}