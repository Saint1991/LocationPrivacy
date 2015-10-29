#include "stdafx.h"
#include "HayashidaSimulator.h"

namespace Simulation
{

	HayashidaSimulator::HayashidaSimulator() 
		: ISimulator<Map::BasicDbMap, Entity::PauseMobileEntity<Geography::LatLng>, Requirement::KatoMethodRequirement>()
	{
	}

	
	HayashidaSimulator::~HayashidaSimulator()
	{
	}


	void HayashidaSimulator::build_map()
	{
	}

	void HayashidaSimulator::create_user()
	{
		//random�Ɍo�H��ݒ�ł���悤�ɂ������I�I
		//input_user_plan�����
		//get_user���������͂����肳����D
		
		
		int phase_id = 0;
		std::vector<std::shared_ptr<Map::BasicPoi const>> random_pois_list = map->find_pois_within_boundary(Graph::Rectangle<Geography::LatLng>(1.0, 1.0, 1.0, 1.0));//�����̎l�p�`�ɂ͎n�_�ɂ������͈͂��C���X�^���X�œ���
		std::random_device device;
		std::mt19937_64 generator(device());
		std::shuffle(random_pois_list.begin(), random_pois_list.end(), generator);
		
		std::vector<std::shared_ptr<Map::BasicPoi const>>::const_iterator poi = random_pois_list.begin();
		user->set_position_of_phase(phase_id, Graph::MapNodeIndicator((*poi)->get_id()), (*poi)->data->get_position());
		//user->set_random_pause_time();

		//user->set_random_speed(phase_id, requirements->average_speed_of_dummy, requirement->speed_range_of_dummy);
		


	}

	void HayashidaSimulator::make_requirement_list()
	{
		//�������񑲘_��@�̗v���p�����[�^
		//�R���X�g���N�^���쐬����
		/*
			double required_anonymous_area,
			size_t dummy_num,
			time_t service_interval,
			int interval_of_base_phase,
			int cycle_of_interval_of_base_phase,
			time_t max_pause_time = 600,
			time_t min_pause_time = 60,
			double average_speed = 3.0,
			double speed_range = 2.0
		*/
		requirements = 
		{
			std::make_shared<Requirement::KatoMethodRequirement>(1000 * 1000, 16, 180, 10, 5),
			std::make_shared<Requirement::KatoMethodRequirement>(1000 * 1000, 25, 180, 10, 5),
		};
	}

	void HayashidaSimulator::run()
	{

	}

}