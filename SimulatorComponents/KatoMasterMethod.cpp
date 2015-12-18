#include "stdafx.h"
#include "KatoMasterMethod.h"

namespace Method
{

	///<summary>
	/// �R���X�g���N�^
	/// �����Simulator�ō쐬�����e����͂ւ̎Q�Ƃ�n��
	///</summary>
	KatoMasterMethod::KatoMasterMethod(std::shared_ptr<Map::HayashidaDbMap const> map, std::shared_ptr<Entity::DifferentMovementUser<Geography::LatLng>> user, std::shared_ptr<Requirement::KatoMethodRequirement const> requirement, std::shared_ptr<Time::TimeSlotManager> time_manager)
		:KatoBachelorMethod(map, user, requirement, time_manager),
		real_user(nullptr), predicted_user(nullptr),Tu(0.0)
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
	std::shared_ptr<Entity::RevisablePauseMobileEntity<Geography::LatLng>> KatoMasterMethod::copy_predicted_user_plan(std::shared_ptr<Entity::PauseMobileEntity<Geography::LatLng>> input_user)
	{
		std::shared_ptr<std::vector<std::shared_ptr<Geography::LatLng>>> input_user_positions = input_user->read_trajectory()->read_positions();
		std::shared_ptr<std::vector<std::shared_ptr<Geography::LatLng>>> predicted_user_positions;

		std::copy(input_user_positions->begin(), input_user_positions->end(), predicted_user_positions->begin());
		
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

		return std::make_pair(requirement->max_variation_of_speed*(max_speed - speed), requirement->max_variation_of_speed*(speed - min_speed));
	}

