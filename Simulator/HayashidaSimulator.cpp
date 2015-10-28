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
		//randomに経路を設定できるようにしたい！！
		//input_user_planを作る
		//get_userが何かをはっきりさせる．
		
		int phase_id = 0;
		std::vector<std::shared_ptr<Map::BasicPoi const>> random_pois_list = map->find_pois_within_boundary(Graph::Rectangle<Geography::LatLng>(1.0, 1.0, 1.0, 1.0));//ここの四角形には始点にしたい範囲をインスタンスで入力
		std::random_device device;
		std::mt19937_64 generator(device());
		std::shuffle(random_pois_list.begin(), random_pois_list.end(), generator);
		/*
		std::vector<std::shared_ptr<Map::BasicPoi const>>::const_iterator poi = random_pois_list.begin();
		user->set_position_of_phase(phase_id, Graph::MapNodeIndicator((*poi)->get_id()), (*poi)->data->get_position());
		user->set_random_pause_time();

		user->set_random_speed(phase_id, requirements->average_speed_of_dummy, requirement->speed_range_of_dummy);
		*/


	}
	void HayashidaSimulator::make_requirement_list()
	{

	}

	void HayashidaSimulator::run()
	{

	}
}