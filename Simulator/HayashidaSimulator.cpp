#include "stdafx.h"
#include "HayashidaSimulator.h"

namespace Simulation
{
	///<summary>
	/// �R���X�g���N�^
	///</summary>
	HayashidaSimulator::HayashidaSimulator(const Graph::Rectangle<Geography::LatLng>& boundary) 
		: ISimulator<Map::HayashidaDbMap, 
			Entity::DifferentMovementUser<Geography::LatLng>, 
			Entity::RevisablePauseMobileEntity<Geography::LatLng>, 
			Requirement::KatoMethodRequirement, Geography::LatLng, 
			Graph::RevisableTrajectory<Geography::LatLng>
		>(), map_boundary(boundary)
	{
	}

	///<summary>
	/// �f�X�g���N�^
	///</summary>
	HayashidaSimulator::~HayashidaSimulator()
	{
	}


	///<summary>
	/// rect�͈͓̔��ɂ���poi_list��K���ɃV���b�t��������ԂŎ擾
	/// ���T�����āC������Ȃ������ꍇ�́Crect�͈̔͂��L���Ȃ����čČ���
	///</summary>
	std::vector<std::shared_ptr<Map::BasicPoi const>> HayashidaSimulator::get_pois_list(Graph::Rectangle<Geography::LatLng>& boundary)
	{
		std::vector<std::shared_ptr<Map::BasicPoi const>> pois_list = map->find_pois_within_boundary(boundary);
		double length = 0.005;
		//�����͈͓���POI��������Ȃ�������C�͈͂��L���čČv�Z
		if (pois_list.size() == 0) {
			while (pois_list.size() == 0) {
				length += 0.001;
				boundary.top += 0.5 * length;
				boundary.left -= 0.5 * length;
				boundary.bottom -= 0.5 * length;
				boundary.right += 0.5 * length;
				pois_list = map->find_pois_within_boundary(boundary);
			}
		}

		std::random_device device;
		std::mt19937_64 generator(device());
		std::shuffle(pois_list.begin(), pois_list.end(), generator);

		return pois_list;
	}

	
	///<summary>
	/// time_manager�̐���
	/// predicted_user�Ɠ��������I
	///</summary>
	std::shared_ptr<Time::TimeSlotManager> HayashidaSimulator::create_time_manager() {
		std::unique_ptr<std::vector<time_t>> timeslots = std::make_unique<std::vector<time_t>>();
		int time = 0;
		for (int id = 0; id < predicted_user->get_trajectory()->phase_count(); id++, time += SERVICE_INTERVAL) {
			timeslots->push_back(time);
		}

		return std::make_shared<Time::TimeSlotManager>(std::move(timeslots));
	}


	///<summary>
	/// �ŒZ�H�̒��ň�ԋ߂�intersection��T���C���`��Ԃ���D 
	/// now_poi�ɂ͑��x�ƒ�~���Ԃ�ݒ肵�Ă��邱��
	/// �Ō�͖ړI�n�̒��O�̏ꏊ��⊮�����ꏊ���o�^����Ă���D
	///</summary>
	void HayashidaSimulator::set_path_between_poi(std::vector<std::shared_ptr<Map::BasicPoi const>>::iterator& now_poi, std::vector<Graph::MapNodeIndicator>::iterator& path_iter, Graph::MapNodeIndicator& nearest_position, double pause_position_speed, double service_interval, double *distance, int *phase_id)
	{
		while (*distance > map->shortest_distance((*now_poi)->get_id(), *path_iter))
		{
			nearest_position = *path_iter;
			path_iter++;
		}

		//if (*distance != map->shortest_distance((*now_poi)->get_id(), *path_iter)) {
		double distance_between_start_and_nearest_position = map->shortest_distance((*now_poi)->get_id(), nearest_position);
		double distance_between_nearest_intersection_and_arrive_position = *distance - distance_between_start_and_nearest_position;

		Geography::LatLng nearest_latlng
			= nearest_position.type() == Graph::NodeType::POI ? map->get_static_poi(nearest_position.id())->data->get_position() : *map->get_static_node(nearest_position.id())->data;
		Geography::LatLng next_nearest_latlang
			= (*path_iter).type() == Graph::NodeType::POI ? map->get_static_poi((*path_iter).id())->data->get_position() : *map->get_static_node((*path_iter).id())->data;
		double angle = Geography::GeoCalculation::lambert_azimuth_angle(nearest_latlng, next_nearest_latlang);

		Geography::LatLng arrive_position = Geography::GeoCalculation::calc_translated_point(nearest_latlng, distance_between_nearest_intersection_and_arrive_position, angle);

		if (*phase_id == time_manager->phase_count() - 1) return;
		(*phase_id)++;
		user->set_now_speed(*phase_id, pause_position_speed);
		user->set_position_of_phase(*phase_id, Graph::MapNodeIndicator(Graph::NodeType::OTHERS, Graph::NodeType::OTHERS), arrive_position);

		/*}
		//distance��map->shortest_distance((*now_poi)->get_id(), *path_iter)�����������́C���x�����_orPOI�ɓ�������ꍇ
		else {
		(*phase_id)++;
		(*path_iter).type() == Graph::NodeType::POI ?
		user->set_position_of_phase(*phase_id, (*path_iter).id(), map->get_static_poi((*path_iter).id())->data->get_position()) :
		user->set_position_of_phase(*phase_id, (*path_iter).id(), *map->get_static_node((*path_iter).id())->data);
		}*/

		*distance += service_interval * pause_position_speed;
	}

