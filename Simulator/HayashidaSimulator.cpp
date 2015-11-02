#include "stdafx.h"
#include "HayashidaSimulator.h"

namespace Simulation
{
	///<summary>
	/// �R���X�g���N�^
	///</summary>
	HayashidaSimulator::HayashidaSimulator() 
		: ISimulator<Map::BasicDbMap, Entity::PauseMobileEntity<Geography::LatLng>, Entity::PauseMobileEntity<Geography::LatLng>, Requirement::KatoMethodRequirement>()
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
		const int POI_NUM = 4;
		Graph::Rectangle<Geography::LatLng> rect_init_range(1.0, 1.0, 1.0, 1.0);//�����̎l�p�`�ɂ͎n�_�ɂ������͈͂��C���X�^���X�œ���
		double length_of_rect = 0.0005;//�����ɂ͓K�؂Ȕ͈͓��̈ܓx�o�x��������
	    //-----���v���p�����[�^�Drequirement�Ɠ����l�ɂ��邱�ƁI��---------//

		int phase_id = 0;

		//---------------------------�ŏ��̓_������---------------------------------------------
		std::vector<std::shared_ptr<Map::BasicPoi const>> random_pois_list = map->find_pois_within_boundary(rect_init_range);
		std::random_device device;
		std::mt19937_64 generator(device());
		std::shuffle(random_pois_list.begin(), random_pois_list.end(), generator);

		std::vector<std::shared_ptr<Map::BasicPoi const>>::const_iterator now_poi = random_pois_list.begin();
		user->set_position_of_phase(phase_id, Graph::MapNodeIndicator((*now_poi)->get_id()), (*now_poi)->data->get_position());
		user->set_random_speed(phase_id, AVERAGE_SPEED, RANGE_OF_SPEED);
		phase_id++;

		int dest_rest_time = 0;//phase�̓������ԂƎ��ۂ̓������Ԃ̍���.�ŏ�����0

		//--------------��ڈȍ~�̓_������Dfor�ŁC�m�ۂ�����poi�̌��������[�v������-------------------
		for (int i = 1; i < POI_NUM; i++)
		{
			//���̌��_�͈̔͂����߂�
			Math::Probability generator;
			double distance_between_positions = generator.uniform_distribution(300.0, 1000.0);
			double angle_of_positions = 45;// generator.uniform_distribution(0, 90) or (270,360);
			Geography::LatLng next_candidate_poi_position_range
				= Geography::GeoCalculation::calc_translated_point((*now_poi)->data->get_position(), distance_between_positions, angle_of_positions);

			Graph::Rectangle<Geography::LatLng> range(next_candidate_poi_position_range.lat() + length_of_rect, next_candidate_poi_position_range.lng(), next_candidate_poi_position_range.lat(), next_candidate_poi_position_range.lng() + length_of_rect);

			//����POI�̌���
			std::vector<std::shared_ptr<Map::BasicPoi const>> candidate_pois_list = map->find_pois_within_boundary(range);
			std::random_device device2;
			std::mt19937_64 generator2(device2());
			std::shuffle(candidate_pois_list.begin(), candidate_pois_list.end(), generator2);
			std::vector<std::shared_ptr<Map::BasicPoi const>>::const_iterator next_poi = random_pois_list.begin();

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
				user->set_position_of_phase(phase_id, (*now_poi)->get_id(), (*now_poi)->data->get_position());
				user->set_speed(phase_id, 0);
				phase_id++;
			}


			std::vector<Graph::MapNodeIndicator>::iterator path_iter = shortests_path_between_pois.begin();//path���������邽�߂�index
			//���x��phase�Ŗ��߂�O���Q�Ƃ��Ȃ���΂Ȃ�Ȃ����Ƃɒ���
			double pause_position_speed = user->get_speed((phase_id - 1) - variable_of_converted_pause_time_to_phase.quot);
			
			//�ŏ�������~���Ԃ�phase�Ɋ��Z�������̗]���time�Ƃ��C����ȊO��service_interval��time�Ƃ��āC���ݒn���狁�߂����n�_��distance���v�Z
			double distance = variable_of_converted_pause_time_to_phase.rem * pause_position_speed;
			
			//path���쐬�D�ꏊ�͈�ԋ߂�intersection���m�Ő��`��Ԃ���DMapNodeIndicator��Type��INVALID�Ƃ���D
			while (distance >= map->shortest_distance((*now_poi)->get_id(),(*next_poi)->get_id())) {
				
				//�ŒZ�H�̒��ň�ԋ߂�intersection��T���C���`��Ԃ���D
				Graph::MapNodeIndicator nearest_position  =  user->read_node_pos_info_of_phase(phase_id-1).first;
								
				while (distance > map->shortest_distance(user->read_node_pos_info_of_phase(phase_id - 1).first, *path_iter))
				{
					nearest_position = *path_iter;
					path_iter++;
				}
				double distance_between_nearest_intersection_and_arrive_position = distance - map->shortest_distance(user->read_node_pos_info_of_phase(phase_id - 1).first, nearest_position);
				Geography::LatLng nearest_latlng
					= nearest_position.type() == Graph::NodeType::POI ? map->get_static_poi(nearest_position.id())->data->get_position() : *map->get_static_node(nearest_position.id())->data;
				Geography::LatLng next_nearest_latlang
					= nearest_position.type() == Graph::NodeType::POI ? map->get_static_poi((*path_iter).id())->data->get_position() : *map->get_static_node((*path_iter).id())->data;
				double angle = Geography::GeoCalculation::lambert_azimuth_angle(nearest_latlng, next_nearest_latlang);

				Geography::LatLng arrive_position = Geography::GeoCalculation::calc_translated_point(nearest_latlng, distance_between_nearest_intersection_and_arrive_position, angle);
				
				user->set_position_of_phase(phase_id, Graph::MapNodeIndicator(Graph::NodeType::INVALID, Graph::NodeType::INVALID), arrive_position);
				phase_id++;
					
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
	void HayashidaSimulator::build_map()
	{
	
	}


	///<summary>
	/// ���[�U�𐶐�����
	///</summary>
	void HayashidaSimulator::create_user()
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


	///<summary>
	/// ���s
	///</summary>
	void HayashidaSimulator::run()
	{

	}

}