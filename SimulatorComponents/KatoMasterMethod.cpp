#include "stdafx.h"
#include "KatoMasterMethod.h"

namespace Method
{

	///<summary>
	/// �R���X�g���N�^
	/// �����Simulator�ō쐬�����e����͂ւ̎Q�Ƃ�n��
	///</summary>
	KatoMasterMethod::KatoMasterMethod(std::shared_ptr<Map::HayashidaDbMap const> map, std::shared_ptr<Entity::DifferentMovementUser<Geography::LatLng>> user, std::shared_ptr<Requirement::KatoMethodRequirement const> requirement, std::shared_ptr<Time::TimeSlotManager> time_manager)
		: KatoBachelorMethod(map, user, requirement, time_manager),Tu(0.0)
	{
		//input_user��real_user�̐����D����2�͕ύX���Ȃ�
		input_user = entities->get_user()->get_predicted_user();
		real_user = entities->get_user()->get_real_user();

		//predicted_user��ύX�����Ă���
		predicted_user = entities->get_user()->get_predicted_user();
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
	std::shared_ptr<Entity::RevisablePauseMobileEntity<Geography::LatLng>> KatoMasterMethod::copy_predicted_user_plan(std::shared_ptr<Entity::PauseMobileEntity<Geography::LatLng>> input_user)
	{
	
		return predicted_user;
	}


	///<summary>
	/// ���x�̍ő�ω��l�ƍŏ��ω��l���v�Z����
	/// pair(Max, min)
	///</summary>
	std::pair<double, double> KatoMasterMethod::calc_max_variable_speed(double speed)
	{
		double max_speed = requirement->average_speed_of_dummy + requirement->speed_range_of_dummy;
		double min_speed = requirement->average_speed_of_dummy - requirement->speed_range_of_dummy;

		return std::make_pair(MAX_VARIATION_OF_SPEED*(max_speed - speed), MAX_VARIATION_OF_SPEED*(speed - min_speed));
	}

	///<summary>
	/// ��~���Ԃ̍ő�ω��l�ƍŏ��ω��l���v�Z����
	/// pair(Max, min)
	///</summary>
	std::pair<double, double> KatoMasterMethod::calc_max_variable_pause_time(double pause_time)
	{
		return std::make_pair(MAX_VARIATION_OF_PAUSE_TIME*(requirement->max_pause_time - pause_time), MAX_VARIATION_OF_PAUSE_TIME*(pause_time - requirement->min_pause_time));
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
	KatoMasterMethod::ChangeParameter KatoMasterMethod::check_user_plan(int now_phase)
	{
		//�~�܂��Ă��遨��~���Ԃ̕ύXor��~�n�_�̕ύX
		//�����Е��ł���~���Ă����灨��~���ԕύX�̔��f
		if (predicted_user->isPause(now_phase) || real_user->isPause(now_phase)) {
			return check_user_pause_time(now_phase);
		}
		//�����Ă�����C
		else {
			//�������`��Ԃ������̏�Ɉʒu���Ă�����
			if (is_on_the_path(now_phase)) {
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
		KatoMasterMethod::ChangeParameter KatoMasterMethod::check_user_pause_time(int now_phase)
		{
			//�����v�����ʂ��~���Ă�����CNO_CHANGE
			if (predicted_user->isPause(now_phase) && real_user->isPause(now_phase)) {
				return NO_CHANGE;
			}
			else if (predicted_user->isPause(now_phase) || real_user->isPause(now_phase)) {
				//����real�̂ق�����~�t���O��0��(�o�����Ă���)�Cpredict��1�Ȃ�,�\���葁���o��
				if (predicted_user->isPause(now_phase) == true && real_user->isPause(now_phase) == false) {
					//change_time�̍��������߂�
					Tu -= requirement->service_interval;
					return SHORTER_PAUSE_TIME;
				}
				else {
					//change_time�̍��������߂�
					Tu += requirement->service_interval;
					return LONGER_PAUSE_TIME;
				}
			}
			//�����t���O��0�Ȃ�(���x�ύX����W�����v���Ĕ��ł����ꍇ)�C���x�ύX�łȂ��Crest_time������������m
			else{
				throw std::invalid_argument("This situation can't happen!!");
				if (now_phase == 0) throw std::invalid_argument(" It must set pause time at init visited POI !! ");
				double real_user_dist = Geography::GeoCalculation::lambert_distance(*real_user->read_position_of_phase(now_phase - 1), *real_user->read_position_of_phase(now_phase));
				double predicted_user_dist = Geography::GeoCalculation::lambert_distance(*predicted_user->read_position_of_phase(now_phase - 1), *predicted_user->read_position_of_phase(now_phase));
				//�ړ�������real�̂ق����傫���Ȃ�
				if (real_user_dist > predicted_user_dist) {
					//change_time�̍��������߂�
					Tu -= (real_user_dist - predicted_user_dist) / predicted_user->get_starting_speed();
					return SHORTER_PAUSE_TIME;
				}
				else {
					//change_time�̍��������߂�
					Tu -= (predicted_user_dist - real_user_dist) / predicted_user->get_starting_speed();
					return LONGER_PAUSE_TIME;
				}
			}
		}
	

		///<summary>
		/// ���[�U�̍s���v�����ύX�̔��f
		/// real_user��predicted_user�̈Ⴂ�Ō���
		/// ���x�̃`�F�b�N
		///</summary>
		KatoMasterMethod::ChangeParameter KatoMasterMethod::check_user_speed(int now_phase)
		{
			Geography::LatLng previous_real_user_position = *real_user->read_node_pos_info_of_phase(now_phase - 1).second;
			Geography::LatLng previous_predicted_user_position = *predicted_user->read_node_pos_info_of_phase(now_phase - 1).second;
			Geography::LatLng now_real_user_position = *real_user->read_node_pos_info_of_phase(now_phase).second;
			Geography::LatLng now_predicted_user_position = *predicted_user->read_node_pos_info_of_phase(now_phase).second;

			//���������C�����ꏊ�ɂ�����CNO_CHANGE
			if (now_predicted_user_position == now_real_user_position) return NO_CHANGE;

			//���ۂɂ͍����Ōv�Z����K�v�����邪�C������͒l���Q�Ƃ��Ă����`
			double real_user_speed = real_user->get_now_speed(now_phase);
			double predicted_user_speed = predicted_user->get_now_speed(now_phase);

			if (real_user_speed == predicted_user_speed) {
				return check_user_pause_time(now_phase);
			}
			else {
				//real�̕������x���傫���ꍇ
				if (real_user_speed > predicted_user_speed) {
					double change_time =
						map->calc_necessary_time(real_user->read_node_pos_info_of_phase(now_phase - 1).first, real_user->read_node_pos_info_of_phase(real_user->get_next_poi_arrive_phase_when_moving(now_phase)).first, real_user_speed)
						- map->calc_necessary_time(predicted_user->read_node_pos_info_of_phase(now_phase - 1).first, predicted_user->read_node_pos_info_of_phase(predicted_user->get_next_poi_arrive_phase_when_moving(now_phase)).first, predicted_user_speed);
					Tu += change_time;
					return FASTER_SPEED;
				}
				//predicted�̕������x���傫���ꍇ
				else {
					double change_time =
						map->calc_necessary_time(predicted_user->read_node_pos_info_of_phase(now_phase - 1).first, predicted_user->read_node_pos_info_of_phase(predicted_user->get_next_poi_arrive_phase_when_moving(now_phase)).first, predicted_user_speed)
						- map->calc_necessary_time(real_user->read_node_pos_info_of_phase(now_phase - 1).first, real_user->read_node_pos_info_of_phase(real_user->get_next_poi_arrive_phase_when_moving(now_phase)).first, real_user_speed);
						Tu += change_time;
					return SLOER_SPEED;
				}
			}
		}
		

		///<summary>
		/// ���[�U�̍s���v�����ύX�̔��f
		/// real_user��predicted_user�̈Ⴂ�Ō���
		/// path�̃`�F�b�N
		///</summary>
		KatoMasterMethod::ChangeParameter KatoMasterMethod::check_user_path(int now_phase)
		{
			//change_time�����߂�
			//Tu = 
			return PATH;
		}


		///<summary>
		/// ���[�U�̍s���v�����ύX�̔��f
		/// real_user��predicted_user�̈Ⴂ�Ō���
		/// ��~�ʒu�̃`�F�b�N
		///</summary>
		KatoMasterMethod::ChangeParameter KatoMasterMethod::check_user_position(int now_phase)
		{
			return VISIT_POI;
		}

		///<summary>
		/// �_�~�[���p�X��ɑ��݂��邩�ǂ������`�F�b�N
		/// predicted_user�̌���~POI���玟�̒�~POI�܂ł̌o�H������Ă��āC���̒�����ɏ���Ă��邩�ǂ������`�F�b�N
		///</summary>
		bool KatoMasterMethod::is_on_the_path(int phase_id)
		{
			Graph::MapNodeIndicator source;
			Graph::MapNodeIndicator destination;

			std::shared_ptr<std::vector<std::shared_ptr<Geography::LatLng>>> path_between_pois = predicted_user->get_trajectory()->get_positions();
			std::vector<std::shared_ptr<Geography::LatLng>>::iterator path_iter = path_between_pois->begin();//path���������邽�߂�index
						
			Graph::MapNodeIndicator nearest_position = source;

			return true;
		}



	///<summary>
	/// ���[�U�̎��̒�~�n�_�̓������Ԃ�\������
	///</summary>
	void KatoMasterMethod::update_user_plan(ChangeParameter check_parameter, int phase_id)
	{
		switch (check_parameter) {
		case LONGER_PAUSE_TIME:
			revise_user_trajectory_when_LONGER_PAUSE_TIME(phase_id);
			break;
		case SHORTER_PAUSE_TIME:
			revise_user_trajectory_when_SHORTER_PAUSE_TIME(phase_id);
			break;

		case FASTER_SPEED:
			revise_user_trajectory_when_FASTER_SPEED(phase_id);
			break;
		case SLOER_SPEED:
			revise_user_trajectory_when_SLOER_SPEED(phase_id);
			break;

		case PATH:
			revise_user_trajectory_when_PATH(phase_id);
			break;
		case VISIT_POI:
			revise_user_trajectory_when_VISIT_POI(phase_id);
			break;
		}
	}

	///<summary>
	/// ���[�U�̒�~���Ԃ��\���蒷������predicted_user�̏C��
	///</summary>
	void KatoMasterMethod::revise_user_trajectory_when_LONGER_PAUSE_TIME(int phase_id)
	{
		//trajectory��speed,pause_time�����ꂼ�ꂸ�炷���ƂőΉ�
		predicted_user->get_trajectory()->insert_positions_to_trajectory(phase_id, 1);
		predicted_user->insert_speed_list_and_pause_time(phase_id, 1);		
	}


	///<summary>
	/// ���[�U�̒�~���Ԃ��\����Z������predicted_user�̏C��
	///</summary>
	void KatoMasterMethod::revise_user_trajectory_when_SHORTER_PAUSE_TIME(int phase_id)
	{
		//trajectory��speed,pause_time�����炷���ƂőΉ�
		predicted_user->get_trajectory()->delete_positions_to_trajectory(phase_id);
		predicted_user->delete_speed_list_and_pause_time(phase_id);
	}

	///<summary>
	/// ���[�U�̃p�X�̏C��
	///</summary>
	void KatoMasterMethod::revise_user_trajectory_when_PATH(int phase_id)
	{

	}

	///<summary>
	/// ���[�U�̈ړ����x���\���葁�����̏C��
	///</summary>
	void KatoMasterMethod::revise_user_trajectory_when_FASTER_SPEED(int phase_id)
	{
		double real_user_speed = real_user->get_now_speed(phase_id);

	}

	///<summary>
	/// ���[�U�̈ړ����x���\����x�����̏C��
	///</summary>
	void KatoMasterMethod::revise_user_trajectory_when_SLOER_SPEED(int phase_id)
	{
		revise_user_trajectory_when_FASTER_SPEED(phase_id);
	}


	///<summary>
	/// ���[�U�̒�~�ʒu�̏C��
	///</summary>
	void KatoMasterMethod::revise_user_trajectory_when_VISIT_POI(int phase_id)
	{

	}


	///<summary>
	/// �_�~�[�̍s���v�������C������
	///</summary>
	void KatoMasterMethod::revise_dummy_trajectory(int phase_id)
	{
		revise_dummy_pause_time(phase_id);
		if (Tu != 0.0) revise_dummy_path(phase_id);
		if (Tu != 0.0) revise_dummy_speed(phase_id);
		if (Tu != 0.0) revise_dummy_visit_poi(phase_id);

		/*		
		//��������̏C�_��@�̃A���S���Y���ł́C
		//poi�ɒ�~���Ă��鎞�ƁC���݈ړ����̎��͕ʂɂ��čl���Ă��邯�ǁC
		//visited_pois_info_list_id���擾���鎞�ɁC����2�͋�ʂ��Ă���̂ŁC
		//�ꍇ��������K�v�͂Ȃ�(�͂�)
		//�S�Ă̒�~�n�_�̓������Ԃ�ύX���CTu���ύX������D
		//visited_poi���̍X�V
		update_visited_pois_info_of_dummy();
		if (revising_dummy->isPause(phase_id)) {
			revise_dummy_pause_time(phase_id);
			if (Tu != 0.0) revise_dummy_path(phase_id);
			if (Tu != 0.0) revise_dummy_speed(phase_id);
			if (Tu != 0.0) revise_dummy_visit_poi(phase_id);
		}
		else {
			//�S�Ă̒�~�n�_�̓������Ԃ�ύX���CTu���ύX������D
			revise_dummy_pause_time(phase_id);
			if (Tu != 0.0)revise_dummy_path(phase_id);
			if (Tu != 0.0) revise_dummy_speed(phase_id);
			if (Tu != 0.0) revise_dummy_visit_poi(phase_id);
		}
		*/
		
	}


	///<summary>
	/// �_�~�[�̒�~���Ԃ̏C��
	/// ���e�͈͂܂ŏC�������݂�
	/// ��������������Ȃ�������C����path�̒���������
	///</summary>
	void KatoMasterMethod::revise_dummy_pause_time(int phase_id)
	{
		int changed_poi_num_id = 0;//��~���Ԃ��ύX���ꂽpoi�̐����L�^���邽�߂�id
		
		//�ύX�O��trajectory�ƃX�s�[�h���X�g�ƒ�~���ԃ��X�g��ێ�
		std::shared_ptr<Graph::RevisableTrajectory<>> prev_trajectory = revising_dummy->get_trajectory();
		std::vector<double> prev_now_speed_list = revising_dummy->get_now_speed_list();
		std::vector<double> prev_now_pause_time_list = revising_dummy->get_now_pause_time_list();

		//�C���Ώۂ̃t�F�[�Y���擾�D���ݒ�~�������݂̃t�F�[�Y�C�ړ��������̗\�蓞���t�F�[�Y
		int revise_phase = revising_dummy->isPause(phase_id) ? phase_id : revising_dummy->get_next_poi_arrive_phase_when_moving(phase_id);
		int next_poi_arrive_phase = revising_dummy->get_next_poi_arrive_phase_when_moving((revising_dummy->get_poi_last_pause_phase_when_pausing(revise_phase) + 1));

		//�v���O������g�݂₷���悤�ɁC�\�߁C�o�����x�ƒ�~���Ԃ𒊏o���Ă���
		std::vector<double> speed_list;
		int phase = revise_phase;
		phase = revising_dummy->get_poi_last_pause_phase_when_pausing(phase);
		while(phase != time_manager->last_phase()) {
			phase++;
			speed_list.push_back(revising_dummy->get_now_speed(phase));
			phase = revising_dummy->get_next_poi_arrive_phase_when_moving(phase);
			phase = revising_dummy->get_poi_last_pause_phase_when_pausing(phase);
		}

		std::vector<double> pause_time_list;
		int phase2 = time_manager->last_phase();
		phase2 = revising_dummy->get_poi_init_pause_phase_when_pausing(phase2);
		while (phase2 <= revise_phase) {
			phase2--;
			pause_time_list.push_back(revising_dummy->get_now_pause_time(phase2));
			phase2 = revising_dummy->get_prev_poi_depart_phase_when_moving(phase2);
			phase2 = revising_dummy->get_poi_init_pause_phase_when_pausing(phase2);
		}


		//�S��~���ԕ��C�_�~�[�̒�~���Ԃ��C������D
		for (int id = revise_phase; id < time_manager->last_phase(); changed_poi_num_id++)
		{
			int changed_poi_num = 0;

			//��~��or�������Ă���POI�̒�~���Ԃ��擾
			//����Cdest_time���l�����Ă��Ȃ�����C���ۂ̒�~���Ԃ��኱�Z���Ȃ��Ă���D
			double pause_time 
				= revising_dummy->isPause(phase_id) 
				? revising_dummy->get_now_pause_time(revising_dummy->get_poi_init_pause_phase_when_pausing(phase_id))
				: revising_dummy->get_now_pause_time(revising_dummy->get_next_poi_arrive_phase_when_moving(phase_id));
			
			//�ő�ω���
			double max_variable_value = calc_max_variable_pause_time(pause_time).first;
			double min_variable_value = calc_max_variable_pause_time(pause_time).second;

			//�ύX���ԁD�����l�́C�ύX��Tu
			double change_time = Tu;

			//�C���� �� �ő�ω��ʁ@�𖞂����C���C�C���� < �ő��~���ԂƂ���D
			if (Tu > 0) {
				if (Tu > max_variable_value) change_time = max_variable_value;
				if (max_variable_value + pause_time > requirement->max_pause_time) change_time = requirement->max_pause_time - pause_time;
			}
			else {
				if (std::abs(Tu) > min_variable_value) change_time = - min_variable_value;
				if (pause_time - min_variable_value < requirement->min_pause_time) change_time = requirement->min_pause_time - pause_time;
			}

			double new_pause_time = change_time + pause_time;
			
			//change_time�����݂̎c���~���ԂɋL�^
			revising_dummy->add_now_pause_time(revise_phase, change_time);
			
			recalculation_path(revising_dummy->get_trajectory()->get_visited_node_id(revise_phase), revising_dummy->get_trajectory()->get_visited_node_id(next_poi_arrive_phase), &revise_phase, speed_list.at(changed_poi_num), pause_time_list.at(changed_poi_num));

			Tu -= change_time;
			if (Tu == 0.0) break;
		}
				
		//�ύX����Ȃ���������prev_trajectory��copy������	
		//�����ʒu���R�s�[���邽�߂ɁC���̈ʒu�������t�F�[�Y���擾
		int same_phase = prev_trajectory->search_phase_same_id(revising_dummy->get_trajectory()->get_visited_node_id(revise_phase));
		int last_copied_phase = same_phase + revising_dummy->get_trajectory()->last_phase() - revise_phase;
		int laset_phase = prev_trajectory->get_positions()->size();
		//revising_dummy->get_trajectory()->copy_trajectory(revise_phase, laset_phase, same_phase, laset_phase, *prev_trajectory);
		
		//now_speed_list��now_pause_list�̃R�s�[���s��
		std::copy(prev_now_speed_list.begin() + same_phase, prev_now_speed_list.begin() + last_copied_phase, revising_dummy->get_now_speed_list().begin() + revise_phase);
		std::copy(prev_now_pause_time_list.begin() + same_phase, prev_now_pause_time_list.begin() + last_copied_phase, revising_dummy->get_now_pause_time_list().begin() + revise_phase);

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
		int changed_poi_num_id = 0;

		//�ύX�O��trajectory�ƃX�s�[�h���X�g�ƒ�~���ԃ��X�g��ێ�
		std::shared_ptr<Graph::RevisableTrajectory<>> prev_trajectory = revising_dummy->get_trajectory();
		std::vector<double> prev_now_speed_list = revising_dummy->get_now_speed_list();
		std::vector<double> prev_now_pause_time_list = revising_dummy->get_now_pause_time_list();

		//�C���Ώۂ̃t�F�[�Y���擾�D���ݒ�~�������݂̃t�F�[�Y�C�ړ��������̗\�蓞���t�F�[�Y
		int revise_phase = revising_dummy->isPause(phase_id) ? phase_id : revising_dummy->get_next_poi_arrive_phase_when_moving(phase_id);
		int next_poi_arrive_phase = revising_dummy->get_next_poi_arrive_phase_when_moving((revising_dummy->get_poi_last_pause_phase_when_pausing(revise_phase) + 1));

		//�v���O������g�݂₷���悤�ɁC�\�߁C�o�����x�ƒ�~���Ԃ𒊏o���Ă���
		std::vector<double> speed_list;
		int phase = revise_phase;
		phase = revising_dummy->get_poi_last_pause_phase_when_pausing(phase);
		while (phase != time_manager->last_phase()) {
			phase++;
			speed_list.push_back(revising_dummy->get_now_speed(phase));
			phase = revising_dummy->get_next_poi_arrive_phase_when_moving(phase);
			phase = revising_dummy->get_poi_last_pause_phase_when_pausing(phase);
		}

		std::vector<double> pause_time_list;
		int phase2 = time_manager->last_phase();
		phase2 = revising_dummy->get_poi_init_pause_phase_when_pausing(phase2);
		while (phase2 <= revise_phase) {
			phase2--;
			pause_time_list.push_back(revising_dummy->get_now_pause_time(phase2));
			phase2 = revising_dummy->get_prev_poi_depart_phase_when_moving(phase2);
			phase2 = revising_dummy->get_poi_init_pause_phase_when_pausing(phase2);
		}

		//�S��~���ԕ��C�_�~�[�̒�~���Ԃ��C������D
		for (int id = revise_phase; id < time_manager->last_phase(); changed_poi_num_id++)
		{
			double distance = map->shortest_distance(revising_dummy->get_trajectory()->get_visited_node_id(revise_phase), revising_dummy->get_trajectory()->get_visited_node_id(next_poi_arrive_phase));
			double time_between_visited_poi = time_manager->time_of_phase(next_poi_arrive_phase) - time_manager->time_of_phase(revise_phase);

			double new_speed = distance / (time_between_visited_poi + Tu);
			double check = revising_dummy->get_now_speed(next_poi_arrive_phase - 1);//prev�ƌ��ݑ��x�̊֌W�����`�F�b�N
			double prev_speed = revising_dummy->get_now_speed(next_poi_arrive_phase - 1);

			//���x�̕ω���
			double variable_speed = new_speed - prev_speed;

			//�ő�ω��ʁE�ŏ��ω���
			double max_variable_speed = calc_max_variable_speed(new_speed).first;
			double min_variable_speed = calc_max_variable_speed(new_speed).second;

			//�ő�ŏ����x
			double max_speed = requirement->average_speed_of_dummy + 0.5 * requirement->speed_range_of_dummy;
			double min_speed = requirement->average_speed_of_dummy - 0.5 * requirement->speed_range_of_dummy;

			//�ύX���x�D�����l�͕ύX��
			double change_speed = new_speed;

			//�C���� �� �ő�ω��ʁ@�𖞂����C���C�C���� < �ő呬�x�Ƃ���D
			if (variable_speed > 0) {
				if (variable_speed > max_variable_speed) change_speed = new_speed + max_variable_speed;
				if (change_speed > max_speed) change_speed = max_speed;
			}
			else {
				if (std::abs(variable_speed) < min_variable_speed) change_speed = new_speed + min_variable_speed;
				if (change_speed < min_speed) change_speed = min_speed;
			}
			//change_time�����݂̎c���~���ԂɋL�^
			recalculation_path(revising_dummy->get_trajectory()->get_visited_node_id(revise_phase), revising_dummy->get_trajectory()->get_visited_node_id(next_poi_arrive_phase), &revise_phase, change_speed, pause_time_list.at(changed_poi_num_id));

			change_speed -= new_speed;
			if (change_speed == 0.0) break;
		}

		//�ύX����Ȃ���������prev_trajectory��copy������	
		//�����ʒu���R�s�[���邽�߂ɁC���̈ʒu�������t�F�[�Y���擾
		int same_phase = prev_trajectory->search_phase_same_id(revising_dummy->get_trajectory()->get_visited_node_id(revise_phase));
		int last_copied_phase = same_phase + revising_dummy->get_trajectory()->last_phase() - revise_phase;
		int last_phase = prev_trajectory->get_positions()->size() - 1;
		//revising_dummy->get_trajectory()->copy_trajectory(revise_phase, last_phase, same_phase, last_phase, *prev_trajectory);

		//now_speed_list��now_pause_list�̃R�s�[���s��
		std::copy(prev_now_speed_list.begin() + same_phase, prev_now_speed_list.begin() + last_copied_phase, revising_dummy->get_now_speed_list().begin() + revise_phase);
		std::copy(prev_now_pause_time_list.begin() + same_phase, prev_now_pause_time_list.begin() + last_copied_phase, revising_dummy->get_now_pause_time_list().begin() + revise_phase);
	}


	///<summary>
	/// �_�~�[�̒�~�ʒu�̏C��
	///</summary>
	void KatoMasterMethod::revise_dummy_visit_poi(int phase_id)
	{

	}
	

	///<summary>
	/// �_�~�[�̒�~�ʒu�̏C��
	/// 1:���݂̎c���~���Ԃ���ɁC��~phase�𖄂߂�
	/// 2:path���Čv�Z(���ݒ�~���Ԃ�0�ŃZ�b�g���������Ƃɒ���)
	///</summary>
	void KatoMasterMethod::recalculation_path(const Graph::MapNodeIndicator& source, const Graph::MapNodeIndicator& destination, int *revise_phase, double speed, double next_pause_time)
	{
		double dest_time = 0.0;
		
		//���̏ꍇ�̑Ή�
		if (revising_dummy->get_now_pause_time(*revise_phase) < 0) {
			dest_time = -revising_dummy->get_now_pause_time(*revise_phase);
			revising_dummy->set_now_pause_time(*revise_phase, 0.0);
		}
		double rest_pause_time = revising_dummy->get_now_pause_time(*revise_phase);
		lldiv_t variable_of_converted_pause_time_to_phase = std::lldiv(rest_pause_time, requirement->service_interval);
				
		//����phase����Cnow_puase_time��0�ȉ��ɂȂ�܂ŁC��~����o�^
		for (int i = 0; i < variable_of_converted_pause_time_to_phase.quot; i++)
		{
			(*revise_phase)++;
			rest_pause_time -= (double)requirement->service_interval;
			revising_dummy->set_now_pause_time(*revise_phase, rest_pause_time);
			revising_dummy->set_position_of_phase(*revise_phase, source, map->get_static_poi(source.id())->data->get_position());
		}
		
		std::vector<Graph::MapNodeIndicator> shortests_path_between_pois = map->get_shortest_path(source, destination);
		std::vector<Graph::MapNodeIndicator>::iterator path_iter = shortests_path_between_pois.begin();//path���������邽�߂�index

		//source����̋���
		//�ŏ�������~���Ԃ�phase�Ɋ��Z�������̗]���time�Ƃ��C����ȊO��service_interval��time�Ƃ��āC���ݒn���狁�߂����n�_��distance���v�Z
		//���x��phase�Ŗ��߂�O���Q�Ƃ��Ȃ���΂Ȃ�Ȃ����Ƃɒ���
		double now_time = requirement->service_interval - revising_dummy->get_now_pause_time(*revise_phase) - dest_time;
		
		double total_time_from_source_to_destination = map->calc_necessary_time(source, destination, speed);
		Graph::MapNodeIndicator nearest_position = source;
		//path���쐬�D�ꏊ�͈�ԋ߂�intersection���m�Ő��`��Ԃ���DMapNodeIndicator��Type��INVALID�Ƃ���D
		while (now_time < total_time_from_source_to_destination)
		{
			//�ŏ��͒�~���Ԃ�phase�Ɋ��Z�����Ƃ��̗]�蕪��distance�Ƃ��āC�ŒZ�H�̒��ň�ԋ߂�intersection��T���C���`��Ԃ���D
			//double total_path_length = map->shortest_distance(source, *path_iter);
			while (now_time > map->calc_necessary_time(source, *path_iter, speed))
			{
				nearest_position = *path_iter;
				path_iter++;
			}

			double distance = now_time * speed;
			double distance_between_nearest_intersection_and_arrive_position = distance - map->shortest_distance(source, nearest_position);
			Geography::LatLng nearest_latlng
				= nearest_position.type() == Graph::NodeType::POI ? map->get_static_poi(nearest_position.id())->data->get_position() : *map->get_static_node(nearest_position.id())->data;
			Geography::LatLng next_nearest_latlang
				= (*path_iter).type() == Graph::NodeType::POI ? map->get_static_poi((*path_iter).id())->data->get_position() : *map->get_static_node((*path_iter).id())->data;
			double angle = Geography::GeoCalculation::lambert_azimuth_angle(nearest_latlng, next_nearest_latlang);

			Geography::LatLng arrive_position = Geography::GeoCalculation::calc_translated_point(nearest_latlng, distance_between_nearest_intersection_and_arrive_position, angle);

			if (*revise_phase == time_manager->phase_count() - 1) return;//�c���path�����߂鎞�̏I������
			//�ȉ��C��~�ʒu�Ɋւ������o�^
			(*revise_phase)++;
			revising_dummy->set_position_of_phase(*revise_phase, Graph::MapNodeIndicator(Graph::NodeType::OTHERS, Graph::NodeType::OTHERS), arrive_position);
			revising_dummy->set_now_speed(*revise_phase, speed);
			revising_dummy->set_now_pause_time(*revise_phase, 0.0);
			now_time += requirement->service_interval;
		}

		//destination�̂Ƃ���܂ŕ⊮�ł�����Crest_time��ێ����Ă����I
		double time_between_arrive_position_and_dest_position = now_time - total_time_from_source_to_destination;
		double dest_rest_time
			= time_between_arrive_position_and_dest_position == requirement->service_interval ? 0 : time_between_arrive_position_and_dest_position;
		
		//�ړI�n�̓o�^
		//�o�^���(����phase,dest_rest_time)���X�V
		(*revise_phase)++;
		revising_dummy->set_position_of_phase(*revise_phase, destination, map->get_static_poi(destination.id())->data->get_position());
		revising_dummy->set_now_pause_time(*revise_phase, next_pause_time);
	}

	///<summary>
	/// �_�~�[�̖K��POI�����X�V����
	/// pause_phases,
	///</summary>
	void KatoMasterMethod::update_visited_pois_info_of_dummy()
	{
		/*
		std::vector<Entity::VisitedPoiInfo> visited_poi_list = revising_dummy->get_visited_poi_info_list();
		int id = 0;
		int phase_id = 0;
		int num = 0;
		//pause_phases�̍X�V
		for (std::vector<double>::iterator iter = revising_dummy->get_now_pause_time_list().begin(); *iter != 0.0; iter++) {
			if (*iter != 0.0) {
				int start_phase = std::distance(revising_dummy->get_now_pause_time_list().begin(), iter);

			}
			revising_dummy->revise_pause_phases();
			if (*(iter + 1) == 0.0) {}

		}
		*/
	}

	///<summary>
	/// �_�~�[�̖K��POI�����X�V����
	///</summary>
	void KatoMasterMethod::increment_visited_pois_info_list_id_of_users(int phase_id)
	{
		if ((phase_id - 1) == input_user->get_pause_phases().back()) input_user->increment_visited_pois_info_list_id();
		if ((phase_id - 1) == real_user->get_pause_phases().back()) real_user->increment_visited_pois_info_list_id();
		if ((phase_id - 1) == predicted_user->get_pause_phases().back()) predicted_user->increment_visited_pois_info_list_id();
	}


	///<summary>
	/// �_�~�[�̖K��POI�����X�V����
	///</summary>
	void KatoMasterMethod::clear_visited_pois_info_list_id_of_users()
	{
		input_user->clear_visited_pois_info_list_id();
		real_user->clear_visited_pois_info_list_id();
		predicted_user->clear_visited_pois_info_list_id();
	}
	

	///<summary>
	/// ������
	///</summary>
	void KatoMasterMethod::initialize()
	{
	}


	///<summary>
	/// ��������Ď�@�̊j�ɂȂ镔��.�_�~�[�̍s�����C������
	///</summary>
	void KatoMasterMethod::revise_dummy_positions(int phase_id, entity_id dummy_id)
	{
		//���[�U�̍s������y�у_�~�[�̏C��
		if (check_going_same_poi_as_plan()) {
			Method::KatoMasterMethod::ChangeParameter parmeter = check_user_plan(phase_id);
			if (parmeter != NO_CHANGE) {
				revise_dummy_trajectory(phase_id);//dummy�̍s���v�����̍X�V
				update_user_plan(parmeter, phase_id);//���̒�~�n�_�̓������Ԃ�\�����C���[�U�̍s���v�������X�V
			}
		}
	}
		

	void KatoMasterMethod::run()
	{
		//�����Ŏ��s���Ԃ̌v�����J�n
		timer->start();

		//������.�s���ύX��̃��[�U�̐���
		initialize();

		//���������s����(�e�����̃_�~�[�ʒu���v�Z����)(�������񑲘_��@)[Kato 13]
		Method::KatoBachelorMethod::decide_dummy_positions();
		clear_visited_pois_info_list_id_of_users();//users��visited_pois_info_list_id�̃N���A
		
		//�����Ń��[�U�̍s���̗\����_�~�[�̍s�����C������(��������C�_��@)[Kato 14]
		for (int phase_id = 0; phase_id < real_user->get_trajectory()->phase_count(); phase_id++){
			for (size_t dummy_id = 1; dummy_id <= entities->get_dummy_count(); dummy_id++){
				revising_dummy = entities->get_dummy_by_id(dummy_id);
				//�_�~�[��visited_pois_info_list_id�̍X�V
				if ((phase_id - 1) == revising_dummy->get_pause_phases().back()) revising_dummy->increment_visited_pois_info_list_id();
				//���[�U��visited_pois_info_list_id�̍X�V
				increment_visited_pois_info_list_id_of_users(phase_id);
				//�_�~�[�ƃ��[�U�̍s���v�����̏C��
				revise_dummy_positions(phase_id, dummy_id);
			}
		}

		//�����Ōv�����I��
		timer->end();

		//�I������
		terminate();
	}

}