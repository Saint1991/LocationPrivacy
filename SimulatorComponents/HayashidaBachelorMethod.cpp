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
		std::vector<Graph::MapNodeIndicator> order_visited_poi = repredicting_visited_poi_order_by_tsp(rest_poi_list);
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
			set_pause_time_and_phases_of_visited_POI_of_predicted_user(&phase_id, rest_pause_time, variable_of_converted_pause_time_to_phase.quot, *now_poi);

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
				set_path_between_poi_of_predicted_user(now_poi, path_iter, nearest_position, pause_position_speed, requirement->service_interval, &distance, &phase_id);
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
		set_pause_time_and_phases_of_visited_POI_of_predicted_user(&phase_id, rest_pause_time, last_variable_of_converted_pause_time_to_phase.quot, now_poi);

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
		std::vector<std::pair<std::pair<entity_id, int>, double>> candidate_cross_dummies_ordered_by_dist = distance_between_user_and_dummies_at_pause_phases(all_pause_phases);

		//���X�̌����\��񐔕��C�ݒ�����݂Ă���
		for (int i = 0; i < original_cross_num; i++) {
			for (auto iter = candidate_cross_dummies_ordered_by_dist.begin(); iter != candidate_cross_dummies_ordered_by_dist.end(); iter++) {
				std::shared_ptr<Entity::RevisablePauseMobileEntity<Geography::LatLng>> target_dummy = entities->get_dummy_by_id(iter->first.first);
				if (true) {
					//�Ώۃ_�~�[������phase�ɂ����āCPOI�ɒ�~���̏ꍇ
					if (target_dummy->check_pause_condition(now_phase))
					{

					}
					//�Ώۃ_�~�[������phase�ŖK��POI�ȊO�ɑ��݂��Ă���ꍇ
					else
					{
						//���B�\�����ǂ̒��x���邩�ǂ����ɂ�邪�C�Ƃ肠�����V���v���ɂR�ɏꍇ����
					}
				}
				else if (true) {

				}
				else {

				}
			}
		}

	}


	///<summary>
	/// ���[�U�ƃ��[�U�̑S��~���Ԓ��ɑ��݂���S�_�~�[�Ƃ̋������v�Z
	/// �����̏��������Ƀ\�[�g���ĕԂ�
	/// pair(pair(dummy_id, phase), distance)
	///</summary>
	std::vector<std::pair<std::pair<Method::KatoBachelorMethod::entity_id, int>, double>> HayashidaBachelorMethod::distance_between_user_and_dummies_at_pause_phases(std::vector<int> all_pause_phases)
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
	std::vector<Graph::MapNodeIndicator> HayashidaBachelorMethod::repredicting_visited_poi_order_by_tsp(std::vector<Graph::MapNodeIndicator>& input_poi_list)
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
	void HayashidaBachelorMethod::set_path_between_poi_of_predicted_user(std::vector<Graph::MapNodeIndicator>::iterator& now_poi, std::vector<Graph::MapNodeIndicator>::iterator& path_iter, Graph::MapNodeIndicator& nearest_position, double pause_position_speed, double service_interval, double *distance, int *phase_id)
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
	void HayashidaBachelorMethod::set_pause_time_and_phases_of_visited_POI_of_predicted_user(int *phase_id, double rest_pause_time, int total_pause_phase, Graph::MapNodeIndicator now_poi){
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

				if (!check_user_going_to_sheduled_POI()) {
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
