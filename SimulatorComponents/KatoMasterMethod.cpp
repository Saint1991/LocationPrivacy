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
		//���[�U��2���Ă�悤�ɁC�f�[�^�\�����C������Dinput_user�̕���get_user()�ł͂Ȃ��̂ŁC����
		input_user = entities->get_user();
		real_user = entities->get_user();
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
	std::shared_ptr<Entity::PauseMobileEntity<Geography::LatLng>> KatoMasterMethod::copy_predicted_user_plan(std::shared_ptr<Entity::PauseMobileEntity<Geography::LatLng>> input_user)
	{
		//std::shared_ptr<std::vector<std::shared_ptr<Geography::LatLng>>> input_user_positions = input_user->read_trajectory()->read_positions();
		std::shared_ptr<std::vector<std::shared_ptr<Geography::LatLng>>> predicted_user_positions;

		//std::copy(input_user_positions->begin(), input_user_positions->end(), predicted_user_positions->begin());
		
		return predicted_user;
	}


	///<summary>
	/// ���x�̍ő�ω��l�ƍŏ��ω��l���v�Z����
	///</summary>
	std::pair<double, double> KatoMasterMethod::calc_max_variable_speed(double speed)
	{
		double max_speed = requirement->average_speed_of_dummy + requirement->speed_range_of_dummy;
		double min_speed = requirement->average_speed_of_dummy - requirement->speed_range_of_dummy;

		return std::make_pair(requirement->max_variation_of_speed*(max_speed - speed), requirement->max_variation_of_speed*(speed - min_speed));
	}

	///<summary>
	/// ��~���Ԃ̍ő�ω��l�ƍŏ��ω��l���v�Z����
	///</summary>
	std::pair<double, double> KatoMasterMethod::calc_max_variable_pause_time(double pause_time)
	{
		return std::make_pair(requirement->max_variation_of_pause_time*(requirement->max_pause_time - pause_time), requirement->max_variation_of_pause_time*(pause_time - requirement->min_pause_time));
	}


		
	///<summary>
	/// ���[�U�̍s���v�����Ɋ܂܂���~�n�_�Ɍ������Ă��邩�ǂ������`�F�b�N
	///</summary>
	bool KatoMasterMethod::check_going_same_poi_as_plan()
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
			return std::make_pair(VISIT_POI, 0.0);
		}

		

	///<summary>
	/// ���[�U�̎��̒�~�n�_�̓������Ԃ�\������
	///</summary>
	void KatoMasterMethod::update_user_plan(std::pair<ChangeParameter, double> check_parameter, int phase_id)
	{
		switch (check_parameter.first) {
		case LONGER_PAUSE_TIME:
			modification_of_user_trajectory_when_LONGER_PAUSE_TIME(check_parameter, phase_id);
			break;
		case SHORTER_PAUSE_TIME:
			modification_of_user_trajectory_when_SHORTER_PAUSE_TIME(check_parameter, phase_id);
			break;
		case PATH:
			modification_of_user_trajectory_when_PATH(check_parameter, phase_id);
			break;
		case FASTER_SPEED:
			modification_of_user_trajectory_when_FASTER_SPEED(check_parameter, phase_id);
			break;
		case SLOER_SPEED:
			modification_of_user_trajectory_when_SLOER_SPEED(check_parameter, phase_id);
			break;
		case VISIT_POI:
			modification_of_user_trajectory_when_VISIT_POI(check_parameter, phase_id);
			break;
		}
	}

	///<summary>
	/// ���[�U�̒�~���Ԃ��\���蒷������predicted_user�̏C��
	///</summary>
	void KatoMasterMethod::modification_of_user_trajectory_when_LONGER_PAUSE_TIME(std::pair<ChangeParameter, double> check_parameter, int phase_id)
	{
		
	}


	///<summary>
	/// ���[�U�̒�~���Ԃ��\����Z������predicted_user�̏C��
	///</summary>
	void KatoMasterMethod::modification_of_user_trajectory_when_SHORTER_PAUSE_TIME(std::pair<ChangeParameter, double> check_parameter, int phase_id)
	{

	}

	///<summary>
	/// ���[�U�̃p�X�̏C��
	///</summary>
	void KatoMasterMethod::modification_of_user_trajectory_when_PATH(std::pair<ChangeParameter, double> check_parameter, int phase_id)
	{

	}

	///<summary>
	/// ���[�U�̈ړ����x���\���葁�����̏C��
	///</summary>
	void KatoMasterMethod::modification_of_user_trajectory_when_FASTER_SPEED(std::pair<ChangeParameter, double> check_parameter, int phase_id)
	{
		double real_user_speed = check_parameter.second;

	}

	///<summary>
	/// ���[�U�̈ړ����x���\����x�����̏C��
	///</summary>
	void KatoMasterMethod::modification_of_user_trajectory_when_SLOER_SPEED(std::pair<ChangeParameter, double> check_parameter, int phase_id)
	{

	}


	///<summary>
	/// ���[�U�̒�~�ʒu�̏C��
	///</summary>
	void KatoMasterMethod::modification_of_user_trajectory_when_VISIT_POI(std::pair<ChangeParameter, double> check_parameter, int phase_id)
	{

	}



	///<summary>
	/// �_�~�[�̍s���v�������C������
	///</summary>
	void KatoMasterMethod::revise_dummy_trajectory(int phase_id)
	{
		//poi�Ɏ~�܂��Ă��鎞
		if (revising_dummy->check_pause_flag()) {
			//�S�Ă̒�~�n�_�̓������Ԃ�ύX���CTu���ύX������D
			revise_dummy_pause_time(phase_id);
			revise_dummy_path(phase_id);
			revise_dummy_speed(phase_id);
			if (changes_in_arrival_time != 0) revise_dummy_visit_poi(phase_id);
			
		}
		//���݈ړ����ŁC�v�����Ɠ���poi�Ɍ������Ă���ꍇ�C�������Ă���poi�ɒ�~�����Ƃ��čl����
		else {
			//�S�Ă̒�~�n�_�̓������Ԃ�ύX���CTu���ύX������D
			revise_dummy_pause_time(phase_id);
			revise_dummy_path(phase_id);
			revise_dummy_speed(phase_id);
			if (changes_in_arrival_time != 0) revise_dummy_visit_poi(phase_id);

		}
	}


	///<summary>
	/// �_�~�[�̒�~���Ԃ̏C��
	///</summary>
	void KatoMasterMethod::revise_dummy_pause_time(int phase_id)
	{
		//�O�̒l�̕ێ�
		int pause_phase = revising_dummy->get_pause_phase(phase_id);
		double pause_time = revising_dummy->get_pause_time(pause_phase);
		double next_arrive_time = time_manager->time_of_phase(pause_phase);

		double max_variable_value = calc_max_variable_pause_time(pause_phase).first;
		double min_variable_value = calc_max_variable_pause_time(pause_phase).second;
		double new_pause_time = changes_in_arrival_time;

		//�C���� �� �ő�ω��ʁ@�𖞂����C���C�C���� < �ő��~���ԂƂ���D
		if (changes_in_arrival_time > 0) {
			if (changes_in_arrival_time > max_variable_value) new_pause_time = max_variable_value;
			if (max_variable_value > requirement->max_pause_time) new_pause_time = requirement->max_pause_time;
		}
		else {
			if (changes_in_arrival_time < min_variable_value) new_pause_time = min_variable_value;
			if (min_variable_value < requirement->min_pause_time) new_pause_time = requirement->min_pause_time;
		}
		revising_dummy->set_pause_time(pause_phase, pause_time + new_pause_time);
		
		//�����������C�_�~�[�̒�~���Ԃ��C������D
		for (int i = phase_id + 1; i <= time_manager->phase_count(); i++)
		{
			//�����Ɏ��̖K��POI�̓������ԂƏo���������l�����āC�C������֐��������_���ŋL�q�D		
		}
		if (time_manager->time_of_phase(phase_id + 1) == next_arrive_time + changes_in_arrival_time) return;
		
		//��������������Ȃ�������C����path�̒���������
		changes_in_arrival_time -= new_pause_time;
	}


	///<summary>
	/// �_�~�[�̈ړ��o�H�̏C��
	///</summary>
	void KatoMasterMethod::revise_dummy_path(int phase_id)
	{
		
	}


	///<summary>
	/// �_�~�[�̍s�����x�̏C��
	///</summary>
	void KatoMasterMethod::revise_dummy_speed(int phase_id)
	{
		//�O�̒l�̕ێ�
		int next_pause_phase = revising_dummy->get_pause_phase(phase_id);
		int next_next_pause_phase = revising_dummy->get_pause_phase(next_pause_phase);
		double next_departing_speed = revising_dummy->get_speed(next_pause_phase);
		double next_arrive_time = time_manager->time_of_phase(next_pause_phase);
		
		double distance = map->shortest_distance(revising_dummy->read_node_pos_info_of_phase(next_pause_phase).first, revising_dummy->read_node_pos_info_of_phase(next_next_pause_phase).first);
		
		double new_speed = distance / (time_manager->time_of_phase(next_next_pause_phase) + changes_in_arrival_time - time_manager->time_of_phase(next_pause_phase));
		revising_dummy->set_speed(next_pause_phase, new_speed);

		if (std::abs(revising_dummy->get_speed(next_pause_phase) - next_departing_speed) > requirement->max_variation_of_speed)
		{
			new_speed = revising_dummy->get_speed(next_pause_phase) - next_departing_speed > 0 ? next_departing_speed + requirement->max_variation_of_speed : next_departing_speed - requirement->max_variation_of_speed;
			revising_dummy->set_pause_time(next_pause_phase, new_speed);
		}
		double max_speed = requirement->average_speed_of_dummy + 0.5 * requirement->speed_range_of_dummy;
		double min_speed = requirement->average_speed_of_dummy - 0.5 * requirement->speed_range_of_dummy;
		if (max_speed < revising_dummy->get_speed(next_pause_phase)) revising_dummy->set_speed(next_pause_phase, max_speed);
		if (min_speed > revising_dummy->get_speed(next_pause_phase)) revising_dummy->set_speed(next_pause_phase, min_speed);

		//time_manager->time_of_phase(next_next_pause_phase) = time_manager->time_of_phase(phase_id) + revising_dummy->get_pause_time(phase_id) + (time_t)(distance / revising_dummy->get_speed(phase_id));
		double variation_of_arrive_time = time_manager->time_of_phase(next_next_pause_phase) - next_arrive_time;

		for (int i = phase_id + 1; i <= time_manager->phase_count(); i++)
		{
			//�����ɁC���x���ω������ۂ̃_�~�[�̍s���C�����L�q
		}
		if (time_manager->time_of_phase(phase_id + 1) == next_arrive_time + changes_in_arrival_time) return;

	}


	///<summary>
	/// �_�~�[�̒�~�ʒu�̏C��
	///</summary>
	void KatoMasterMethod::revise_dummy_visit_poi(int phase_id)
	{

	}
	

	///<summary>
	/// ������
	///</summary>
	void KatoMasterMethod::initialize()
	{
		//���[�U�̓����̕ύX���V�������D
		predicted_user = copy_predicted_user_plan(entities->get_user());
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
				if (check_going_same_poi_as_plan()) {
					if (check_user_plan(phase_id).second != 0.0) {
						revise_dummy_trajectory(phase_id);//dummy�̍s���v�����̍X�V
						update_user_plan(check_user_plan(phase_id), phase_id);//���̒�~�n�_�̓������Ԃ�\�����C���[�U�̍s���v�������X�V
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