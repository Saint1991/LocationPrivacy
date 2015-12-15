#include "stdafx.h"
#include "HayashidaSimulator.h"

namespace Simulation
{
	///<summary>
	/// �R���X�g���N�^
	///</summary>
	HayashidaSimulator::HayashidaSimulator(const Graph::Rectangle<Geography::LatLng>& boundary) 
		: ISimulator<Map::BasicDbMap, Entity::PauseMobileEntity<Geography::LatLng>, Entity::RevisablePauseMobileEntity<Geography::LatLng>, Requirement::KatoMethodRequirement, Geography::LatLng, Graph::RevisableTrajectory<Geography::LatLng>>(), map_boundary(boundary)
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
	/// time_manager�̐���
	///</summary>
	std::shared_ptr<Time::TimeSlotManager> HayashidaSimulator::create_time_manager() {
		std::unique_ptr<std::vector<time_t>> timeslots = std::make_unique<std::vector<time_t>>();
		for (int time = 0; time <= end_time; time += SERVICE_INTERVAL) {
			timeslots->push_back(time);
		}

		return std::make_shared<Time::TimeSlotManager>(std::move(timeslots));
	}

	///<summary>
	/// ��~���ԂƑ��x0�̃Z�b�g
	///</summary>
	void HayashidaSimulator::set_pause_time_and_phases_of_visited_POI(int *phase_id, div_t variable_of_converted_pause_time_to_phase, std::vector<std::shared_ptr<Map::BasicPoi const>>::iterator& now_poi) {
		double rest_pause_time = SERVICE_INTERVAL * variable_of_converted_pause_time_to_phase.quot + variable_of_converted_pause_time_to_phase.rem;
		user->set_now_pause_time(*phase_id, rest_pause_time);

		for (int i = 0; i < variable_of_converted_pause_time_to_phase.quot; i++)
		{
			if (*phase_id == time_manager->phase_count() - 1) break;
			(*phase_id)++;
			user->set_pause_phases(*phase_id);
			rest_pause_time -= SERVICE_INTERVAL;
			user->set_now_pause_time(*phase_id, rest_pause_time);
			user->set_position_of_phase(*phase_id, (*now_poi)->get_id(), (*now_poi)->data->get_position());
		}
	}
		

	///<summary>
	/// input����poi��list���쐬
	///</summary>
	void HayashidaSimulator::input_visit_pois() {
		//POI�n����쐬����ۂ́C�I��͈�
		Graph::Rectangle<Geography::LatLng> poi_range(BASE_LAT + 0.5*length_of_all_POI_rect, BASE_LNG - 0.5*length_of_all_POI_rect, BASE_LAT - 0.5*length_of_all_POI_rect, BASE_LNG + 0.5*length_of_all_POI_rect);

		std::vector<std::shared_ptr<Map::BasicPoi const>> random_pois_list = get_pois_list(poi_range);
		std::vector<std::shared_ptr<Map::BasicPoi const>>::iterator iter = random_pois_list.begin();
		for (int i = 0; i < POI_NUM; i++, iter++) {
			input_poi_list.push_back(*iter);
		}
	}

	///<summary>
	/// input_elements�̓���POI����ɁC����Z�[���X�}���������C���������������Ƀ\�[�g����vector��Ԃ��D
	///</summary>
	std::vector<std::vector<std::shared_ptr<Map::BasicPoi const>>> HayashidaSimulator::all_traveling_salesman_problem(std::vector<std::shared_ptr<Map::BasicPoi const>>& visited_pois)
	{
		std::vector<std::vector<std::shared_ptr<Map::BasicPoi const>>> all_tsp_solution;
		return all_tsp_solution;
	}


	///<summary>
	/// input_elements�̓���POI����ɁC����Z�[���X�}�����������C�œK����Ԃ��D
	///</summary>
	std::vector<std::shared_ptr<Map::BasicPoi const>> HayashidaSimulator::traveling_salesman_problem(std::vector<std::shared_ptr<Map::BasicPoi const>>& visited_pois)
	{
		return all_traveling_salesman_problem(visited_pois).front();
	}