	///<summary>
	/// ��~���Ԃ�phase�̃Z�b�g
	///</summary>
	void HayashidaSimulator::set_pause_time_and_phases_of_visited_POI(int *phase_id, double rest_pause_time, int total_pause_phase, std::vector<std::shared_ptr<Map::BasicPoi const>>::iterator& now_poi){
		user->set_now_pause_time(*phase_id, rest_pause_time);

		for (int i = 0; i < total_pause_phase; i++)
		{
			if (*phase_id == time_manager->last_phase()) break;
			(*phase_id)++;
			user->set_pause_phases(*phase_id);
			rest_pause_time -= SERVICE_INTERVAL;
			if (rest_pause_time < 0) rest_pause_time = 0.0;
			user->set_now_pause_time(*phase_id, rest_pause_time);
			user->set_position_of_phase(*phase_id, (*now_poi)->get_id(), (*now_poi)->data->get_position());
		}
	}
	

	///<summary>
	/// input����poi��list���쐬
	///</summary>
	void HayashidaSimulator::input_visit_pois() {
		//POI�n����쐬����ۂ́C�I��͈�
		Graph::Rectangle<Geography::LatLng> pois_range = map_boundary.transform_rect_of_latlang_to_x_times(0.4);

		std::vector<std::shared_ptr<Map::BasicPoi const>> random_pois_list = get_pois_list(pois_range);
		std::vector<std::shared_ptr<Map::BasicPoi const>>::iterator iter = random_pois_list.begin();
		for (int i = 0; i < POI_NUM; i++, iter++) {
			input_poi_list.push_back(*iter);
		}
	}

	///<summary>
	/// input_elements�̓���POI����ɁC����Z�[���X�}�����������C�����������������Ƀ\�[�g����vector��Ԃ��D
	///</summary>
	std::vector<std::pair<std::vector<std::shared_ptr<Map::BasicPoi const>>, double>> HayashidaSimulator::all_traveling_salesman_problem(std::vector<std::shared_ptr<Map::BasicPoi const>>& visited_pois)
	{
		std::vector<std::pair<std::vector<std::shared_ptr<Map::BasicPoi const>>, double>> all_tsp_solution;
		std::shared_ptr<Map::BasicPoi const> start_poi = visited_pois.front();//�ŏ��̖K��POI
		
		std::vector<std::shared_ptr<Map::BasicPoi const>> visited_pois_except_start_poi;//�ŏ��̖K��POI��������POI_list
		for (std::vector<std::shared_ptr<Map::BasicPoi const>>::iterator iter = visited_pois.begin() + 1; iter != visited_pois.end(); iter++) {
			visited_pois_except_start_poi.push_back(*iter);
		}

		//id���Ƀ\�[�g
		std::sort(visited_pois_except_start_poi.begin(), visited_pois_except_start_poi.end(), [](std::shared_ptr<Map::BasicPoi const>& poi1, std::shared_ptr<Map::BasicPoi const>& poi2) {
			return poi1->get_id() < poi2->get_id();
		});

		do {
			double distance = map->shortest_distance(start_poi->get_id(), visited_pois_except_start_poi.front()->get_id());
			std::vector<std::shared_ptr<Map::BasicPoi const>>::iterator iter = visited_pois_except_start_poi.begin();
			for (int i= 0; i < POI_NUM - 2; i++, iter++) {
				distance += map->shortest_distance((*iter)->get_id(), (*(iter+1))->get_id());
			}
			all_tsp_solution.push_back(std::make_pair(visited_pois_except_start_poi, distance));
			all_tsp_solution.back().first.insert(all_tsp_solution.back().first.begin(), start_poi);
		} while (std::next_permutation(visited_pois_except_start_poi.begin(), visited_pois_except_start_poi.end()));
		
		std::sort(all_tsp_solution.begin(), all_tsp_solution.end(), [](std::pair<std::vector<std::shared_ptr<Map::BasicPoi const>>, double>& poi1, std::pair<std::vector<std::shared_ptr<Map::BasicPoi const>>, double> poi2) {
			return poi1.second < poi2.second;
		});

		//�\���p
		std::for_each(all_tsp_solution.begin(), all_tsp_solution.end(), [](std::pair<std::vector<std::shared_ptr<Map::BasicPoi const>>, double>& tsp) {
			for (auto iter = tsp.first.begin(); iter != tsp.first.end(); iter++) std::cout << "( Node ID : " << (*iter)->get_id() << " ) �� ";
			std::cout << " distance : " << tsp.second << std::endl;
		});

		//�t�@�C���o�͗p
		std::ofstream ofs("C:/Users/Shuhei/Desktop/Result_Path/tsp_distance.txt");
		std::for_each(all_tsp_solution.begin(), all_tsp_solution.end(), [&ofs](std::pair<std::vector<std::shared_ptr<Map::BasicPoi const>>, double>& tsp) {
			ofs << tsp.second << std::endl;
		});

		return all_tsp_solution;
	}


