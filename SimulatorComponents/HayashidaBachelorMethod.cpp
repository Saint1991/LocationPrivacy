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
	void HayashidaBachelorMethod::re_predicted_user_trajectory()
	{
		//---------------------------POI�K�⏇��������---------------------------------------------
		int phase_id = 0;
		/*
		//�ēx���[�U�̖K��POI������T.S.P.��p���ė\��
		std::pair<std::vector<std::shared_ptr<Map::BasicPoi const>>, double> order_visited_poi = re_traveling_salesman_problem(input_poi_list);

		//�ŏ��̓_��o�^
		std::vector<std::shared_ptr<Map::BasicPoi const>>::iterator now_poi = order_visited_poi.first.begin();
		std::vector<std::shared_ptr<Map::BasicPoi const>>::iterator next_poi = order_visited_poi.first.begin() + 1;

		user->set_visited_poi_of_phase(phase_id, Graph::MapNodeIndicator((*now_poi)->get_id()), (*now_poi)->data->get_position());
		user->set_random_starting_speed_at_poi(AVERAGE_SPEED, RANGE_OF_SPEED);

		double dest_rest_time = 0.0;//phase�̓������ԂƎ��ۂ̓������Ԃ̍���.�ŏ�����0

									//--------------��ڈȍ~�̓_������Dfor�ŁC�m�ۂ�����poi�̌��������[�v������-------------------
		for (int i = 1; i < POI_NUM; i++)
		{
			user->set_dest_rest_time(dest_rest_time);//�������̗]�蕪��o�^
			user->set_random_pause_time(phase_id, MIN_PAUSE_TIME, MAX_PAUSE_TIME);

			double moving_time_between_poi_and_next_poi = map->calc_necessary_time((*now_poi)->get_id(), (*next_poi)->get_id(), user->get_now_speed(phase_id));
			int next_arrive_time = moving_time_between_poi_and_next_poi + user->get_pause_time();

			double rest_pause_time = user->get_pause_time() - dest_rest_time;

			//��~���Ԃ�phase�Ɋ��Z���Cpause_time�ƍŒZ�H�o�H����path�����肵�Ă���
			lldiv_t variable_of_converted_pause_time_to_phase = std::lldiv(rest_pause_time, SERVICE_INTERVAL);

			//�o������Ƃ��̗]�莞�Ԃ�o�^
			user->set_rest_pause_time_when_departing(variable_of_converted_pause_time_to_phase.rem);

			//��~���ԕ��C�ephase�ɒ�~�ꏊ�ƈړ����x(0)��o�^
			set_pause_time_and_phases_of_visited_POI(&phase_id, rest_pause_time, variable_of_converted_pause_time_to_phase.quot, now_poi);

			std::vector<Graph::MapNodeIndicator> shortests_path_between_pois = map->get_shortest_path((*now_poi)->get_id(), (*next_poi)->get_id());
			std::vector<Graph::MapNodeIndicator>::iterator path_iter = shortests_path_between_pois.begin();//path���������邽�߂�index

			//���x��phase�Ŗ��߂�O���Q�Ƃ��Ȃ���΂Ȃ�Ȃ����Ƃɒ���
			double pause_position_speed = user->get_starting_speed();

			//�ŏ�������~���Ԃ�phase�Ɋ��Z�������̗]���time�Ƃ��C����ȊO��service_interval��time�Ƃ��āC���ݒn���狁�߂����n�_��distance���v�Z
			double distance = (SERVICE_INTERVAL - variable_of_converted_pause_time_to_phase.rem) * pause_position_speed;
			double distance_between_now_and_next_poi = map->shortest_distance((*now_poi)->get_id(), (*next_poi)->get_id());

			Graph::MapNodeIndicator nearest_position = (*now_poi)->get_id();

			//path���쐬�D�ꏊ�͈�ԋ߂�intersection���m�Ő��`��Ԃ���DMapNodeIndicator��Type��INVALID�Ƃ���D
			while (distance < distance_between_now_and_next_poi && phase_id != time_manager->phase_count() - 1) {
				//�ŒZ�H�̒��ň�ԋ߂�intersection��T���C���`��Ԃ���D     
				set_path_between_poi(now_poi, path_iter, nearest_position, pause_position_speed, SERVICE_INTERVAL, &distance, &phase_id);
			}

			//destination�̂Ƃ���܂ŕ⊮�ł�����Crest_time��ێ����Ă����I
			double distance_between_arrive_position_and_dest_position = distance - distance_between_now_and_next_poi;
			dest_rest_time = distance_between_arrive_position_and_dest_position / pause_position_speed;

			//�ړI�n�̓o�^
			if (phase_id == time_manager->phase_count() - 1) return;//�����o�H�������ɑS�Ė��܂��Ă��܂�����C���̎��_�ŏI��

			phase_id++;
			user->increment_visited_pois_info_list_id();
			user->set_visited_poi_of_phase(phase_id, (*next_poi)->get_id(), map->get_static_poi((*next_poi)->get_id())->data->get_position());
			user->set_random_starting_speed_at_poi(AVERAGE_SPEED, RANGE_OF_SPEED);

			now_poi++;
			next_poi++;
		}

		//---------------------------------end_time�܂œK���Ɍo�H�����߂�I---------------------------------------------------

		Math::Probability generator;
		//�ŏI�n�_�͏��������ɂƂ�(1.5�{�`2�{)�D�������C�}�b�v�̌��E�͈͂ɒ���
		double last_distance = 1.4 * (end_time - time_manager->time_of_phase(phase_id)) * user->get_now_speed(phase_id);

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
		user->set_random_pause_time(phase_id, MIN_PAUSE_TIME, MAX_PAUSE_TIME);
		user->set_dest_rest_time(dest_rest_time);


		//���ݒn�̒�~���Ԃ������_���Őݒ肵�C���n�_�̏o���n�̑��x�ŁC����POI�܂ł̍ŒZ�H�ňړ��������̎��Ԃ����߂�D
		double moving_time_between_poi_and_next_poi = map->calc_necessary_time((*now_poi)->get_id(), (*last_poi)->get_id(), user->get_now_speed(phase_id));
		int next_arrive_time = moving_time_between_poi_and_next_poi + user->get_pause_time();

		//��~���Ԃ�phase�Ɋ��Z���Cpause_time�ƍŒZ�H�o�H����path�����肵�Ă���
		double rest_pause_time = user->get_pause_time() - dest_rest_time;
		lldiv_t last_variable_of_converted_pause_time_to_phase = std::lldiv(rest_pause_time, SERVICE_INTERVAL);

		user->set_rest_pause_time_when_departing(last_variable_of_converted_pause_time_to_phase.rem);

		std::vector<Graph::MapNodeIndicator> last_shortests_path = map->get_shortest_path((*now_poi)->get_id(), (*last_poi)->get_id());

		//��~���ԕ��C�ephase�ɒ�~�ꏊ�ƒ�~phase��o�^
		set_pause_time_and_phases_of_visited_POI(&phase_id, rest_pause_time, last_variable_of_converted_pause_time_to_phase.quot, now_poi);

		std::vector<Graph::MapNodeIndicator>::iterator last_path_iter = last_shortests_path.begin();//path���������邽�߂�index
		//���x��phase�Ŗ��߂�O���Q�Ƃ��Ȃ���΂Ȃ�Ȃ����Ƃɒ���
		double last_pause_position_speed = user->get_starting_speed();

		//�ŏ�������~���Ԃ�phase�Ɋ��Z�������̗]���time�Ƃ��C����ȊO��service_interval��time�Ƃ��āC���ݒn���狁�߂����n�_��distance���v�Z
		double distance = (SERVICE_INTERVAL - last_variable_of_converted_pause_time_to_phase.rem) * last_pause_position_speed;
		double distance_between_now_and_next_poi = map->shortest_distance((*now_poi)->get_id(), (*last_poi)->get_id());

		Graph::MapNodeIndicator last_nearest_position = (*now_poi)->get_id();

		//path���쐬�D�ꏊ�͈�ԋ߂�intersection���m�Ő��`��Ԃ���DMapNodeIndicator��Type��INVALID�Ƃ���D
		//last_phase�܂Ŗ��߂�I
		while (phase_id != (time_manager->phase_count() - 1)) {
			set_path_between_poi(now_poi, last_path_iter, last_nearest_position, last_pause_position_speed, SERVICE_INTERVAL, &distance, &phase_id);
		}
		*/
		std::cout << "Success Making Re-Predicted User" << std::endl;
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
		
		//�S��~phase���擾
		std::vector<int> all_pause_phases = predicted_user->get_all_pause_phases();
		
		//���[�U�ƃ��[�U�̑S��~���Ԓ��ɑ��݂���S�_�~�[�Ƃ̋������v�Z
		//���ɋ����ʂ�sort�ς�
		//pair(pair(dummy_id, phase), distance)
		std::vector<std::pair<std::pair<entity_id, int>, double>> candidate_cross_dummies_ordered_by_dist = distance_between_user_and_dummies_at_pause_phases(all_pause_phases);

		//���X�̌����\��񐔕��C�ݒ�����݂Ă���
		for (int i = 0; i < original_cross_num; i++) {
			for (auto iter = candidate_cross_dummies_ordered_by_dist.begin(); iter != candidate_cross_dummies_ordered_by_dist.end(); iter++) {
				std::shared_ptr<Entity::RevisablePauseMobileEntity<Geography::LatLng>> target_dummy = entities->get_dummy_by_id(iter->first.first);
				//�Ώۃ_�~�[��POI��POI�ɒ�~���̏ꍇ
				if (target_dummy->read_node_pos_info_of_phase(iter->first.second).first.type() == Graph::POI)
				{
					//�ǐՉ\���ቺ�̂��߂̌o�H�ݒ�Ɠ��l�ȕ��@�ŁC�O�ʂ�ɏꍇ����
				}
				//�Ώۃ_�~�[���Ώ�phase���ɖK��POI�ȊO�ɑ��݂��Ă���ꍇ
				else
				{
					//���B�\�����ǂ̒��x���邩�ǂ����ɂ�邪�C�Ƃ肠�����V���v���ɂR�ɏꍇ����
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
	void HayashidaBachelorMethod::re_traveling_salesman_problem()
	{

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
					re_predicted_user_trajectory();
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