	///<summary>
	/// input_elements�̓���POI����ɁC����Z�[���X�}�����������C�K�E�X���z�ɏ]���Ĉ��m���ŋ��߂�poi�n���Ԃ��D
	///</summary>
	std::vector<std::shared_ptr<Map::BasicPoi const>> HayashidaSimulator::Gaussian_distribution(std::vector<std::shared_ptr<Map::BasicPoi const>>& visited_pois)
	{
		std::vector<std::vector<std::shared_ptr<Map::BasicPoi const>>> all_tsp_solution = all_traveling_salesman_problem(visited_pois);
		std::vector<std::shared_ptr<Map::BasicPoi const>> gaussian_pois;
		
		return gaussian_pois;
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
	 
		//----------------------------time_manager��user�̐���-------------------------------------//
		time_manager = create_time_manager();
		user = std::make_shared<Entity::PauseMobileEntity<Geography::LatLng>>(0, time_manager);
		
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

			double moving_time_between_poi_and_next_poi = map->calc_necessary_time((*now_poi)->get_id(), (*next_poi)->get_id(), user->get_now_speed(phase_id));
			int next_arrive_time = moving_time_between_poi_and_next_poi + user->get_pause_time();

			int rest_pause_time = user->get_pause_time() - dest_rest_time;

			//��~���Ԃ�phase�Ɋ��Z���Cpause_time�ƍŒZ�H�o�H����path�����肵�Ă���
			div_t variable_of_converted_pause_time_to_phase = std::div(rest_pause_time, SERVICE_INTERVAL);
			
			//�o������Ƃ��̗]�莞�Ԃ�o�^
			user->set_rest_pause_time_when_departing(variable_of_converted_pause_time_to_phase.rem);

			//��~���ԕ��C�ephase�ɒ�~�ꏊ�ƈړ����x(0)��o�^
			set_pause_time_and_phases_of_visited_POI(&phase_id, variable_of_converted_pause_time_to_phase, now_poi);
			

			std::vector<Graph::MapNodeIndicator> shortests_path_between_pois = map->get_shortest_path((*now_poi)->get_id(), (*next_poi)->get_id());
			std::vector<Graph::MapNodeIndicator>::iterator path_iter = shortests_path_between_pois.begin();//path���������邽�߂�index
			//���x��phase�Ŗ��߂�O���Q�Ƃ��Ȃ���΂Ȃ�Ȃ����Ƃɒ���
			double pause_position_speed = user->get_starting_speed();
			
			//�ŏ�������~���Ԃ�phase�Ɋ��Z�������̗]���time�Ƃ��C����ȊO��service_interval��time�Ƃ��āC���ݒn���狁�߂����n�_��distance���v�Z
			double distance = (SERVICE_INTERVAL - variable_of_converted_pause_time_to_phase.rem) * pause_position_speed;
			double distance_between_now_and_next_poi = map->shortest_distance((*now_poi)->get_id(), (*next_poi)->get_id());

			Graph::MapNodeIndicator nearest_position = (*now_poi)->get_id();

			//path���쐬�D�ꏊ�͈�ԋ߂�intersection���m�Ő��`��Ԃ���DMapNodeIndicator��Type��INVALID�Ƃ���D
			while (distance < distance_between_now_and_next_poi) {
				//�ŒZ�H�̒��ň�ԋ߂�intersection��T���C���`��Ԃ���D		
				set_path_between_poi(now_poi, path_iter, nearest_position, pause_position_speed, SERVICE_INTERVAL, &distance, &phase_id);
			}
			
			//destination�̂Ƃ���܂ŕ⊮�ł�����Crest_time��ێ����Ă����I
			double distance_between_arrive_position_and_dest_position = distance - distance_between_now_and_next_poi;
			dest_rest_time = distance_between_arrive_position_and_dest_position / pause_position_speed;
			//�ړI�n�̓o�^
			phase_id++;
			user->increment_visited_pois_info_list_id();
			user->set_visited_poi_of_phase(phase_id, (*next_poi)->get_id(), map->get_static_poi((*next_poi)->get_id())->data->get_position());
			user->set_random_starting_speed_at_poi(AVERAGE_SPEED, RANGE_OF_SPEED);

			//���݈ʒu�̍X�V
			*now_poi = *next_poi;

		}

		//---------------------------------end_time�܂œK���Ɍo�H�����߂�I---------------------------------------------------

