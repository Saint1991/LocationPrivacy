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
	bool HayashidaBachelorMethod::is_user_moving_to_POI()
	{
		return true;
	}


	///<summary>
	/// ���[�U���K��\��POI�Ɍ������Ă��邩�ǂ������`�F�b�N����
	///</summary>
	Graph::MapNodeIndicator HayashidaBachelorMethod::predict_next_poi() {
		return Graph::MapNodeIndicator(0);
	}

	///<summary>
	/// ���[�U�̍s�����ė\������
	///</summary>
	void HayashidaBachelorMethod::repredicting_user_trajectory(int now_phase)
	{
		//---------------------------POI�K�⏇��������---------------------------------------------
		std::vector<Graph::MapNodeIndicator> rest_poi_list;
		//���Ɍ�����POI���擾
		Graph::MapNodeIndicator first_poi = predict_next_poi();
		rest_poi_list.push_back(first_poi);

		//���O��visited_poi_id
		int prev_id = predicted_user->get_visited_pois_info_list_id() - 1;
		
		//�c��̖K��POI��push_back
		for (int id = predicted_user->get_visited_pois_info_list_id(); id < predicted_user->get_visited_pois_num(); id++) {
			if (predicted_user->get_any_visited_poi(id).first == first_poi) continue;
			rest_poi_list.push_back(predicted_user->get_any_visited_poi(id).first);
		}
		
		//�ēx���[�U�̖K��POI������T.S.P.��p���ė\��
		std::vector<Graph::MapNodeIndicator> order_visited_poi = repredict_visited_poi_order_by_tsp(rest_poi_list);
		Graph::MapNodeIndicator prev_position = predicted_user->get_any_visited_poi(prev_id).first.id();
		//�擪�ɒ��O�̖K��POI��o�^���Ă���
		auto iter = order_visited_poi.begin();
		order_visited_poi.insert(iter, prev_position);
		std::vector<Graph::MapNodeIndicator>::iterator now_poi = order_visited_poi.begin();
		std::vector<Graph::MapNodeIndicator>::iterator next_poi = order_visited_poi.begin() + 1;//�����first_poi�Ɠ������I

		//VisitedPoiInfo�̃��Z�b�g
		//�������C���Z�b�g����O�ɖK��POI�̐��Ɠ����t�F�[�Y���L�^���Ă���
		const int POI_NUM = predicted_user->get_visited_pois_num();
		int prev_arrive_phase = predicted_user->get_arrive_phase_of_any_visited_poi(prev_id);
		
		for (int id = predicted_user->get_visited_pois_info_list_id(); id < POI_NUM; id++) {
			predicted_user->get_visited_poi_info_list().pop_back();
		}
				
		double dest_rest_time = predicted_user->get_dest_rest_time_of_any_visited_poi(prev_id);//phase�̓������ԂƎ��ۂ̓������Ԃ̍���.�ŏ�����0
		int phase_id = prev_arrive_phase;
		//-------------�c��̓_������Dfor�ŁC�m�ۂ�����poi�̌��������[�v������-------------------
		for (int i = predicted_user->get_visited_pois_info_list_id(); i < POI_NUM; i++)
		{
			double moving_time_between_poi_and_next_poi = map->calc_necessary_time((*now_poi).id(), (*next_poi).id(), predicted_user->get_now_speed(now_phase));
			int next_arrive_time = moving_time_between_poi_and_next_poi + predicted_user->get_pause_time();

			double rest_pause_time = predicted_user->get_pause_time_of_any_visited_poi(i-1) - dest_rest_time;

			//��~���Ԃ�phase�Ɋ��Z���Cpause_time�ƍŒZ�H�o�H����path�����肵�Ă���
			lldiv_t variable_of_converted_pause_time_to_phase = std::lldiv(rest_pause_time, requirement->service_interval);

			//�o������Ƃ��̗]�莞�Ԃ�o�^
			predicted_user->set_rest_pause_time_when_departing(variable_of_converted_pause_time_to_phase.rem);

			//��~���ԕ��C�ephase�ɒ�~�ꏊ�ƈړ����x(0)��o�^
			set_predicted_user_pause_time_and_phases(&phase_id, rest_pause_time, variable_of_converted_pause_time_to_phase.quot, *now_poi);

			//���ݒn�D�������C���`��Ԃ����ꏊ�Ȃ̂ŁC���̂܂�shortest_distance�͎g���Ȃ����߁C�������v�Z����D
			std::vector<Graph::MapNodeIndicator> shortests_path_between_pois = map->get_shortest_path(predicted_user->get_any_visited_poi(prev_id).first.id(), first_poi.id());
			std::vector<Graph::MapNodeIndicator>::iterator path_iter = shortests_path_between_pois.begin();//path���������邽�߂�index

			//���x�͌��݂̑��x���Q��
			double pause_position_speed = predicted_user->get_starting_speed_using_arrive_phase(phase_id);

			//�ŏ�������~���Ԃ�phase�Ɋ��Z�������̗]���time�Ƃ��C����ȊO��service_interval��time�Ƃ��āC���ݒn���狁�߂����n�_��distance���v�Z
			double distance = (requirement->service_interval - predicted_user->get_rest_pause_time_when_departing_of_any_visited_poi(prev_id)) * pause_position_speed;
			double distance_between_now_and_next_poi = map->shortest_distance(prev_position.id(), first_poi.id());

			Graph::MapNodeIndicator nearest_position = prev_position.id();

			//path���쐬�D�ꏊ�͈�ԋ߂�intersection���m�Ő��`��Ԃ���DMapNodeIndicator��Type��INVALID�Ƃ���D
			while (distance < distance_between_now_and_next_poi && phase_id != time_manager->phase_count() - 1) {
				//�ŒZ�H�̒��ň�ԋ߂�intersection��T���C���`��Ԃ���D     
				set_predicted_user_path(now_poi, path_iter, nearest_position, pause_position_speed, requirement->service_interval, &distance, &phase_id);
			}

			//destination�̂Ƃ���܂ŕ⊮�ł�����Crest_time��ێ����Ă����I
			double distance_between_arrive_position_and_dest_position = distance - distance_between_now_and_next_poi;
			dest_rest_time = distance_between_arrive_position_and_dest_position / pause_position_speed;

			//�ړI�n�̓o�^
			if (phase_id == time_manager->phase_count() - 1) return;//�����o�H�������ɑS�Ė��܂��Ă��܂�����C���̎��_�ŏI��

			phase_id++;
			predicted_user->increment_visited_pois_info_list_id();
			predicted_user->set_visited_poi_of_phase(phase_id, (*next_poi).id(), map->get_static_poi((*next_poi).id())->data->get_position());
			predicted_user->set_random_starting_speed_at_poi(requirement->average_speed_of_dummy, requirement->speed_range_of_dummy);

			now_poi++;
			next_poi++;
			predicted_user->set_dest_rest_time(dest_rest_time);//�������̗]�蕪��o�^
			predicted_user->set_random_pause_time(phase_id, requirement->min_pause_time, requirement->max_pause_time);
		}

		//---------------------------------end_time�܂ł̐U�镑�������߂�I---------------------------------------------------

		last_behavior_of_predicted_user(phase_id, dest_rest_time, *now_poi);
		
		std::cout << "Success Re-Predicting User Plan" << std::endl;
	}


	///<summary>
	/// ���[�U�́C�Ō�̖K��\��POI���B��̍ŏI�t�F�[�Y�܂ł̐U�镑��
	///</summary>
	void HayashidaBachelorMethod::last_behavior_of_predicted_user(int phase_id, double dest_rest_time, Graph::MapNodeIndicator& now_poi)
	{
		pause_until_last_phase(phase_id, dest_rest_time, now_poi);
		//go_to_random_position(phase_id);
	}

	///<summary>
	/// �Ō�̃t�F�[�Y�܂Œ�~�𑱂���
	///</summary>
	void HayashidaBachelorMethod::pause_until_last_phase(int phase_id, double dest_rest_time, Graph::MapNodeIndicator& now_poi)
	{
		//���ݒn�̒�~���Ԃ������_���Őݒ肵�C���n�_�̏o���n�̑��x�ŁC����POI�܂ł̍ŒZ�H�ňړ��������̎��Ԃ����߂�D
		predicted_user->set_random_pause_time(phase_id, requirement->min_pause_time, requirement->max_pause_time);
		predicted_user->set_dest_rest_time(dest_rest_time);

		double rest_pause_time = predicted_user->get_pause_time() - dest_rest_time;
		lldiv_t last_variable_of_converted_pause_time_to_phase = std::lldiv(rest_pause_time, requirement->service_interval);

		predicted_user->set_rest_pause_time_when_departing(last_variable_of_converted_pause_time_to_phase.rem);
		//��~���ԕ��C�ephase�ɒ�~�ꏊ�ƒ�~phase��o�^
		set_predicted_user_pause_time_and_phases(&phase_id, rest_pause_time, last_variable_of_converted_pause_time_to_phase.quot, now_poi);

	}

	///<summary>
	/// �ǂ�����_�����_���Ɉʒu�����肵�C�����Ɍ������Đi�ށD
	///</summary>
	void HayashidaBachelorMethod::go_to_random_position(int phase_id, double dest_rest_time, Graph::MapNodeIndicator& now_poi)
	{
		/*
		Math::Probability generator;
		int end_time = time_manager->time_of_phase(time_manager->last_phase());

		//�ŏI�n�_�͏��������ɂƂ�(1.5�{�`2�{)�D�������C�}�b�v�̌��E�͈͂ɒ���
		double last_distance = 1.4 * (end_time - time_manager->time_of_phase(phase_id)) * predicted_user->get_now_speed(phase_id);

		//���̌��_�͈̔͂����߂�
		Geography::LatLng center = map_boundary.center();
		double last_angle = Geography::GeoCalculation::lambert_azimuth_angle((*now_poi)->data->get_position(), center);
		Geography::LatLng last_candidate_poi_position_range
			= Geography::GeoCalculation::calc_translated_point((*now_poi)->data->get_position(), last_distance, last_angle);

		Graph::Rectangle<Geography::LatLng> last_range(last_candidate_poi_position_range.lat() + 0.001, last_candidate_poi_position_range.lng(), last_candidate_poi_position_range.lat(), last_candidate_poi_position_range.lng() + 0.001);

		std::vector<std::shared_ptr<Map::BasicPoi const>> last_candidate_pois_list = get_pois_list(last_range);
		//����POI�̌���
		std::vector<std::shared_ptr<Map::BasicPoi const>>::iterator last_poi = last_candidate_pois_list.begin();


		//���ݒn�̒�~���Ԃ������_���Őݒ肵�C���n�_�̏o���n�̑��x�ŁC����POI�܂ł̍ŒZ�H�ňړ��������̎��Ԃ����߂�D
		predicted_user->set_random_pause_time(phase_id, requirement->min_pause_time, requirement->max_pause_time);
		predicted_user->set_dest_rest_time(dest_rest_time);


		//���ݒn�̒�~���Ԃ������_���Őݒ肵�C���n�_�̏o���n�̑��x�ŁC����POI�܂ł̍ŒZ�H�ňړ��������̎��Ԃ����߂�D
		double moving_time_between_poi_and_next_poi = map->calc_necessary_time((*now_poi)->get_id(), (*last_poi)->get_id(), user->get_now_speed(phase_id));
		int next_arrive_time = moving_time_between_poi_and_next_poi + user->get_pause_time();

		//��~���Ԃ�phase�Ɋ��Z���Cpause_time�ƍŒZ�H�o�H����path�����肵�Ă���
		double rest_pause_time = user->get_pause_time() - dest_rest_time;
		lldiv_t last_variable_of_converted_pause_time_to_phase = std::lldiv(rest_pause_time, requirement->service_interval);

		user->set_rest_pause_time_when_departing(last_variable_of_converted_pause_time_to_phase.rem);

		std::vector<Graph::MapNodeIndicator> last_shortests_path = map->get_shortest_path((*now_poi)->get_id(), (*last_poi)->get_id());

		//��~���ԕ��C�ephase�ɒ�~�ꏊ�ƒ�~phase��o�^
		set_pause_time_and_phases_of_visited_POI_of_predicted_user(&phase_id, rest_pause_time, last_variable_of_converted_pause_time_to_phase.quot, now_poi);

		std::vector<Graph::MapNodeIndicator>::iterator last_path_iter = last_shortests_path.begin();//path���������邽�߂�index
		//���x��phase�Ŗ��߂�O���Q�Ƃ��Ȃ���΂Ȃ�Ȃ����Ƃɒ���
		double last_pause_position_speed = user->get_starting_speed();

		//�ŏ�������~���Ԃ�phase�Ɋ��Z�������̗]���time�Ƃ��C����ȊO��service_interval��time�Ƃ��āC���ݒn���狁�߂����n�_��distance���v�Z
		double distance = (requirement->service_interval - last_variable_of_converted_pause_time_to_phase.rem) * last_pause_position_speed;
		double distance_between_now_and_next_poi = map->shortest_distance((*now_poi)->get_id(), (*last_poi)->get_id());

		Graph::MapNodeIndicator last_nearest_position = (*now_poi)->get_id();

		//path���쐬�D�ꏊ�͈�ԋ߂�intersection���m�Ő��`��Ԃ���DMapNodeIndicator��Type��INVALID�Ƃ���D
		//last_phase�܂Ŗ��߂�I
		while (phase_id != (time_manager->phase_count() - 1)) {
			set_path_between_poi_of_predicted_user(now_poi, last_path_iter, last_nearest_position, last_pause_position_speed, requirement->service_interval, &distance, &phase_id);
		}
		*/
	}

	///<summary>
	/// ��Ď�@�̊j�ɂȂ镔��
	/// ���[�U�̌������Đݒ肷��
		/*
		�p�^�[���P
		���݂��ړ����ŁC�ݒ莞���ړ�or ���݂��ړ����ŁC�ݒ莞��POI�ɒ�~��
		���݁��ݒ�ꏊ�C�ݒ�ꏊ��min�~�܂遨����POI

		�p�^�[���Q
		���݂�POI��~���ŁC�ݒ莞�͈ړ���or���݂�POI��~���ŁC�ݒ莞��POI��~��
		���݂���POI�̍Ō�̒�~�t�F�[�Y���ݒ�ꏊ�C�ݒ�ꏊ��min�~�܂遨����POI

		�p�^�[��3
		���݂�POI��~���ŁC�ݒ莞������POI�ɒ�~��
		�ݒ薳��

		------------�ȉ��͌��݃t�F�[�Y����^���Ȃ��p�^�[����prev��prev�����݃t�F�[�Y���傫��------------------------------

		�p�^�[���S
		�ݒ�t�F�[�Y���ړ���or�ݒ�t�F�[�Y�Œ�~��
		�O���POI���ݒ�ꏊ�C�ݒ�ꏊ��min�~�܂遨����POI

		��������̏ꍇ���C����POI���ŏI�t�F�[�Y�ɂȂ�Ƃ���next�ŃG���[�ɂȂ�̂ŁC���ӁI�I

		�ݒ莞�̒�~�̗L���Ŕ���H�H
		*/
	///</summary>
	void HayashidaBachelorMethod::re_setting_of_user_cross(int now_phase)
	{
		//���X�̃��[�U�̌����񐔂��L�^(�Q�Ƃ��郆�[�U�̓v�����ύX�O��input_user�ł��邱�Ƃɒ���)
		//����phase�܂ł̌����񐔂͍l�����Ȃ����Ƃɒ���
		int original_cross_num = input_user->get_cross_count() - input_user->get_cross_count_until_the_phase(now_phase);
		
		//����phase�ȍ~�̑S��~phase���擾
		std::vector<int> all_pause_phases = predicted_user->get_all_pause_phases_since_current_phase(now_phase);
		
		//���[�U�ƃ��[�U�̑S��~���Ԓ��ɑ��݂���S�_�~�[�Ƃ̋������v�Z
		//���ɋ����ʂ�sort�ς�
		//pair(pair(dummy_id, phase), distance)
		std::vector<std::pair<std::pair<entity_id, int>, double>> candidate_cross_dummies_ordered_by_dist = calc_distance_between_user_and_dummies(all_pause_phases);

		//���X�̌����\��񐔕��C�ݒ�����݂Ă���
		for (int i = 0; i < original_cross_num; i++) {
			for (auto iter = candidate_cross_dummies_ordered_by_dist.begin(); iter != candidate_cross_dummies_ordered_by_dist.end(); iter++) {
				//�ΏۃG���e�B�e�B�ƃt�F�[�Y���擾
				int target_phase = iter->first.second;
				entity_id target_dummy_id = iter->first.first;
				std::shared_ptr<Entity::RevisablePauseMobileEntity<Geography::LatLng>> target_dummy = entities->get_dummy_by_id(target_dummy_id);
				Graph::MapNodeIndicator target_pos = predicted_user->get_trajectory()->get_visited_node_id(target_phase);
				
				//�X�R�[�v�m�ۂ̂��߁C���݃t�F�[�Y���N�p���Ȃ��ꍇ�ɂ��Ă�if���̊O�ɏo���Ă���
				int prev_phase = target_dummy->get_prev_poi_arrive_phase_when_pausing(target_phase);
				int next_phase = target_dummy->get_next_poi_arrive_phase_when_pausing(target_phase);
				Graph::MapNodeIndicator prev_pos = target_dummy->get_trajectory()->get_visited_node_id(prev_phase);
				Graph::MapNodeIndicator next_pos = target_dummy->get_trajectory()->get_visited_node_id(next_phase);
				Math::Probability generator;
				double speed_at_target_phase = generator.uniform_distribution(requirement->average_speed_of_dummy - 0.5 * requirement->speed_range_of_dummy, requirement->average_speed_of_dummy + 0.5 * requirement->speed_range_of_dummy);

				//���݃t�F�[�Y����^���Ȃ��ꍇ
				if (target_dummy->get_prev_phase_when_visiting_poi(target_dummy->get_prev_phase_when_visiting_poi(target_phase) - 1) > now_phase){
					//�Ώۃt�F�[�Y�ňړ����H
					if (!target_dummy->isPause(target_phase)) {
						prev_phase = target_dummy->get_prev_phase_when_visiting_poi(target_phase);
						next_phase = target_dummy->get_next_phase_when_visiting_poi(target_phase);
					}
				}
				//���݃t�F�[�Y����^����ꍇ
				else {
					//���݃t�F�[�Y����~��
					if (target_dummy->isPause(now_phase)) {
						prev_phase = target_dummy->get_next_phase_when_visiting_poi(now_phase);
						//�Ώۃt�F�[�Y����~���Ă��邩�ۂ��ŏꍇ����
						next_phase =
							target_dummy->isPause(target_phase) ? target_dummy->get_next_poi_arrive_phase_when_pausing(target_phase) : target_dummy->get_next_phase_when_visiting_poi(target_phase);

						prev_pos = target_dummy->get_trajectory()->get_visited_node_id(prev_phase);
						next_pos = target_dummy->get_trajectory()->get_visited_node_id(next_phase);

						//���݈ʒu�Ɛݒ�ʒu�������ꍇ�͂������������͏o���Ȃ��D
						if (prev_pos == target_pos) continue;
					}
					//���݃t�F�[�Y�ňړ���
					else {
						prev_phase = now_phase;
						next_phase =
							target_dummy->isPause(target_phase) ? target_dummy->get_next_poi_arrive_phase_when_pausing(target_phase) : target_dummy->get_next_phase_when_visiting_poi(target_phase);

						//���݃t�F�[�Y��OTHERS�Ȃ͂��Ȃ̂ŁC��ԋ߂��_��⊮����
						prev_pos = *map->get_nearest_node_of_now_position(*target_dummy->get_trajectory()->get_positions()->at(now_phase));
						next_pos = target_dummy->get_trajectory()->get_visited_node_id(next_phase);
					}
					//time_limit��previous�ł̒�~���Ԃ��l�����Ȃ���΂Ȃ�Ȃ����Ƃɒ���
					double previous_time_limit = time_manager->time_of_phase(target_phase) - time_manager->time_of_phase(prev_phase);;
					double next_time_limit = time_manager->time_of_phase(next_phase) - time_manager->time_of_phase(target_phase) - requirement->min_pause_time;

					//���L�ꏊ�ɓ��B�\�Ȃ炻�̈ʒu��ݒ肵�C���B�s�\�Ȃ�΂�����x�ʂ̃t�F�[�Y������
					if (previous_time_limit < 0 || next_time_limit < 0) continue;
					if (!map->is_reachable(prev_pos, target_pos, target_dummy->get_now_speed(prev_phase+1), previous_time_limit)
						|| !map->is_reachable(target_pos, next_pos, speed_at_target_phase, next_time_limit)) {
						continue;
					}
				}
				//�o�H�̍Đݒ���s���D
				//�������̃N���A�Ɛݒ肪�K�v���Ǝv�����ǁC�Ƃ肠�������͕K�v�Ȃ����疳��
				double dest_rest_time = 0;
				int phase_id = prev_phase;
				calc_dummy_path(target_dummy_id, prev_pos, target_pos, &phase_id, &dest_rest_time, target_dummy->get_now_speed(prev_phase + 1));
				set_now_pause_time_and_phase(target_dummy_id, prev_pos, target_pos, &phase_id, &dest_rest_time, next_phase, target_phase, speed_at_target_phase);
				calc_dummy_path(target_dummy_id, target_pos, next_pos, &phase_id, &dest_rest_time, speed_at_target_phase);
				break;
			}
		}
		
		std::cout << "Success Re-Setting Cross" << std::endl;
	}

	///<summary>
	/// �_�~�[�̌o�H���Čv�Z
	///</summary>
	void HayashidaBachelorMethod::calc_dummy_path(entity_id target_dummy_id, const Graph::MapNodeIndicator& source, const Graph::MapNodeIndicator& destination, int *phase_id, double *dest_rest_time, double speed)
	{
		std::shared_ptr<Entity::RevisablePauseMobileEntity<Geography::LatLng>> target_dummy = entities->get_dummy_by_id(target_dummy_id);
		
		std::vector<Graph::MapNodeIndicator> shortests_path_between_pois = map->get_shortest_path(source, destination);
		std::vector<Graph::MapNodeIndicator>::iterator path_iter = shortests_path_between_pois.begin();//path���������邽�߂�index
				
		//source����̋���
		//�ŏ�������~���Ԃ�phase�Ɋ��Z�������̗]���time�Ƃ��C����ȊO��service_interval��time�Ƃ��āC���ݒn���狁�߂����n�_��distance���v�Z
		//���x��phase�Ŗ��߂�O���Q�Ƃ��Ȃ���΂Ȃ�Ȃ����Ƃɒ���
		double now_time = requirement->service_interval - target_dummy->get_now_pause_time(*phase_id);
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

			if (*phase_id == time_manager->phase_count() - 1) return;//�c���path�����߂鎞�̏I������
			(*phase_id)++;
			target_dummy->set_now_speed(*phase_id, speed);
			target_dummy->set_now_pause_time(*phase_id, 0.0);
			target_dummy->set_position_of_phase(*phase_id, Graph::MapNodeIndicator(Graph::NodeType::OTHERS, Graph::NodeType::OTHERS), arrive_position);
			
			now_time += requirement->service_interval;
		}

		//destination�̂Ƃ���܂ŕ⊮�ł�����Crest_time��ێ����Ă����I
		double time_between_arrive_position_and_dest_position = now_time - total_time_from_source_to_destination;
		*dest_rest_time
			= time_between_arrive_position_and_dest_position == requirement->service_interval ? 0 : time_between_arrive_position_and_dest_position;
		//�ړI�n�̓o�^
		(*phase_id)++;
		target_dummy->set_position_of_phase(*phase_id, destination, map->get_static_poi(destination.id())->data->get_position());
		target_dummy->set_now_speed(*phase_id, 0.0);
	}

	///<summary>
	///
	///</summary>
	void HayashidaBachelorMethod::set_now_pause_time_and_phase(entity_id target_dummy_id, Graph::MapNodeIndicator& target_pos, Graph::MapNodeIndicator& next_pos, int *phase_id, double *dest_rest_time, int next_phase, int target_phase, double speed) {
		std::shared_ptr<Entity::RevisablePauseMobileEntity<Geography::LatLng>> target_dummy = entities->get_dummy_by_id(target_dummy_id);
		int time_between_target_phase_and_next_phase = time_manager->time_of_phase(next_phase) - time_manager->time_of_phase(target_phase);
		double pause_time_at_target_phase
			= time_between_target_phase_and_next_phase - map->calc_necessary_time(target_pos, next_pos, speed);

		if (pause_time_at_target_phase < 0) throw std::invalid_argument("Pause Time is negative number!!");

		double rest_pause_time = pause_time_at_target_phase - *dest_rest_time;
		lldiv_t variable_of_converted_pause_time_to_phase = std::lldiv(rest_pause_time, requirement->service_interval);

		int arrive_phase = *phase_id;
		target_dummy->set_now_pause_time(*phase_id, rest_pause_time);

		for (int i = 0; i < variable_of_converted_pause_time_to_phase.quot; i++)
		{
			if (*phase_id == time_manager->phase_count() - 1) return;
			(*phase_id)++;
			rest_pause_time -= requirement->service_interval;
			target_dummy->set_now_pause_time(*phase_id, rest_pause_time);
			target_dummy->set_now_speed(*phase_id, 0.0);
			target_dummy->set_position_of_phase(*phase_id, target_pos, map->get_static_poi(target_pos.id())->data->get_position());
		}
	}


	///<summary>
	/// ���[�U�ƃ��[�U�̑S��~���Ԓ��ɑ��݂���S�_�~�[�Ƃ̋������v�Z
	/// �����̏��������Ƀ\�[�g���ĕԂ�
	/// pair(pair(dummy_id, phase), distance)
	///</summary>
	std::vector<std::pair<std::pair<Method::KatoBachelorMethod::entity_id, int>, double>> HayashidaBachelorMethod::calc_distance_between_user_and_dummies(std::vector<int> all_pause_phases)
	{
		std::vector<std::pair<std::pair<entity_id, int>, double>> candidate_cross_dummies;

		//���[�U�ƃ��[�U�̑S��~���Ԓ��ɑ��݂���S�_�~�[�Ƃ̋������v�Z
		for (std::vector<int>::iterator iter = all_pause_phases.begin(); iter != all_pause_phases.end(); iter++) {
			for (entity_id dummy_id = 0; dummy_id < requirement->dummy_num; dummy_id++) {
				//|user-dummy[dummy_id](*iter)|���v�Z���郁�\�b�h
				double dist = map->shortest_distance(predicted_user->read_node_pos_info_of_phase(*iter).first, entities->get_dummy_by_id(dummy_id)->read_node_pos_info_of_phase(*iter).first);
				candidate_cross_dummies.push_back(std::make_pair(std::make_pair(dummy_id, *iter), dist));
			}
		}
		//�C�������̏��������Ƀ\�[�g
		//�������C�����C�������̏ꍇ(POI�ɒ�~��)�Cphase����������D��
		std::sort(candidate_cross_dummies.begin(), candidate_cross_dummies.end(),
			[](std::pair<std::pair<entity_id, int>, double>& candidate1, std::pair<std::pair<entity_id, int>, double>& candidate2) {
			if (candidate1.second == candidate2.second) return candidate1.first.second < candidate2.first.second;
			return candidate1.second < candidate2.second;
		});

		return candidate_cross_dummies;
	}

	///<summary>
	///�@���[�U�̖K��POI�������ė\������D
	///</summary>
	std::vector<Graph::MapNodeIndicator> HayashidaBachelorMethod::repredict_visited_poi_order_by_tsp(std::vector<Graph::MapNodeIndicator>& input_poi_list)
	{
		std::vector<std::pair<std::vector<Graph::MapNodeIndicator>, double>> all_tsp_solution;
		Graph::MapNodeIndicator start_poi = input_poi_list.front();//�ŏ��̖K��POI

		std::vector<int> visited_pois_except_start_poi;//�ŏ��̖K��POI��������POI_list
		for (std::vector<Graph::MapNodeIndicator>::iterator iter = input_poi_list.begin() + 1; iter != input_poi_list.end(); iter++) {
			visited_pois_except_start_poi.push_back((*iter).id());
		}

		//id���Ƀ\�[�g
		std::sort(visited_pois_except_start_poi.begin(), visited_pois_except_start_poi.end(), [](int& poi1, int& poi2) {
			return poi1 < poi2;
		});

		do {
			double distance = map->shortest_distance(start_poi.id(), visited_pois_except_start_poi.front());
			std::vector<int>::iterator iter = visited_pois_except_start_poi.begin();
			for (int i = 0; i < visited_pois_except_start_poi.size() - 1; i++, iter++) {
				distance += map->shortest_distance(*iter, *(iter + 1));
			}
			std::vector<Graph::MapNodeIndicator> visited_pois_except_start_poi_convert_to_mapnodeindicator;
			for (auto iter = visited_pois_except_start_poi.begin(); iter != visited_pois_except_start_poi.end(); iter++) {
				visited_pois_except_start_poi_convert_to_mapnodeindicator.push_back(Graph::MapNodeIndicator(*iter, Graph::POI));
			}
			all_tsp_solution.push_back(std::make_pair(visited_pois_except_start_poi_convert_to_mapnodeindicator, distance));
			all_tsp_solution.back().first.insert(all_tsp_solution.back().first.begin(), start_poi);
		} while (std::next_permutation(visited_pois_except_start_poi.begin(), visited_pois_except_start_poi.end()));

		std::sort(all_tsp_solution.begin(), all_tsp_solution.end(), [](std::pair<std::vector<Graph::MapNodeIndicator>, double>& poi1, std::pair<std::vector<Graph::MapNodeIndicator>, double>& poi2) {
			return poi1.second < poi2.second;
		});

		return all_tsp_solution.front().first;
	}

	///<summary>
	/// �ŒZ�H�̒��ň�ԋ߂�intersection��T���C���`��Ԃ���D 
	/// now_poi�ɂ͑��x�ƒ�~���Ԃ�ݒ肵�Ă��邱��
	/// �Ō�͖ړI�n�̒��O�̏ꏊ��⊮�����ꏊ���o�^����Ă���D
	///</summary>
	void HayashidaBachelorMethod::set_predicted_user_path(std::vector<Graph::MapNodeIndicator>::iterator& now_poi, std::vector<Graph::MapNodeIndicator>::iterator& path_iter, Graph::MapNodeIndicator& nearest_position, double pause_position_speed, double service_interval, double *distance, int *phase_id)
	{
		while (*distance > map->shortest_distance((*now_poi).id(), *path_iter))
		{
			nearest_position = *path_iter;
			path_iter++;
		}

		double distance_between_start_and_nearest_position = map->shortest_distance((*now_poi).id(), nearest_position);
		double distance_between_nearest_intersection_and_arrive_position = *distance - distance_between_start_and_nearest_position;

		Geography::LatLng nearest_latlng
			= nearest_position.type() == Graph::NodeType::POI ? map->get_static_poi(nearest_position.id())->data->get_position() : *map->get_static_node(nearest_position.id())->data;
		Geography::LatLng next_nearest_latlang
			= (*path_iter).type() == Graph::NodeType::POI ? map->get_static_poi((*path_iter).id())->data->get_position() : *map->get_static_node((*path_iter).id())->data;
		double angle = Geography::GeoCalculation::lambert_azimuth_angle(nearest_latlng, next_nearest_latlang);

		Geography::LatLng arrive_position = Geography::GeoCalculation::calc_translated_point(nearest_latlng, distance_between_nearest_intersection_and_arrive_position, angle);

		if (*phase_id == time_manager->phase_count() - 1) return;
		(*phase_id)++;
		predicted_user->set_now_speed(*phase_id, pause_position_speed);
		predicted_user->set_position_of_phase(*phase_id, Graph::MapNodeIndicator(Graph::NodeType::OTHERS, Graph::NodeType::OTHERS), arrive_position);

		*distance += service_interval * pause_position_speed;
	}


	///<summary>
	/// ��~���Ԃ�phase�̃Z�b�g
	///</summary>
	void HayashidaBachelorMethod::set_predicted_user_pause_time_and_phases(int *phase_id, double rest_pause_time, int total_pause_phase, Graph::MapNodeIndicator now_poi){
		predicted_user->set_now_pause_time(*phase_id, rest_pause_time);

		for (int i = 0; i < total_pause_phase; i++)
		{
			if (*phase_id == time_manager->phase_count() - 1) break;
			(*phase_id)++;
			predicted_user->set_pause_phases(*phase_id);
			rest_pause_time -= requirement->service_interval;
			predicted_user->set_now_pause_time(*phase_id, rest_pause_time);
			predicted_user->set_position_of_phase(*phase_id, now_poi.id(), map->get_static_poi(now_poi.id())->data->get_position());
		}
	}

	/*
	///<summary>
	/// ������ (����͓��ɂ�邱�Ƃ͂Ȃ�)
	///</summary>
	void HayashidaBachelorMethod::initialize()
	{
		//���[�U�̓����̕ύX���V�������D
	}
	*/

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

				if (!is_user_moving_to_POI()) {
					repredicting_user_trajectory(phase_id);
					re_setting_of_user_cross(phase_id);
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