	///<summary>
	/// input_elements�̓���POI����ɁC����Z�[���X�}�����������C�œK����Ԃ��D
	///</summary>
	std::pair<std::vector<std::shared_ptr<Map::BasicPoi const>>, double> HayashidaSimulator::traveling_salesman_problem(std::vector<std::shared_ptr<Map::BasicPoi const>>& visited_pois)
	{
		return all_traveling_salesman_problem(visited_pois).front();
	}

	///<summary>
	/// input_elements�̓���POI����ɁC����Z�[���X�}�����������C�����̔�̋t���̕��z�ɏ]���Ĉ��m���ŋ��߂�poi�n���Ԃ��D
	///</summary>
	std::pair<std::vector<std::shared_ptr<Map::BasicPoi const>>, double> HayashidaSimulator::the_power_alpha_of_the_reciprocal_of_the_ratio_of_the_optimal_distance(std::vector<std::shared_ptr<Map::BasicPoi const>>& visited_pois, int alpha)
	{
		std::vector<std::pair<std::vector<std::shared_ptr<Map::BasicPoi const>>, double>> all_tsp_solution = all_traveling_salesman_problem(visited_pois);
		double optimal_distance = all_tsp_solution.front().second;
				
		std::vector<double> distance_ratio_list;//�����̔�̋t�����i�[
		std::vector<double> normalization_ratio_list;//�����̔�̋t���𐳋K���������̂��i�[

		//�œK���ɑ΂��鋗���̔���v�Z
		//alpha��Y�ꂸ��
		for (size_t i = 0; i < all_tsp_solution.size(); i++) {
			alpha != 0 ? distance_ratio_list.push_back((1.0/(all_tsp_solution.at(i).second/ optimal_distance)) * alpha) : distance_ratio_list.push_back(1.0);
		}

		//�����̔�̋t���𐳋K��
		double total_ratio = 0.0;
		double accumulation_ratio = 0.0;
		for (auto iter : distance_ratio_list) total_ratio += iter;
		for (auto iter : distance_ratio_list) {
			accumulation_ratio += iter / total_ratio;
			normalization_ratio_list.push_back(accumulation_ratio);
		}

		Math::Probability generator;
		double rate = generator.uniform_distribution(0.0, 1.0);

		int i = 0;
		for (std::vector<double>::iterator iter = normalization_ratio_list.begin(); iter != normalization_ratio_list.end(); iter++, i++) {
			if (rate <= *iter) {
				std::cout << " Real User Root is " << i + 1 << " th Root of T.S.P. : " << "distance = " << all_tsp_solution.at(i).second << std::endl;
				return all_tsp_solution.at(i);
			}
		}
	}