		//�ŏI�n�_�͏��������ɂƂ�(1.5�{�`2�{)�D�������C�}�b�v�̌��E�͈͂ɒ���
		double last_distance = 1.3 * (end_time - time_manager->time_of_phase(phase_id)) * user->get_now_speed(phase_id);

		//���̌��_�͈̔͂����߂�
		double last_angle = generator.uniform_distribution(-(M_PI), M_PI_2);
		Geography::LatLng last_candidate_poi_position_range
			= Geography::GeoCalculation::calc_translated_point((*now_poi)->data->get_position(), last_distance, last_angle);

		Graph::Rectangle<Geography::LatLng> last_range(last_candidate_poi_position_range.lat() + 0.001, last_candidate_poi_position_range.lng(), last_candidate_poi_position_range.lat(), last_candidate_poi_position_range.lng() + 0.001);

		std::vector<std::shared_ptr<Map::BasicPoi const>> last_candidate_pois_list = get_pois_list(last_range);
		//����POI�̌���
		std::vector<std::shared_ptr<Map::BasicPoi const>>::iterator last_poi = last_candidate_pois_list.begin();
		
		//���ݒn�̒�~���Ԃ������_���Őݒ肵�C���n�_�̏o���n�̑��x�ŁC����POI�܂ł̍ŒZ�H�ňړ��������̎��Ԃ����߂�D
		user->set_random_pause_time(phase_id, MIN_PAUSE_TIME, MAX_PAUSE_TIME);
		double moving_time_between_poi_and_next_poi = map->calc_necessary_time((*now_poi)->get_id(), (*last_poi)->get_id(), user->get_now_speed(phase_id));
		int next_arrive_time = moving_time_between_poi_and_next_poi + user->get_pause_time();

		
		//��~���Ԃ�phase�Ɋ��Z���Cpause_time�ƍŒZ�H�o�H����path�����肵�Ă���
		int rest_pause_time = user->get_pause_time() - dest_rest_time;
		div_t last_variable_of_converted_pause_time_to_phase = std::div(rest_pause_time, SERVICE_INTERVAL);

		std::vector<Graph::MapNodeIndicator> last_shortests_path = map->get_shortest_path((*now_poi)->get_id(), (*last_poi)->get_id());

		//��~���ԕ��C�ephase�ɒ�~�ꏊ�ƈړ����x(0)��o�^
		set_pause_time_and_phases_of_visited_POI(&phase_id, last_variable_of_converted_pause_time_to_phase, now_poi);

		std::vector<Graph::MapNodeIndicator>::iterator last_path_iter = last_shortests_path.begin();//path���������邽�߂�index
		//���x��phase�Ŗ��߂�O���Q�Ƃ��Ȃ���΂Ȃ�Ȃ����Ƃɒ���
		double last_pause_position_speed = user->get_now_speed(phase_id - last_variable_of_converted_pause_time_to_phase.quot);

		//�ŏ�������~���Ԃ�phase�Ɋ��Z�������̗]���time�Ƃ��C����ȊO��service_interval��time�Ƃ��āC���ݒn���狁�߂����n�_��distance���v�Z
		double distance = (SERVICE_INTERVAL - last_variable_of_converted_pause_time_to_phase.rem) * last_pause_position_speed;
		double distance_between_now_and_next_poi = map->shortest_distance((*now_poi)->get_id(), (*last_poi)->get_id());

		Graph::MapNodeIndicator last_nearest_position = (*now_poi)->get_id();

		//path���쐬�D�ꏊ�͈�ԋ߂�intersection���m�Ő��`��Ԃ���DMapNodeIndicator��Type��INVALID�Ƃ���D
		//last_phase�܂Ŗ��߂�I
		while (phase_id != (time_manager->phase_count() - 1)) {
			set_path_between_poi(now_poi, last_path_iter, last_nearest_position, last_pause_position_speed, SERVICE_INTERVAL, &distance, &phase_id);
		}