	///<summary>
	/// ��~���Ԃ̍ő�ω��l�ƍŏ��ω��l���v�Z����
	/// pair(Max, min)
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
	KatoMasterMethod::ChangeParameter KatoMasterMethod::check_user_plan(int now_phase)
	{
		//�~�܂��Ă��遨��~���Ԃ̕ύXor��~�n�_�̕ύX
		//�����Е��ł���~���Ă����灨��~���ԕύX�̔��f
		if (predicted_user->check_pause_condition(now_phase) || real_user->check_pause_condition(now_phase)) {
			return check_user_pause_time(now_phase);
		}
		//�����Ă�����C
		else {
			//�������`��Ԃ������̏�Ɉʒu���Ă�����
			if (check_on_the_path(now_phase)) {
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
			//�����v�����ʂ��~���Ă�����(���҂̒�~�t���O���P)�CNO_CHANGE
			if (predicted_user->check_pause_condition(now_phase) && real_user->check_pause_condition(now_phase)) {
				return NO_CHANGE;
			}
			else if(predicted_user->check_pause_condition(now_phase) || real_user->check_pause_condition(now_phase)){
				//����real�̂ق�����~�t���O��0��(�o�����Ă���)�Cpredict��1�Ȃ�,�\���葁���o��
				if (predicted_user->check_pause_condition(now_phase) == true && real_user->check_pause_condition(now_phase) == false) {
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
			else {
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
			
			//���������ꏊ�ɂ�����CNO_CHANGE
			if (now_predicted_user_position == now_real_user_position) {
				Tu = 0.0;
				return NO_CHANGE;
			}
			else {
				if (/*����rest_time�����l�����āC�t�Z���āC���x�������Ȃ�*/true) {
					return check_user_pause_time(now_phase);
				}
				else {
					double real_user_dist = Geography::GeoCalculation::lambert_distance(*real_user->read_position_of_phase(now_phase - 1), *real_user->read_position_of_phase(now_phase));
					double predicted_user_dist = Geography::GeoCalculation::lambert_distance(*predicted_user->read_position_of_phase(now_phase - 1), *predicted_user->read_position_of_phase(now_phase));
					//�ړ�������real�̂ق����傫���Ȃ�
					if (real_user_dist > predicted_user_dist) {
						//change_time�̍��������߂�
						//�����ŁC�ǂꂭ�炢���x���傫���Ȃ����������߂Ă��܂��D
						//�����͌v�Z�ʂɊւ���Ă�����������C�v����
						double real_speed = real_user_dist / requirement->service_interval;
						Tu += map->calc_necessary_time(real_user->read_node_pos_info_of_phase(now_phase).first, real_user->get_poi().first, real_speed);
						return FASTER_SPEED;
					}
					else {
						//change_time�̍��������߂�
						//�����ŁC�ǂꂭ�炢���x���������Ȃ����������߂Ă��܂��D	
						double real_speed = real_user_dist / requirement->service_interval;
						Tu -= map->calc_necessary_time(real_user->read_node_pos_info_of_phase(now_phase).first, real_user->get_poi().first, real_speed);;
						return SLOER_SPEED;
					}
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
			double change_time = 0.0;
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
		bool KatoMasterMethod::check_on_the_path(int phase_id)
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
	void KatoMasterMethod::modification_of_user_trajectory_when_LONGER_PAUSE_TIME(ChangeParameter check_parameter, int phase_id)
	{
		if (Tu == requirement->service_interval) {
			//trajectory�����炷���ƂőΉ�
			predicted_user->get_trajectory()->insert_positions_to_trajectory(phase_id, 1);
			
			//��~���Ԃ̏C�����s���D
			predicted_user->revise_now_pause_time(phase_id, Tu);
		}
		else {
			//��~���Ԃ̏C�����s���D
			predicted_user->revise_now_pause_time(phase_id, Tu);

			//�o�H���Čv�Z
			//recalculation_path(phase_id);
		}
	}


	///<summary>
	/// ���[�U�̒�~���Ԃ��\����Z������predicted_user�̏C��
	///</summary>
	void KatoMasterMethod::modification_of_user_trajectory_when_SHORTER_PAUSE_TIME(ChangeParameter check_parameter, int phase_id)
	{
		if (Tu == requirement->service_interval) {
			//trajectory�����炷���ƂőΉ�
			predicted_user->get_trajectory()->delete_positions_to_trajectory(phase_id, 1);

			//��~���Ԃ̏C�����s���D
			predicted_user->revise_now_pause_time(phase_id, Tu);
		}
		else {
			//�o�H���Čv�Z
		}
	}

	///<summary>
	/// ���[�U�̃p�X�̏C��
	///</summary>
	void KatoMasterMethod::modification_of_user_trajectory_when_PATH(ChangeParameter check_parameter, int phase_id)
	{

	}

	///<summary>
	/// ���[�U�̈ړ����x���\���葁�����̏C��
	///</summary>
	void KatoMasterMethod::modification_of_user_trajectory_when_FASTER_SPEED(ChangeParameter check_parameter, int phase_id)
	{
		double real_user_speed = 0.0;

	}

	///<summary>
	/// ���[�U�̈ړ����x���\����x�����̏C��
	///</summary>
	void KatoMasterMethod::modification_of_user_trajectory_when_SLOER_SPEED(ChangeParameter check_parameter, int phase_id)
	{

	}


	///<summary>
	/// ���[�U�̒�~�ʒu�̏C��
	///</summary>
	void KatoMasterMethod::modification_of_user_trajectory_when_VISIT_POI(ChangeParameter check_parameter, int phase_id)
	{

	}



	///<summary>
	/// �_�~�[�̍s���v�������C������
	///</summary>
	void KatoMasterMethod::revise_dummy_trajectory(int phase_id)
	{
		//poi�Ɏ~�܂��Ă��鎞
		if (revising_dummy->check_pause_condition(phase_id)) {
			//�S�Ă̒�~�n�_�̓������Ԃ�ύX���CTu���ύX������D
			revise_dummy_pause_time(phase_id);
			revise_dummy_path(phase_id);
			revise_dummy_speed(phase_id);
			if (Tu != 0) revise_dummy_visit_poi(phase_id);
			
		}
		//���݈ړ����ŁC�v�����Ɠ���poi�Ɍ������Ă���ꍇ�C�������Ă���poi�ɒ�~�����Ƃ��čl����
		else {
			//�S�Ă̒�~�n�_�̓������Ԃ�ύX���CTu���ύX������D
			revise_dummy_pause_time(phase_id);
			revise_dummy_path(phase_id);
			revise_dummy_speed(phase_id);
			if (Tu != 0) revise_dummy_visit_poi(phase_id);

		}
	}


	///<summary>
	/// �_�~�[�̒�~���Ԃ̏C��
	///</summary>
	void KatoMasterMethod::revise_dummy_pause_time(int phase_id)
	{
		//�O�̒l�̕ێ�
		int pause_phase = revising_dummy->get_arrive_phase();
		double pause_time = revising_dummy->get_now_pause_time(pause_phase);
	
		double max_variable_value = calc_max_variable_pause_time(pause_phase).first;
		double min_variable_value = calc_max_variable_pause_time(pause_phase).second;
		double new_pause_time = Tu;//�����l�́C�ύX��Tu

		//�C���� �� �ő�ω��ʁ@�𖞂����C���C�C���� < �ő��~���ԂƂ���D
		if (Tu > 0) {
			if (Tu > max_variable_value) new_pause_time = max_variable_value;
			if (max_variable_value > requirement->max_pause_time) new_pause_time = requirement->max_pause_time;
		}
		else {
			if (std::abs(Tu) < min_variable_value) new_pause_time = min_variable_value;
			if (min_variable_value < requirement->min_pause_time) new_pause_time = requirement->min_pause_time;
		}		
				
		//�����������C�_�~�[�̒�~���Ԃ��C������D
		for (int i = phase_id + 1; i <= time_manager->phase_count(); i++)
		{
			//�����Ɏ��̖K��POI�̓������ԂƏo���������l�����āC�C������֐��������_���ŋL�q�D		
		}
		//if (time_manager->time_of_phase(phase_id + 1) == next_arrive_time + Tu) return;
		
		//��������������Ȃ�������C����path�̒���������
		Tu -= new_pause_time;
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
		int next_pause_phase = revising_dummy->get_arrive_phase();
		int next_next_pause_phase = revising_dummy->get_arrive_phase();
		double next_departing_speed = revising_dummy->get_now_speed(next_pause_phase);
		double next_arrive_time = time_manager->time_of_phase(next_pause_phase);
		
		double distance = map->shortest_distance(revising_dummy->read_node_pos_info_of_phase(next_pause_phase).first, revising_dummy->read_node_pos_info_of_phase(next_next_pause_phase).first);
		
		double new_speed = distance / (time_manager->time_of_phase(next_next_pause_phase) + Tu - time_manager->time_of_phase(next_pause_phase));
		revising_dummy->set_now_speed(next_pause_phase, new_speed);

		if (std::abs(revising_dummy->get_now_speed(next_pause_phase) - next_departing_speed) > requirement->max_variation_of_speed)
		{
			new_speed = revising_dummy->get_now_speed(next_pause_phase) - next_departing_speed > 0 ? next_departing_speed + requirement->max_variation_of_speed : next_departing_speed - requirement->max_variation_of_speed;
			revising_dummy->set_pause_time(next_pause_phase, new_speed);
		}
		double max_speed = requirement->average_speed_of_dummy + 0.5 * requirement->speed_range_of_dummy;
		double min_speed = requirement->average_speed_of_dummy - 0.5 * requirement->speed_range_of_dummy;
		if (max_speed < revising_dummy->get_now_speed(next_pause_phase)) revising_dummy->set_now_speed(next_pause_phase, max_speed);
		if (min_speed > revising_dummy->get_now_speed(next_pause_phase)) revising_dummy->set_now_speed(next_pause_phase, min_speed);

		//time_manager->time_of_phase(next_next_pause_phase) = time_manager->time_of_phase(phase_id) + revising_dummy->get_pause_time(phase_id) + (time_t)(distance / revising_dummy->get_speed(phase_id));
		double variation_of_arrive_time = time_manager->time_of_phase(next_next_pause_phase) - next_arrive_time;

		for (int i = phase_id + 1; i <= time_manager->phase_count(); i++)
		{
			//�����ɁC���x���ω������ۂ̃_�~�[�̍s���C�����L�q
		}
		if (time_manager->time_of_phase(phase_id + 1) == next_arrive_time + Tu) return;

	}


	///<summary>
	/// �_�~�[�̒�~�ʒu�̏C��
	///</summary>
	void KatoMasterMethod::revise_dummy_visit_poi(int phase_id)
	{

	}
	

	///<summary>
	/// �_�~�[�̒�~�ʒu�̏C��
	///</summary>
	void KatoMasterMethod::recalculation_path(const Graph::MapNodeIndicator& source, const Graph::MapNodeIndicator& destination, int phase_id)
	{
		double pause_position_speed = revising_dummy->get_now_speed(phase_id);

		//����phase����Cnow_puase_time��0�ȉ��ɂȂ�܂ŁC��~����o�^
		phase_id++;
		for (double rest_pause_time = revising_dummy->get_now_pause_time(phase_id); time < 0; phase_id++)
		{
			rest_pause_time -= requirement->service_interval;
			revising_dummy->set_now_pause_time(phase_id, rest_pause_time);
			revising_dummy->set_position_of_phase(phase_id, source, map->get_static_poi(source.id())->data->get_position());
		}

		std::vector<Graph::MapNodeIndicator> shortests_path_between_pois = map->get_shortest_path(source, destination);
		std::vector<Graph::MapNodeIndicator>::iterator path_iter = shortests_path_between_pois.begin();//path���������邽�߂�index

		//source����̋���
		//�ŏ�������~���Ԃ�phase�Ɋ��Z�������̗]���time�Ƃ��C����ȊO��service_interval��time�Ƃ��āC���ݒn���狁�߂����n�_��distance���v�Z
		//���x��phase�Ŗ��߂�O���Q�Ƃ��Ȃ���΂Ȃ�Ȃ����Ƃɒ���
		double now_time = requirement->service_interval - revising_dummy->get_now_pause_time(phase_id);
		double total_time_from_source_to_destination = map->calc_necessary_time(source, destination, pause_position_speed);
		Graph::MapNodeIndicator nearest_position = source;
		//path���쐬�D�ꏊ�͈�ԋ߂�intersection���m�Ő��`��Ԃ���DMapNodeIndicator��Type��INVALID�Ƃ���D
		while (now_time < total_time_from_source_to_destination)
		{
			//�ŏ��͒�~���Ԃ�phase�Ɋ��Z�����Ƃ��̗]�蕪��distance�Ƃ��āC�ŒZ�H�̒��ň�ԋ߂�intersection��T���C���`��Ԃ���D
			//double total_path_length = map->shortest_distance(source, *path_iter);
			while (now_time > map->calc_necessary_time(source, *path_iter, pause_position_speed))
			{
				nearest_position = *path_iter;
				path_iter++;
			}

			double distance = now_time * pause_position_speed;
			double distance_between_nearest_intersection_and_arrive_position = distance - map->shortest_distance(source, nearest_position);
			Geography::LatLng nearest_latlng
				= nearest_position.type() == Graph::NodeType::POI ? map->get_static_poi(nearest_position.id())->data->get_position() : *map->get_static_node(nearest_position.id())->data;
			Geography::LatLng next_nearest_latlang
				= (*path_iter).type() == Graph::NodeType::POI ? map->get_static_poi((*path_iter).id())->data->get_position() : *map->get_static_node((*path_iter).id())->data;
			double angle = Geography::GeoCalculation::lambert_azimuth_angle(nearest_latlng, next_nearest_latlang);

			Geography::LatLng arrive_position = Geography::GeoCalculation::calc_translated_point(nearest_latlng, distance_between_nearest_intersection_and_arrive_position, angle);

			if (phase_id == time_manager->phase_count() - 1) return;//�c���path�����߂鎞�̏I������
			(phase_id)++;
			revising_dummy->set_position_of_phase(phase_id, Graph::MapNodeIndicator(Graph::NodeType::OTHERS, Graph::NodeType::OTHERS), arrive_position);
			revising_dummy->set_now_speed(phase_id, pause_position_speed);

			now_time += requirement->service_interval;
		}

		//destination�̂Ƃ���܂ŕ⊮�ł�����Crest_time��ێ����Ă����I
		double time_between_arrive_position_and_dest_position = now_time - total_time_from_source_to_destination;
		double dest_rest_time
			= time_between_arrive_position_and_dest_position == requirement->service_interval ? 0 : time_between_arrive_position_and_dest_position;
		//�ړI�n�̓o�^
		//speed�͕ʓr�ݒ�̂��ߕs�v
		(phase_id)++;
		revising_dummy->set_position_of_phase(phase_id, destination, map->get_static_poi(destination.id())->data->get_position());

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
					if (check_user_plan(phase_id) != NO_CHANGE) {
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