	///<summary>
	/// �����_�����[�u���s�����[�U�𐶐�����
	/// �������C���݂͍ŒZ�o�H�ňړ�
	///</summary>
	void HayashidaSimulator::make_random_movement_user()
	{
		Math::Probability generator;

		//rect_init_lang�ɂ͎n�_�ɂ������͈͂��C���X�^���X�œ���
		Graph::Rectangle<Geography::LatLng> rect_init_range(BASE_LAT + 0.5*length_of_rect, BASE_LNG - 0.5*length_of_rect, BASE_LAT - 0.5*length_of_rect, BASE_LNG + 0.5*length_of_rect);
	 
		//----------------------------time_manager�̐���-------------------------------------//
		time_manager = create_time_manager();
		
		//---------------------------�ŏ��̓_������---------------------------------------------
		int phase_id = 0;
		std::vector<std::shared_ptr<Map::BasicPoi const>> random_pois_list = get_pois_list(rect_init_range);
		std::vector<std::shared_ptr<Map::BasicPoi const>>::iterator now_poi = random_pois_list.begin();
		user->set_visited_poi_of_phase(phase_id, Graph::MapNodeIndicator((*now_poi)->get_id()), (*now_poi)->data->get_position());
		user->set_random_starting_speed_at_poi(AVERAGE_SPEED, RANGE_OF_SPEED);
		
		double dest_rest_time = 0;//phase�̓������ԂƎ��ۂ̓������Ԃ̍���.�ŏ�����0

		//--------------��ڈȍ~�̓_������Dfor�ŁC�m�ۂ�����poi�̌��������[�v������-------------------
		for (int i = 1; i < POI_NUM; i++)
		{
			//���̌��_�͈̔͂����߂�
			double distance_between_positions = generator.uniform_distribution(150.0, 350.0);
			double angle_of_positions = generator.uniform_distribution(-(M_PI), M_PI_4);
			Geography::LatLng next_candidate_poi_position_range
				= Geography::GeoCalculation::calc_translated_point((*now_poi)->data->get_position(), distance_between_positions, angle_of_positions);

			Graph::Rectangle<Geography::LatLng> range(next_candidate_poi_position_range.lat() + 0.001, next_candidate_poi_position_range.lng(), next_candidate_poi_position_range.lat(), next_candidate_poi_position_range.lng() + 0.001);
			
			std::vector<std::shared_ptr<Map::BasicPoi const>> candidate_pois_list = get_pois_list(range);
			//����POI�̌���
			std::vector<std::shared_ptr<Map::BasicPoi const>>::iterator next_poi = candidate_pois_list.begin();

			//���ݒn�̒�~���Ԃ������_���Őݒ肵�C���n�_�̏o���n�̑��x�ŁC����POI�܂ł̍ŒZ�H�ňړ��������̎��Ԃ����߂�D
			user->set_random_pause_time(phase_id, MIN_PAUSE_TIME, MAX_PAUSE_TIME);
			user->set_dest_rest_time(dest_rest_time);

			double moving_time_between_poi_and_next_poi = map->calc_necessary_time((*now_poi)->get_id(), (*next_poi)->get_id(), user->get_starting_speed());
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
			while (distance < distance_between_now_and_next_poi && phase_id != time_manager->phase_count() - 1 ) {
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

			//���݈ʒu�̍X�V
			*now_poi = *next_poi;

		}

		//---------------------------------end_time�܂œK���Ɍo�H�����߂�I---------------------------------------------------
		//last_behavior_of_user(phase_id, dest_rest_time, now_poi);
		
		std::cout << "Success Creating Random User" << std::endl;
	}

	

	///<summary>
	/// input_pois����ɁCT.S.P.��p���C�\�z���[�U���쐬����D
	///</summary>
	std::shared_ptr<Entity::RevisablePauseMobileEntity<Geography::LatLng>> HayashidaSimulator::create_artificial_user(std::pair<std::vector<std::shared_ptr<Map::BasicPoi const>>, double>& order_visited_poi) {
		int phase_id = 0;
		Math::Probability generator;

		//�ȉ��̃��X�g������āC�R���X�g���N�^�ɂ�����
		std::shared_ptr<std::vector<std::shared_ptr<Geography::LatLng const>>> positions = std::make_shared<std::vector<std::shared_ptr<Geography::LatLng const>>>();
		std::shared_ptr<std::vector<Graph::MapNodeIndicator>> visited_node_ids	= std::make_shared<std::vector<Graph::MapNodeIndicator>>();
		std::shared_ptr<std::vector<std::string>> venue_names = std::make_shared<std::vector<std::string>>();
		std::vector<double> now_speed_list;
		std::vector<double> current_pause_time_list;
		
		std::vector<Entity::VisitedPoiInfo> visited_pois_info_list;
		Entity::VisitedPoiInfo visited_poi_info;
		
		//---------------------------POI�K�⏇��������---------------------------------------------

		//user��visited_pois��p���āC�n�_��start_poi�̏���Z�[���X�}����������
		
		//�ŏ��̏���F�X���߂�
		//�ŏ��̓_��o�^
		std::vector<std::shared_ptr<Map::BasicPoi const>>::iterator now_poi = order_visited_poi.first.begin();
		double pause_time = generator.uniform_distribution(MIN_PAUSE_TIME, MAX_PAUSE_TIME);
		double speed = generator.uniform_distribution(MIN_SPEED, MAX_SPEED);

		double dest_rest_time = 0.0;//phase�̓������ԂƎ��ۂ̓������Ԃ̍���.�ŏ�����0
		double rest_pause_time = pause_time - dest_rest_time;
		lldiv_t variable_of_converted_pause_time_to_phase = std::lldiv(rest_pause_time, SERVICE_INTERVAL);

		//visited_poi_info��	����o�^
		visited_poi_info.visited_poi = std::make_pair(Graph::MapNodeIndicator((*now_poi)->get_id(), Graph::POI), (*now_poi)->data->get_position());
		for (int id = 0; id <= variable_of_converted_pause_time_to_phase.quot; id++) visited_poi_info.pause_phases.push_back(phase_id + id);
		visited_poi_info.arrive_phase = phase_id;
		visited_poi_info.pause_time = pause_time;
		visited_poi_info.starting_speed = speed;
		visited_poi_info.rest_pause_time_when_departing = variable_of_converted_pause_time_to_phase.rem;
		visited_poi_info.dest_rest_time = dest_rest_time;
		visited_pois_info_list.push_back(visited_poi_info);
		
		//�ʒu�Cnode_ids, venue_name�C��~���ԁC��~�t�F�[�Y��o�^
		for (int i = 0; i <= variable_of_converted_pause_time_to_phase.quot + 1; i++, phase_id++)
		{
			positions->push_back(std::make_shared<Geography::LatLng const>((*now_poi)->data->get_position()));
			visited_node_ids->push_back(Graph::MapNodeIndicator((*now_poi)->get_id(), Graph::POI));
			venue_names->push_back((*now_poi)->name());

			now_speed_list.push_back(0.0);
			current_pause_time_list.push_back(rest_pause_time);
			rest_pause_time -= SERVICE_INTERVAL;
		}
				
		std::vector<std::shared_ptr<Map::BasicPoi const>>::iterator next_poi = order_visited_poi.first.begin() + 1;

		//--------------��ڈȍ~�̓_������Dfor�ŁC�m�ۂ�����poi�̌��������[�v������-------------------
		for (int i = 1; i < POI_NUM; i++)
		{	

			double moving_time_between_poi_and_next_poi = map->calc_necessary_time((*now_poi)->get_id(), (*next_poi)->get_id(), speed);
			double next_arrive_time = moving_time_between_poi_and_next_poi + pause_time;
						
			std::vector<Graph::MapNodeIndicator> shortests_path_between_pois = map->get_shortest_path((*now_poi)->get_id(), (*next_poi)->get_id());
			std::vector<Graph::MapNodeIndicator>::iterator path_iter = shortests_path_between_pois.begin();//path���������邽�߂�index
			
			//�ŏ�������~���Ԃ�phase�Ɋ��Z�������̗]���time�Ƃ��C����ȊO��service_interval��time�Ƃ��āC���ݒn���狁�߂����n�_��distance���v�Z
			//double distance = (SERVICE_INTERVAL - variable_of_converted_pause_time_to_phase.rem) * speed;
			double distance = SERVICE_INTERVAL * speed;
			double distance_between_now_and_next_poi = map->shortest_distance((*now_poi)->get_id(), (*next_poi)->get_id());

			Graph::MapNodeIndicator nearest_position = (*now_poi)->get_id();

			//path���쐬�D�ꏊ�͈�ԋ߂�intersection���m�Ő��`��Ԃ���DMapNodeIndicator��Type��INVALID�Ƃ���D
			while (distance < distance_between_now_and_next_poi) {
				//�ŒZ�H�̒��ň�ԋ߂�intersection��T���C���`��Ԃ���D		
				while (distance > map->shortest_distance((*now_poi)->get_id(), *path_iter))
				{
					nearest_position = *path_iter;
					path_iter++;
				}

				double distance_between_start_and_nearest_position = map->shortest_distance((*now_poi)->get_id(), nearest_position);
				double distance_between_nearest_intersection_and_arrive_position = distance - distance_between_start_and_nearest_position;

				Geography::LatLng nearest_latlng
					= nearest_position.type() == Graph::NodeType::POI ? map->get_static_poi(nearest_position.id())->data->get_position() : *map->get_static_node(nearest_position.id())->data;
				Geography::LatLng next_nearest_latlang
					= (*path_iter).type() == Graph::NodeType::POI ? map->get_static_poi((*path_iter).id())->data->get_position() : *map->get_static_node((*path_iter).id())->data;
				double angle = Geography::GeoCalculation::lambert_azimuth_angle(nearest_latlng, next_nearest_latlang);

				Geography::LatLng arrive_position = Geography::GeoCalculation::calc_translated_point(nearest_latlng, distance_between_nearest_intersection_and_arrive_position, angle);

				//�o�^
				positions->push_back(std::make_shared<Geography::LatLng>(arrive_position));
				visited_node_ids->push_back(Graph::MapNodeIndicator(Graph::NodeType::OTHERS, Graph::NodeType::OTHERS));
				venue_names->push_back("road");
				now_speed_list.push_back(speed);
				current_pause_time_list.push_back(0.0);
				
				distance += SERVICE_INTERVAL * speed;
				phase_id++;				
			}
			//destination�̂Ƃ���܂ŕ⊮�ł�����Crest_time��ێ����Ă����I
			double distance_between_arrive_position_and_dest_position = distance - distance_between_now_and_next_poi;
			
			//���̏���F�X���߂�
			pause_time = generator.uniform_distribution(MIN_PAUSE_TIME, MAX_PAUSE_TIME);
			speed = generator.uniform_distribution(MIN_SPEED, MAX_SPEED);
			rest_pause_time = pause_time - dest_rest_time;
			variable_of_converted_pause_time_to_phase = std::lldiv(rest_pause_time, SERVICE_INTERVAL);
			dest_rest_time = distance_between_arrive_position_and_dest_position / speed;


			//visited_poi_info��	����o�^
			visited_poi_info.visited_poi = std::make_pair(Graph::MapNodeIndicator((*next_poi)->get_id(), Graph::POI), (*next_poi)->data->get_position());
			while (!visited_poi_info.pause_phases.empty()) visited_poi_info.pause_phases.pop_back();
			for (int id = 0; id <= variable_of_converted_pause_time_to_phase.quot; id++) visited_poi_info.pause_phases.push_back(phase_id + id);
			visited_poi_info.arrive_phase = phase_id;
			visited_poi_info.pause_time = pause_time;
			visited_poi_info.starting_speed = speed;
			visited_poi_info.rest_pause_time_when_departing = variable_of_converted_pause_time_to_phase.rem;
			visited_poi_info.dest_rest_time = dest_rest_time;
			visited_pois_info_list.push_back(visited_poi_info);


			//�ʒu�Cnode_ids, venue_name�C��~���ԁC��~�t�F�[�Y��o�^
			for (int i = 0; i <= variable_of_converted_pause_time_to_phase.quot; i++, phase_id++)
			{
				positions->push_back(std::make_shared<Geography::LatLng>((*next_poi)->data->get_position()));
				visited_node_ids->push_back(Graph::MapNodeIndicator((*next_poi)->get_id(), Graph::POI));
				venue_names->push_back((*next_poi)->name());

				now_speed_list.push_back(0.0);
				current_pause_time_list.push_back(rest_pause_time);
				rest_pause_time -= SERVICE_INTERVAL;
			}

			now_poi++;
			next_poi++;
		}
		//�Ō��timeslots�̍쐬�{end_time�̍쐬
		std::unique_ptr<std::vector<time_t>> timeslots = std::make_unique<std::vector<time_t>>();
		int time = 0;
		for (int id = 0; id < phase_id; id++, time += SERVICE_INTERVAL) {
			timeslots->push_back(time);
		}
		std::shared_ptr<Time::TimeSlotManager const> timeslot = std::make_shared<Time::TimeSlotManager>(std::move(timeslots));
		end_time = time - SERVICE_INTERVAL;

		std::shared_ptr<Graph::RevisableTrajectory<Geography::LatLng>> trajectory
			= std::make_shared<Graph::RevisableTrajectory<Geography::LatLng>>(timeslot, visited_node_ids, positions, venue_names);
		return std::make_shared<Entity::RevisablePauseMobileEntity<Geography::LatLng>>(0, trajectory, visited_pois_info_list, current_pause_time_list, now_speed_list);
	}

	
	///<summary>
	/// ���[�U�ƃ_�~�[��trajectory���o�͂���
	///</summary>
	void HayashidaSimulator::export_users_and_dummies_trajectory(std::shared_ptr<Entity::EntityManager<Geography::LatLng, Graph::RevisableTrajectory<Geography::LatLng>, Entity::RevisablePauseMobileEntity<Geography::LatLng>, Entity::DifferentMovementUser<Geography::LatLng>>> entities, std::shared_ptr<Requirement::KatoMethodRequirement const> requirement, std::shared_ptr<Time::Timer> timer)
	{
		static constexpr auto REAL_USER_TRAJECTORY_OUT_PATH = "C:/Users/Shuhei/Desktop/Result_Path/real_user_trajectory";
		static constexpr auto PREDICTED_USER_TRAJECTORY_OUT_PATH = "C:/Users/Shuhei/Desktop/Result_Path/predicted_user_trajectory";
		static constexpr auto REAL_USER_VISITED_POIS_OUT_PATH = "C:/Users/Shuhei/Desktop/Result_Path/real_user_visited_pois.txt";
		static constexpr auto PREDICTED_USER_VISITED_POIS_OUT_PATH = "C:/Users/Shuhei/Desktop/Result_Path/predicted_user_visited_pois.txt";
		static constexpr auto DUMMIES_VISITED_POIS_OUT_PATH = "C:/Users/Shuhei/Desktop/Result_Path/dummies_visited_pois.txt";
		static constexpr auto DUMMY_TRAJECTORT_OUT_PATH = "C:/Users/Shuhei/Desktop/Result_Path/dummy_trajectory";
		static constexpr auto AR_OUT_PATH = "C:/Users/Shuhei/Desktop/Result_Path/AR.txt";

		std::list<std::pair<std::string, std::string>> export_name_map_of_entities = {
			{ Geography::LatLng::LATITUDE, "�ܓx" },
			{ Geography::LatLng::LONGITUDE, "�o�x" },
			//{ Graph::TrajectoryState<>::TIME, "�^�C���X�^���v" },
			//{ Graph::SemanticTrajectoryState<>::CATEGORY, "�J�e�S��ID" },
			//{ Graph::SemanticTrajectoryState<>::CATEGORY_NAME, "�J�e�S����" },
			//{ Graph::TrajectoryState<>::VENUE_NAME, "POI��" }
		};

		//���ۂ̃��[�U�g���W�F�N�g���̃G�N�X�|�[�g
		IO::FileExporter real_user_exporter(export_name_map_of_entities, REAL_USER_TRAJECTORY_OUT_PATH);
		std::list<std::shared_ptr<IO::FileExportable const>> real_user_trajectory = entities->get_user()->get_real_user()->read_trajectory()->get_export_data();
		real_user_exporter.export_lines(real_user_trajectory);


		//�\���������[�U�g���W�F�N�g���̃G�N�X�|�[�g
		IO::FileExporter predicted_user_exporter(export_name_map_of_entities, PREDICTED_USER_TRAJECTORY_OUT_PATH);
		std::list<std::shared_ptr<IO::FileExportable const>> predicted_user_trajectory = entities->get_user()->read_trajectory()->get_export_data();
		predicted_user_exporter.export_lines(predicted_user_trajectory);
		

		/*
		//���ۂ̃��[�U�̖K��POI�̃G�N�X�|�[�g
		std::ofstream ofs_of_real_user_visited_pois(REAL_USER_VISITED_POIS_OUT_PATH);
		for (int id = 0; real_user->get_visited_pois_num(); id++) {
			Graph::node_id poi_id = entities->get_user()->get_real_user()->get_any_visited_poi(id).first.id();
			ofs_of_real_user_visited_pois << poi_id << std::endl;
		}
		
		//�\�z���[�U�̖K��POI�̃G�N�X�|�[�g
		std::ofstream ofs_of_predicted_user_visited_pois(REAL_USER_VISITED_POIS_OUT_PATH);
		for (int id = 0; real_user->get_visited_pois_num(); id++) {
			Graph::node_id poi_id = entities->get_user()->get_predicted_user()->get_any_visited_poi(id).first.id();
			ofs_of_predicted_user_visited_pois << poi_id << std::endl;
		}

		//�S�_�~�[�̖K��POI�̃G�N�X�|�[�g
		std::ofstream ofs_of_dummies_visited_pois(DUMMIES_VISITED_POIS_OUT_PATH);
		for (int dummy_id = 1; dummy_id < requirement->dummy_num; dummy_id++) {
			for (int id = 0; real_user->get_visited_pois_num(); id++) {
				Graph::node_id poi_id = entities->get_dummy_by_id(dummy_id)->get_any_visited_poi(id).first.id();
				ofs_of_dummies_visited_pois << poi_id << std::endl;
			}
			ofs_of_dummies_visited_pois << "?n";
		}

		//�e�_�~�[�̃G�N�X�|�[�g
		for (int dummy_id = 1; dummy_id <= entities->get_dummy_count(); dummy_id++) {
			std::shared_ptr<Entity::RevisablePauseMobileEntity<> const> dummy = entities->read_dummy_by_id(dummy_id);
			std::list<std::shared_ptr<IO::FileExportable const>> dummy_trajectory = dummy->read_trajectory()->get_export_data();
			IO::FileExporter dummy_exporter(export_name_map_of_entities, DUMMY_TRAJECTORT_OUT_PATH + std::to_string(dummy_id));
			dummy_exporter.export_lines(dummy_trajectory);
		}
		*/

		//AR�n�̕]��
		int achive_count = 0;
		double achive_size = 0.0;
		int phase_count = this->time_manager->phase_count();
		for (int phase = 0; phase < phase_count; phase++) {
			double ar = entities->calc_convex_hull_size_of_fixed_entities_of_phase(phase);
			if (ar >= requirement->required_anonymous_area) achive_count++;
			achive_size += ar / requirement->required_anonymous_area;
		}
		double ar_count = (double)achive_count / phase_count;
		double ar_size = achive_size / phase_count;

		std::ofstream ofs(AR_OUT_PATH);
		ofs << "AR-Count: " << std::to_string(ar_count) << std::endl;
		ofs << "AR-Size: " << std::to_string(ar_size) << "m^2" << std::endl;


	}

	///<summary>
	/// input_list��ǂݎ���p�Ŏ擾
	///</summary>
	std::vector<std::shared_ptr<Map::BasicPoi const>> HayashidaSimulator::read_input_poi_list() const
	{
		return input_poi_list;
	}

	///<summary>
	/// map_data�𐶐�����
	///</summary>
	void HayashidaSimulator::build_map(const Graph::Rectangle<Geography::LatLng>& boundary)
	{
		map = std::make_shared<Map::HayashidaDbMap>(std::make_shared<Graph::Dijkstra<Map::BasicMapNode, Map::BasicRoad>>(), "../settings/mydbsettings.xml", "map_tokyo");
		map->load(boundary);
	}


	///<summary>
	/// ���[�U�𐶐�����
	///</summary>
	void HayashidaSimulator::create_trajectories()
	{
		input_visit_pois();

		std::pair<std::vector<std::shared_ptr<Map::BasicPoi const>>, double> order_visited_poi_of_real_user
			= traveling_salesman_problem(input_poi_list); 
			//the_power_alpha_of_the_reciprocal_of_the_ratio_of_the_optimal_distance(input_poi_list, 1);
		real_user = create_artificial_user(order_visited_poi_of_real_user);
		std::cout << "Success Creating real User" << std::endl;


		std::pair<std::vector<std::shared_ptr<Map::BasicPoi const>>, double> order_visited_poi_of_predicted_user = traveling_salesman_problem(input_poi_list);
		predicted_user = create_artificial_user(order_visited_poi_of_predicted_user);
		std::cout << "Success Creating predicted User" << std::endl;


		//����time_manager��predicted��timeslot�ɂ���I
		time_manager = create_time_manager();

		user = std::make_shared<Entity::DifferentMovementUser<Geography::LatLng>>(0, time_manager, real_user, predicted_user);
	}



	///<summary>
	/// �v���p�����[�^���쐬�D
	/// �R���X�g���N�^�ŋ�̓I�Ȓl�����邱��
	///</summary>
	void HayashidaSimulator::make_requirement_list()
	{
		//���������@�̗v���p�����[�^
		//�R���X�g���N�^���쐬����
		/*-------------���v���p�����[�^---------------
			double required_anonymous_area,
			size_t dummy_num,
			int service_interval,
			int interval_of_base_phase,
			int cycle_of_interval_of_base_phase,
			int max_pause_time = 600,
			int min_pause_time = 300,
			double average_speed = 1.5,
			double speed_range = 0.5
		---------------------------------------------*/

		//�����̈挈��̎����̃p�����[�^�̓��[�U�̑��ړ�����/����T�I

		requirements = 
		{
			std::make_shared<Requirement::KatoMethodRequirement>(600 * 600, 2, 90, 5, 2, 600, 250)
		};
	}

	///<summary>
	/// map,user,requirement�̍쐬
	///</summary>
	void HayashidaSimulator::prepare()
	{
		build_map(map_boundary);
		create_trajectories();
		make_requirement_list();
	}

	///<summary>
	/// ���s
	///</summary>
	void HayashidaSimulator::run()
	{
		
		for (std::list<std::shared_ptr<Requirement::KatoMethodRequirement const>>::iterator requirement = requirements.begin(); requirement != requirements.end(); requirement++)
		{
			/*
			//�������񑲘_��@
			Method::KatoBachelorMethod kato_bachelor_method(map,user,*requirement,time_manager);
			kato_bachelor_method.set_execution_callback(std::bind(&HayashidaSimulator::export_users_and_dummies_trajectory,this,std::placeholders::_1, std::placeholders::_2, std::placeholders::_3));
			kato_bachelor_method.run();
			*/
			
			/*
			//��������C�_��@
			Method::KatoMasterMethod kato_master_method(map, user, *requirement, time_manager);
			kato_master_method.set_execution_callback(std::bind(&HayashidaSimulator::export_users_and_dummies_trajectory, this, std::placeholders::_1, std::placeholders::_2, std::placeholders::_3));
			kato_master_method.run();
			*/

			//�ѓc���񑲘_��@
			Method::HayashidaBachelorMethod hayashida_bachelor_method(map, user, *requirement, time_manager);
			hayashida_bachelor_method.set_execution_callback(std::bind(&HayashidaSimulator::export_users_and_dummies_trajectory, this, std::placeholders::_1, std::placeholders::_2, std::placeholders::_3));
			hayashida_bachelor_method.run();
		}
	}

	///<summary>
	/// �]����������
	/// ������Map��User�����g���ďo�������������Ύ�������
	///</summary>


	///<summary>
	/// user�̃g���W�F�N�g���t�@�C���G�N�X�|�[�g
	///</summary>
	void HayashidaSimulator::export_evaluation_result(const Requirement::KatoMethodRequirement& requirement)
	{
	

		/*
		IO::FileExporter user_exporter({
			{Geography::LatLng::LATITUDE, "�ܓx"},
			{Geography::LatLng::LONGITUDE, "�o�x"},
			{Graph::TrajectoryState<>::TIME, "�^�C���X�^���v" }
		}, USER_TRAJECTORY_OUT_PATH);

		
		std::list<std::shared_ptr<IO::FileExportable const>> user_exportable_positions;
		
		time_manager->for_each_time([&](time_t time, long interval, int phase) {
			user_exportable_positions.push_back(user->read_position_of_phase(phase));
		});
		
		user_exporter.export_lines(user_exportable_positions);*/

		/*
		entities->for_each_dummy([&](int dummy_id, std::shared_ptr<Entity::RevisablePauseMobileEntity<Geography::LatLng>> dummy) {
		std::string file_name = DUMMY_TRAJECTORT_OUT_PATH + std::to_string(dummy_id);
		std::cout << file_name << std::endl;
		IO::FileExporter dummies_exporter({
		{ Geography::LatLng::LATITUDE, "�ܓx" },
		{ Geography::LatLng::LONGITUDE, "�o�x" }
		}, file_name);

		std::list<std::shared_ptr<IO::FileExportable const>> dummy_exportable_positions;
		time_manager->for_each_time([&](time_t time, long interval, int phase) {
		dummy_exportable_positions.push_back(entities->read_dummy_by_id(dummy_id)->read_position_of_phase(phase));
		});
		dummies_exporter.export_lines(dummy_exportable_positions);

		});
		*/
	}

	
}