		std::cout << "Success Creating Random User" << std::endl;
	}

	

	///<summary>
	/// input_pois����ɁCt.s.p.��p���C�\�z���[�U���쐬����D
	///</summary>
	void HayashidaSimulator::make_predicted_user() {
		
		time_manager = create_time_manager();//time_manager�̐���
		user = std::make_shared<Entity::PauseMobileEntity<Geography::LatLng>>(0, time_manager);
				
		input_visit_pois();//user��input��������

		//---------------------------POI�K�⏇��������---------------------------------------------
		int phase_id = 0;
		
		//user��visited_pois��p���āC�n�_��start_poi�̏���Z�[���X�}����������
		std::vector<std::shared_ptr<Map::BasicPoi const>> order_visited_poi = traveling_salesman_problem(input_poi_list);
		
		int dest_rest_time = 0;//phase�̓������ԂƎ��ۂ̓������Ԃ̍���.�ŏ�����0
		//�ŏ��̓_��o�^
		std::vector<std::shared_ptr<Map::BasicPoi const>>::iterator now_poi = order_visited_poi.begin();
		std::vector<std::shared_ptr<Map::BasicPoi const>>::iterator next_poi = order_visited_poi.begin() + 1;

		user->set_position_of_phase(phase_id, Graph::MapNodeIndicator((*now_poi)->get_id()), (*now_poi)->data->get_position());
		user->set_random_now_speed(phase_id, AVERAGE_SPEED, RANGE_OF_SPEED);
		user->set_random_pause_time(phase_id, MIN_PAUSE_TIME, MAX_PAUSE_TIME);
     	//--------------��ڈȍ~�̓_������Dfor�ŁC�m�ۂ�����poi�̌��������[�v������-------------------
		for (int i = 1; i < POI_NUM; i++)
		{
			double moving_time_between_poi_and_next_poi = map->calc_necessary_time((*now_poi)->get_id(), (*next_poi)->get_id(), user->get_now_speed(phase_id));
			int next_arrive_time = moving_time_between_poi_and_next_poi + user->get_pause_time();

			int rest_pause_time = user->get_pause_time() - dest_rest_time;

			//��~���Ԃ�phase�Ɋ��Z���Cpause_time�ƍŒZ�H�o�H����path�����肵�Ă���
			div_t variable_of_converted_pause_time_to_phase = std::div(rest_pause_time, SERVICE_INTERVAL);

			//��~���ԕ��C�ephase�ɒ�~�ꏊ�ƈړ����x(0)��o�^
			set_pause_time_and_phases_of_visited_POI(&phase_id, variable_of_converted_pause_time_to_phase, now_poi);

			std::vector<Graph::MapNodeIndicator> shortests_path_between_pois = map->get_shortest_path((*now_poi)->get_id(), (*next_poi)->get_id());
			std::vector<Graph::MapNodeIndicator>::iterator path_iter = shortests_path_between_pois.begin();//path���������邽�߂�index
			//���x��phase�Ŗ��߂�O���Q�Ƃ��Ȃ���΂Ȃ�Ȃ����Ƃɒ���
			double pause_position_speed = user->get_now_speed(phase_id - variable_of_converted_pause_time_to_phase.quot);

			//�ŏ�������~���Ԃ�phase�Ɋ��Z�������̗]���time�Ƃ��C����ȊO��service_interval��time�Ƃ��āC���ݒn���狁�߂����n�_��distance���v�Z
			double distance = (SERVICE_INTERVAL - variable_of_converted_pause_time_to_phase.rem) * pause_position_speed;
			double distance_between_now_and_next_poi = map->shortest_distance((*now_poi)->get_id(), (*next_poi)->get_id());

			Graph::MapNodeIndicator nearest_position = (*now_poi)->get_id();

			//path���쐬�D�ꏊ�͈�ԋ߂�intersection���m�Ő��`��Ԃ���DMapNodeIndicator��Type��INVALID�Ƃ���D
			while (distance < distance_between_now_and_next_poi) {
				//�ŒZ�H�̒��ň�ԋ߂�intersection��T���C���`��Ԃ���D		
				set_path_between_poi(now_poi, path_iter, nearest_position, pause_position_speed, SERVICE_INTERVAL, &distance, &phase_id);
			}

			//destination�̂Ƃ���܂ŕ⊮�ł�����Crest_time��ێ����Ă����I
			double distance_between_arrive_position_and_dest_position = distance - distance_between_now_and_next_poi;
			dest_rest_time = distance_between_arrive_position_and_dest_position / pause_position_speed;
			
			//�ړI�n�̓o�^
			phase_id++;
			user->set_position_of_phase(phase_id, (*next_poi)->get_id(), map->get_static_poi((*next_poi)->get_id())->data->get_position());
			user->set_random_pause_time(phase_id, MIN_PAUSE_TIME, MAX_PAUSE_TIME);
			user->set_random_now_speed(phase_id, AVERAGE_SPEED, RANGE_OF_SPEED);

			now_poi++;
			next_poi++;
		}

		//---------------------------------end_time�܂œK���Ɍo�H�����߂�I---------------------------------------------------

		Math::Probability generator;
		//�ŏI�n�_�͏��������ɂƂ�(1.5�{�`2�{)�D�������C�}�b�v�̌��E�͈͂ɒ���
		double last_distance = 1.3 * (end_time - time_manager->time_of_phase(phase_id)) * user->get_now_speed(phase_id);

		//���̌��_�͈̔͂����߂�
		double last_angle = generator.uniform_distribution(-(M_PI), M_PI_2);
		Geography::LatLng last_candidate_poi_position_range
			= Geography::GeoCalculation::calc_translated_point((*now_poi)->data->get_position(), last_distance, last_angle);

		Graph::Rectangle<Geography::LatLng> last_range(last_candidate_poi_position_range.lat() + 0.001, last_candidate_poi_position_range.lng(), last_candidate_poi_position_range.lat(), last_candidate_poi_position_range.lng() + 0.001);

		std::vector<std::shared_ptr<Map::BasicPoi const>> last_candidate_pois_list = get_pois_list(last_range);
		//����POI�̌���
		std::vector<std::shared_ptr<Map::BasicPoi const>>::iterator last_poi = last_candidate_pois_list.begin();

		//���ݒn�̒�~���Ԃ������_���Őݒ肵�C���n�_�̏o���n�̑��x�ŁC����POI�܂ł̍ŒZ�H�ňړ��������̎��Ԃ����߂�D
		double moving_time_between_poi_and_next_poi = map->calc_necessary_time((*now_poi)->get_id(), (*last_poi)->get_id(), user->get_now_speed(phase_id));
		int next_arrive_time = moving_time_between_poi_and_next_poi + user->get_pause_time();
		
		//��~���Ԃ�phase�Ɋ��Z���Cpause_time�ƍŒZ�H�o�H����path�����肵�Ă���
		int rest_pause_time = user->get_pause_time() - dest_rest_time;
		div_t last_variable_of_converted_pause_time_to_phase = std::div(rest_pause_time, SERVICE_INTERVAL);

		std::vector<Graph::MapNodeIndicator> last_shortests_path = map->get_shortest_path((*now_poi)->get_id(), (*last_poi)->get_id());

		//��~���ԕ��C�ephase�ɒ�~�ꏊ�ƈړ����x(0)��o�^
		set_pause_time_and_phases_of_visited_POI(&phase_id, last_variable_of_converted_pause_time_to_phase, now_poi);

		std::vector<Graph::MapNodeIndicator>::iterator last_path_iter = last_shortests_path.begin();//path���������邽�߂�index
		//���x��phase�Ŗ��߂�O���Q�Ƃ��Ȃ���΂Ȃ�Ȃ����Ƃɒ���
		double last_pause_position_speed = user->get_now_speed(phase_id - last_variable_of_converted_pause_time_to_phase.quot);

		//�ŏ�������~���Ԃ�phase�Ɋ��Z�������̗]���time�Ƃ��C����ȊO��service_interval��time�Ƃ��āC���ݒn���狁�߂����n�_��distance���v�Z
		double distance = (SERVICE_INTERVAL - last_variable_of_converted_pause_time_to_phase.rem) * last_pause_position_speed;
		double distance_between_now_and_next_poi = map->shortest_distance((*now_poi)->get_id(), (*last_poi)->get_id());

		Graph::MapNodeIndicator last_nearest_position = (*now_poi)->get_id();

		//path���쐬�D�ꏊ�͈�ԋ߂�intersection���m�Ő��`��Ԃ���DMapNodeIndicator��Type��INVALID�Ƃ���D
		//last_phase�܂Ŗ��߂�I
		while (phase_id != (time_manager->phase_count() - 1)) {
			set_path_between_poi(now_poi, last_path_iter, last_nearest_position, last_pause_position_speed, SERVICE_INTERVAL, &distance, &phase_id);
		}

		std::cout << "Success Creating Input User" << std::endl;

	}

	
	///<summary>
	/// input_pois����ɁC���ۂ̃��[�U���쐬����D
	/// t.s.p.�̉��̋������߂����ɐ��K���z�ŏd�ݕt�����āC�m���I�ɕω�������D
	///</summary>
	void HayashidaSimulator::make_real_user() {

		time_manager = create_time_manager();//time_manager�̐���
		user = std::make_shared<Entity::PauseMobileEntity<Geography::LatLng>>(0, time_manager);

		input_visit_pois();//user��input��������

		//---------------------------POI�K�⏇��������---------------------------------------------
		int phase_id = 0;

		//user��visited_pois��p���āC�n�_��start_poi�̏���Z�[���X�}�����̉��̂����C�K�E�X���z�ɏ]���C���錋�ʂ𒊏o����D
		std::vector<std::shared_ptr<Map::BasicPoi const>> order_visited_poi = Gaussian_distribution(input_poi_list);

		int dest_rest_time = 0;//phase�̓������ԂƎ��ۂ̓������Ԃ̍���.�ŏ�����0
		//�ŏ��̓_��o�^
		std::vector<std::shared_ptr<Map::BasicPoi const>>::iterator now_poi = order_visited_poi.begin();
		std::vector<std::shared_ptr<Map::BasicPoi const>>::iterator next_poi = order_visited_poi.begin() + 1;

		user->set_position_of_phase(phase_id, Graph::MapNodeIndicator((*now_poi)->get_id()), (*now_poi)->data->get_position());
		user->set_random_now_speed(phase_id, AVERAGE_SPEED, RANGE_OF_SPEED);
		user->set_random_pause_time(phase_id, MIN_PAUSE_TIME, MAX_PAUSE_TIME);
		//--------------��ڈȍ~�̓_������Dfor�ŁC�m�ۂ�����poi�̌��������[�v������-------------------
		for (int i = 1; i < POI_NUM; i++)
		{
			double moving_time_between_poi_and_next_poi = map->calc_necessary_time((*now_poi)->get_id(), (*next_poi)->get_id(), user->get_now_speed(phase_id));
			int next_arrive_time = moving_time_between_poi_and_next_poi + user->get_pause_time();

			int rest_pause_time = user->get_pause_time() - dest_rest_time;

			//��~���Ԃ�phase�Ɋ��Z���Cpause_time�ƍŒZ�H�o�H����path�����肵�Ă���
			div_t variable_of_converted_pause_time_to_phase = std::div(rest_pause_time, SERVICE_INTERVAL);

			//��~���ԕ��C�ephase�ɒ�~�ꏊ�ƈړ����x(0)��o�^
			set_pause_time_and_phases_of_visited_POI(&phase_id, variable_of_converted_pause_time_to_phase, now_poi);

			std::vector<Graph::MapNodeIndicator> shortests_path_between_pois = map->get_shortest_path((*now_poi)->get_id(), (*next_poi)->get_id());
			std::vector<Graph::MapNodeIndicator>::iterator path_iter = shortests_path_between_pois.begin();//path���������邽�߂�index
																										   //���x��phase�Ŗ��߂�O���Q�Ƃ��Ȃ���΂Ȃ�Ȃ����Ƃɒ���
			double pause_position_speed = user->get_now_speed(phase_id - variable_of_converted_pause_time_to_phase.quot);

			//�ŏ�������~���Ԃ�phase�Ɋ��Z�������̗]���time�Ƃ��C����ȊO��service_interval��time�Ƃ��āC���ݒn���狁�߂����n�_��distance���v�Z
			double distance = (SERVICE_INTERVAL - variable_of_converted_pause_time_to_phase.rem) * pause_position_speed;
			double distance_between_now_and_next_poi = map->shortest_distance((*now_poi)->get_id(), (*next_poi)->get_id());

			Graph::MapNodeIndicator nearest_position = (*now_poi)->get_id();

			//path���쐬�D�ꏊ�͈�ԋ߂�intersection���m�Ő��`��Ԃ���DMapNodeIndicator��Type��INVALID�Ƃ���D
			while (distance < distance_between_now_and_next_poi) {
				//�ŒZ�H�̒��ň�ԋ߂�intersection��T���C���`��Ԃ���D		
				set_path_between_poi(now_poi, path_iter, nearest_position, pause_position_speed, SERVICE_INTERVAL, &distance, &phase_id);
			}

			//destination�̂Ƃ���܂ŕ⊮�ł�����Crest_time��ێ����Ă����I
			double distance_between_arrive_position_and_dest_position = distance - distance_between_now_and_next_poi;
			dest_rest_time = distance_between_arrive_position_and_dest_position / pause_position_speed;

			//�ړI�n�̓o�^
			phase_id++;
			user->set_position_of_phase(phase_id, (*next_poi)->get_id(), map->get_static_poi((*next_poi)->get_id())->data->get_position());
			user->set_random_pause_time(phase_id, MIN_PAUSE_TIME, MAX_PAUSE_TIME);
			user->set_random_now_speed(phase_id, AVERAGE_SPEED, RANGE_OF_SPEED);

			now_poi++;
			next_poi++;
		}

		//---------------------------------end_time�܂œK���Ɍo�H�����߂�I---------------------------------------------------

		Math::Probability generator;
		//�ŏI�n�_�͏��������ɂƂ�(1.5�{�`2�{)�D�������C�}�b�v�̌��E�͈͂ɒ���
		double last_distance = 1.3 * (end_time - time_manager->time_of_phase(phase_id)) * user->get_now_speed(phase_id);

		//���̌��_�͈̔͂����߂�
		double last_angle = generator.uniform_distribution(-(M_PI), M_PI_2);
		Geography::LatLng last_candidate_poi_position_range
			= Geography::GeoCalculation::calc_translated_point((*now_poi)->data->get_position(), last_distance, last_angle);

		Graph::Rectangle<Geography::LatLng> last_range(last_candidate_poi_position_range.lat() + 0.001, last_candidate_poi_position_range.lng(), last_candidate_poi_position_range.lat(), last_candidate_poi_position_range.lng() + 0.001);

		std::vector<std::shared_ptr<Map::BasicPoi const>> last_candidate_pois_list = get_pois_list(last_range);
		//����POI�̌���
		std::vector<std::shared_ptr<Map::BasicPoi const>>::iterator last_poi = last_candidate_pois_list.begin();

		//���ݒn�̒�~���Ԃ������_���Őݒ肵�C���n�_�̏o���n�̑��x�ŁC����POI�܂ł̍ŒZ�H�ňړ��������̎��Ԃ����߂�D
		double moving_time_between_poi_and_next_poi = map->calc_necessary_time((*now_poi)->get_id(), (*last_poi)->get_id(), user->get_now_speed(phase_id));
		int next_arrive_time = moving_time_between_poi_and_next_poi + user->get_pause_time();

		//��~���Ԃ�phase�Ɋ��Z���Cpause_time�ƍŒZ�H�o�H����path�����肵�Ă���
		int rest_pause_time = user->get_pause_time() - dest_rest_time;
		div_t last_variable_of_converted_pause_time_to_phase = std::div(rest_pause_time, SERVICE_INTERVAL);

		std::vector<Graph::MapNodeIndicator> last_shortests_path = map->get_shortest_path((*now_poi)->get_id(), (*last_poi)->get_id());

		//��~���ԕ��C�ephase�ɒ�~�ꏊ�ƈړ����x(0)��o�^
		set_pause_time_and_phases_of_visited_POI(&phase_id, last_variable_of_converted_pause_time_to_phase, now_poi);

		std::vector<Graph::MapNodeIndicator>::iterator last_path_iter = last_shortests_path.begin();//path���������邽�߂�index
																									//���x��phase�Ŗ��߂�O���Q�Ƃ��Ȃ���΂Ȃ�Ȃ����Ƃɒ���
		double last_pause_position_speed = user->get_now_speed(phase_id - last_variable_of_converted_pause_time_to_phase.quot);

		//�ŏ�������~���Ԃ�phase�Ɋ��Z�������̗]���time�Ƃ��C����ȊO��service_interval��time�Ƃ��āC���ݒn���狁�߂����n�_��distance���v�Z
		double distance = (SERVICE_INTERVAL - last_variable_of_converted_pause_time_to_phase.rem) * last_pause_position_speed;
		double distance_between_now_and_next_poi = map->shortest_distance((*now_poi)->get_id(), (*last_poi)->get_id());

		Graph::MapNodeIndicator last_nearest_position = (*now_poi)->get_id();

		//path���쐬�D�ꏊ�͈�ԋ߂�intersection���m�Ő��`��Ԃ���DMapNodeIndicator��Type��INVALID�Ƃ���D
		//last_phase�܂Ŗ��߂�I
		while (phase_id != (time_manager->phase_count() - 1)) {
			set_path_between_poi(now_poi, last_path_iter, last_nearest_position, last_pause_position_speed, SERVICE_INTERVAL, &distance, &phase_id);
		}

		std::cout << "Success Creating Real User" << std::endl;

	}

	
	///<summary>
	/// �_�~�[��trajectory���o�͂���
	///</summary>
	void HayashidaSimulator::export_dummy_trajectory(std::shared_ptr<Entity::EntityManager<Entity::RevisablePauseMobileEntity<Geography::LatLng>, Entity::PauseMobileEntity<Geography::LatLng>, Geography::LatLng>> entities, std::shared_ptr<Time::Timer> timer, int dummy_id)
	{
		IO::FileExporter dummy_exporter({
			{ Geography::LatLng::LATITUDE, "�ܓx" },
			{ Geography::LatLng::LONGITUDE, "�o�x" }
		}, DUMMY_TRAJECTORT_OUT_PATH);

		std::list<std::shared_ptr<IO::FileExportable const>> dummy_exportable_positions;
		time_manager->for_each_time([&](time_t time, long interval, int phase) {
			dummy_exportable_positions.push_back(entities->read_dummy_by_id(dummy_id)->read_position_of_phase(phase));
		});

	}


	///<summary>
	/// �_�~�[��trajectory���o�͂���
	///</summary>
	void HayashidaSimulator::export_dummies_trajectory(std::shared_ptr<Entity::EntityManager<Entity::RevisablePauseMobileEntity<Geography::LatLng>, Entity::PauseMobileEntity<Geography::LatLng>, Geography::LatLng>> entities, std::shared_ptr<Requirement::KatoMethodRequirement const> requirement, std::shared_ptr<Time::Timer> timer)
	{
		
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
	}

	///<summary>
	/// map_data�𐶐�����
	///</summary>
	void HayashidaSimulator::build_map(const Graph::Rectangle<Geography::LatLng>& boundary)
	{
		map = std::make_shared<Map::BasicDbMap>(std::make_shared<Graph::Dijkstra<Map::BasicMapNode, Map::BasicRoad>>(), "../settings/mydbsettings.xml", "map_tokyo");
		map->load(boundary);
	}


	///<summary>
	/// ���[�U�𐶐�����
	///</summary>
	void HayashidaSimulator::create_trajectories()
	{
		make_random_movement_user();
		//make_predicted_user();
	}



	///<summary>
	/// �v���p�����[�^���쐬�D
	/// �R���X�g���N�^�ŋ�̓I�Ȓl�����邱��
	///</summary>
	void HayashidaSimulator::make_requirement_list()
	{
		//�������񑲘_��@�̗v���p�����[�^
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
			std::make_shared<Requirement::KatoMethodRequirement>(600 * 600, 4, 90, 5, 3, 500, 200),
			//std::make_shared<Requirement::KatoMethodRequirement>(1000 * 1000, 16, 90, 5, 2)
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
			Method::KatoBachelorMethod kato_bachelor_method(map,user,*requirement,time_manager);
			kato_bachelor_method.set_execution_callback(std::bind(&HayashidaSimulator::export_dummies_trajectory,this,std::placeholders::_1, std::placeholders::_2, std::placeholders::_3));
			kato_bachelor_method.run();
		}
	}

	///<summary>
	/// �]����������
	///</summary>
	void HayashidaSimulator::evaluate()
	{
	
	}

	///<summary>
	/// user�̃g���W�F�N�g���t�@�C���G�N�X�|�[�g
	///</summary>
	void HayashidaSimulator::export_evaluation_result()
	{
		IO::FileExporter user_exporter({
			{Geography::LatLng::LATITUDE, "�ܓx"},
			{Geography::LatLng::LONGITUDE, "�o�x"}
		}, USER_TRAJECTORY_OUT_PATH);

		std::list<std::shared_ptr<IO::FileExportable const>> user_exportable_positions;
		time_manager->for_each_time([&](time_t time, long interval, int phase) {
			user_exportable_positions.push_back(user->read_position_of_phase(phase));
		});
				
		user_exporter.export_lines(user_exportable_positions);
	}

	
}