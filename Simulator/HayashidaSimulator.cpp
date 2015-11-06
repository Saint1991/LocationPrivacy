#include "stdafx.h"
#include "HayashidaSimulator.h"

namespace Simulation
{
	///<summary>
	/// �R���X�g���N�^
	///</summary>
	HayashidaSimulator::HayashidaSimulator(const Graph::Rectangle<Geography::LatLng>& boundary) 
		: ISimulator<Map::BasicDbMap, Entity::PauseMobileEntity<Geography::LatLng>, Entity::PauseMobileEntity<Geography::LatLng>, Requirement::KatoMethodRequirement, Geography::LatLng, Graph::Trajectory<Geography::LatLng>>(), map_boundary(boundary)
	{
	}

	///<summary>
	/// �f�X�g���N�^
	///</summary>
	HayashidaSimulator::~HayashidaSimulator()
	{
	}

	///<summary>
	/// �����_���Ń��[�U�𐶐�����
	///</summary>
	void HayashidaSimulator::random_user()
	{
		//random�Ɍo�H��ݒ�ł���悤�ɂ������I�I
		//input_user_plan�����
		//get_user���������͂����肳����D
		//time_manager��time���Ăǂ��Őݒ肷��H
		
		//-----���v���p�����[�^�Drequirement�Ɠ����l�ɂ��邱�ƁI��---------//
		const double AVERAGE_SPEED = 1.5;
		const double RANGE_OF_SPEED = 0.5;
		const int MAX_PAUSE_TIME = 600;
		const int MIN_PAUSE_TIME = 60;
		const int SERVICE_INTERVAL = 180;
		const int end_time = 1800;
		const int POI_NUM = 4;
		double length_of_rect = 0.005;//�����ɂ͓K�؂Ȕ͈͓��̈ܓx�o�x��������
		double base_lat = 35.677;//�o���n�̖ڈ��̈ܓx�D�SPOI�̕��ϒl
		double base_lng = 139.715;//�o���n�̖ڈ��̌o�x�D�SPOI�̕��ϒl
		//rect_init_lang�ɂ͎n�_�ɂ������͈͂��C���X�^���X�œ���
		Graph::Rectangle<Geography::LatLng> rect_init_range(base_lat + 0.5*length_of_rect, base_lng - 0.5*length_of_rect, base_lat - 0.5*length_of_rect, base_lng + 0.5*length_of_rect);
	  //-----���v���p�����[�^�Drequirement�Ɠ����l�ɂ��邱�ƁI��---------//

		int phase_id = 0;
		std::unique_ptr<std::vector<time_t>> timeslots = std::make_unique<std::vector<time_t>>();
		for (int time = 0; time <= end_time; time += SERVICE_INTERVAL) {
			timeslots->push_back(time);
		}

		time_manager = std::make_shared<Time::TimeSlotManager>(std::move(timeslots));
		user = std::make_shared<Entity::PauseMobileEntity<Geography::LatLng>>(0, time_manager);

		//---------------------------�ŏ��̓_������---------------------------------------------
		std::vector<std::shared_ptr<Map::BasicPoi const>> random_pois_list = map->find_pois_within_boundary(rect_init_range);
		
		//�����͈͓���POI��������Ȃ�������C�͈͂��L���čČv�Z
		if (random_pois_list.size() == 0) {
			while(random_pois_list.size() == 0){
				length_of_rect += 0.005;
				rect_init_range.top += 0.5*length_of_rect;
				rect_init_range.left -= 0.5*length_of_rect; 
				rect_init_range.bottom -= 0.5*length_of_rect;
				rect_init_range.right += 0.5*length_of_rect;
				random_pois_list = map->find_pois_within_boundary(rect_init_range);
			}
		}
				
		std::random_device device;
		std::mt19937_64 generator(device());
		std::shuffle(random_pois_list.begin(), random_pois_list.end(), generator);

		std::vector<std::shared_ptr<Map::BasicPoi const>>::const_iterator now_poi = random_pois_list.begin();
		user->set_position_of_phase(phase_id, Graph::MapNodeIndicator((*now_poi)->get_id()), (*now_poi)->data->get_position());
		user->set_random_speed(phase_id, AVERAGE_SPEED, RANGE_OF_SPEED);

		int dest_rest_time = 0;//phase�̓������ԂƎ��ۂ̓������Ԃ̍���.�ŏ�����0

		//--------------��ڈȍ~�̓_������Dfor�ŁC�m�ۂ�����poi�̌��������[�v������-------------------
		for (int i = 1; i < POI_NUM; i++)
		{
			//���̌��_�͈̔͂����߂�
			Math::Probability generator;
			double distance_between_positions = generator.uniform_distribution(300.0, 1000.0);
			double angle_of_positions = generator.uniform_distribution(-M_PI_2, M_PI_2);
			Geography::LatLng next_candidate_poi_position_range
				= Geography::GeoCalculation::calc_translated_point((*now_poi)->data->get_position(), distance_between_positions, angle_of_positions);

			Graph::Rectangle<Geography::LatLng> range(next_candidate_poi_position_range.lat() + 0.001, next_candidate_poi_position_range.lng(), next_candidate_poi_position_range.lat(), next_candidate_poi_position_range.lng() + 0.001);
			std::vector<std::shared_ptr<Map::BasicPoi const>> candidate_pois_list = map->find_pois_within_boundary(range);

			if (candidate_pois_list.size() == 0) {
				while (candidate_pois_list.size() == 0) {
					length_of_rect += 0.001;
					range.top += 0.5*length_of_rect;
					range.left -= 0.5*length_of_rect;
					range.bottom -= 0.5*length_of_rect;
					range.right += 0.5*length_of_rect;
					candidate_pois_list = map->find_pois_within_boundary(range);
				}
			}
			
			//����POI�̌���
			std::random_device device2;
			std::mt19937_64 generator2(device2());
			std::shuffle(candidate_pois_list.begin(), candidate_pois_list.end(), generator2);
			std::vector<std::shared_ptr<Map::BasicPoi const>>::const_iterator next_poi = candidate_pois_list.begin();

		
			//���ݒn�̒�~���Ԃ������_���Őݒ肵�C���n�_�̏o���n�̑��x�ŁC����POI�܂ł̍ŒZ�H�ňړ��������̎��Ԃ����߂�D
			user->set_random_pause_time(phase_id, MIN_PAUSE_TIME, MAX_PAUSE_TIME);
			int moving_time_between_poi_and_next_poi = map->calc_necessary_time((*now_poi)->get_id(), (*next_poi)->get_id(), user->get_speed(phase_id));
			int next_arrive_time = moving_time_between_poi_and_next_poi + user->get_pause_time(phase_id);

			//��~���Ԃ�phase�Ɋ��Z���Cpause_time�ƍŒZ�H�o�H����path�����肵�Ă���
			div_t variable_of_converted_pause_time_to_phase = std::div(user->get_pause_time(phase_id) - dest_rest_time, SERVICE_INTERVAL);
			std::vector<Graph::MapNodeIndicator> shortests_path_between_pois = map->get_shortest_path((*now_poi)->get_id(), (*next_poi)->get_id());

			//��~���ԕ��C�ephase�ɒ�~�ꏊ�ƈړ����x(0)��o�^
			for (int i = 0; i < variable_of_converted_pause_time_to_phase.quot; i++)
			{
				phase_id++;
				user->set_position_of_phase(phase_id, (*now_poi)->get_id(), (*now_poi)->data->get_position());
				user->set_speed(phase_id, 0);
			}
			
			std::vector<Graph::MapNodeIndicator>::iterator path_iter = shortests_path_between_pois.begin();//path���������邽�߂�index
			//���x��phase�Ŗ��߂�O���Q�Ƃ��Ȃ���΂Ȃ�Ȃ����Ƃɒ���
			double pause_position_speed = user->get_speed(phase_id - variable_of_converted_pause_time_to_phase.quot);
			
			//�ŏ�������~���Ԃ�phase�Ɋ��Z�������̗]���time�Ƃ��C����ȊO��service_interval��time�Ƃ��āC���ݒn���狁�߂����n�_��distance���v�Z
			double distance = variable_of_converted_pause_time_to_phase.rem * pause_position_speed;
			double distance_between_now_and_next_poi = map->shortest_distance((*now_poi)->get_id(), (*next_poi)->get_id());
		
			//path���쐬�D�ꏊ�͈�ԋ߂�intersection���m�Ő��`��Ԃ���DMapNodeIndicator��Type��INVALID�Ƃ���D
			while (distance <= distance_between_now_and_next_poi) {
				
				//�ŒZ�H�̒��ň�ԋ߂�intersection��T���C���`��Ԃ���D
				Graph::MapNodeIndicator nearest_position  =  user->read_node_pos_info_of_phase(phase_id).first;
								
				while (distance > map->shortest_distance(user->read_node_pos_info_of_phase(phase_id).first, *path_iter))
				{
					nearest_position = *path_iter;
					path_iter++;
				}
				
				double distance_between_start_and_nearest_position = map->shortest_distance(user->read_node_pos_info_of_phase(phase_id).first, nearest_position);
				
				double distance_between_nearest_intersection_and_arrive_position = distance - distance_between_start_and_nearest_position;
				Geography::LatLng nearest_latlng
					= nearest_position.type() == Graph::NodeType::POI ? map->get_static_poi(nearest_position.id())->data->get_position() : *map->get_static_node(nearest_position.id())->data;
				Geography::LatLng next_nearest_latlang
					= nearest_position.type() == Graph::NodeType::POI ? map->get_static_poi((*path_iter).id())->data->get_position() : *map->get_static_node((*path_iter).id())->data;
				double angle = Geography::GeoCalculation::lambert_azimuth_angle(nearest_latlng, next_nearest_latlang);

				Geography::LatLng arrive_position = Geography::GeoCalculation::calc_translated_point(nearest_latlng, distance_between_nearest_intersection_and_arrive_position, angle);
				
				phase_id++;
				user->set_position_of_phase(phase_id, Graph::MapNodeIndicator(Graph::NodeType::INVALID, Graph::NodeType::INVALID), arrive_position);
									
				distance += SERVICE_INTERVAL * pause_position_speed;
			}

			//destination�̂Ƃ���܂ŕ⊮�ł�����Crest_time��ێ����Ă����I
			double distance_between_arrive_position_and_dest_position = distance - map->shortest_distance((*now_poi)->get_id(), (*next_poi)->get_id());
			dest_rest_time = distance_between_arrive_position_and_dest_position / pause_position_speed;
			//�ړI�n�̓o�^
			user->set_position_of_phase(phase_id, (*next_poi)->get_id(), map->get_static_poi((*next_poi)->get_id())->data->get_position());
			
			//���݈ʒu�̍X�V
			now_poi = next_poi;

		}
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
		random_user();
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
			int min_pause_time = 60,
			double average_speed = 3.0,
			double speed_range = 2.0
		---------------------------------------------*/

		requirements = 
		{
			std::make_shared<Requirement::KatoMethodRequirement>(1000 * 1000, 16, 180, 10, 5),
			std::make_shared<Requirement::KatoMethodRequirement>(1000 * 1000, 25, 180, 10, 5),
		};
	}

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
			kato_bachelor_method.run();
		}
	}

	void HayashidaSimulator::evaluate()
	{

	}

	void HayashidaSimulator::export_evaluation_result(const std::string& export_base_path)
	{

	}

}