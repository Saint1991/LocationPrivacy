#include "stdafx.h"
#include "HayashidaSimulator.h"

namespace Simulation
{
	///<summary>
	/// コンストラクタ
	///</summary>
	HayashidaSimulator::HayashidaSimulator() 
		: ISimulator<Map::BasicDbMap, Entity::PauseMobileEntity<Geography::LatLng>, Entity::PauseMobileEntity<Geography::LatLng>, Requirement::KatoMethodRequirement>()
	{
	}

	///<summary>
	/// デストラクタ
	///</summary>
	HayashidaSimulator::~HayashidaSimulator()
	{
	}

	///<summary>
	/// ランダムでユーザを生成する
	///</summary>
	void HayashidaSimulator::random_user()
	{
		//randomに経路を設定できるようにしたい！！
		//input_user_planを作る
		//get_userが何かをはっきりさせる．


		//-----↓要求パラメータ．requirementと同じ値にすること！↓---------//
		const double AVERAGE_SPEED = 1.5;
		const double RANGE_OF_SPEED = 0.5;
		const time_t MAX_PAUSE_TIME = 0.5;
		const time_t MIN_PAUSE_TIME = 0.5;
		const time_t SERVICE_INTERVAL = 180;
		const int POI_NUM = 4;
		//-----↑要求パラメータ．requirementと同じ値にすること！↑---------//

		int phase_id = 0;

		//---------------------------最初の点を決定---------------------------------------------
		std::vector<std::shared_ptr<Map::BasicPoi const>> random_pois_list = map->find_pois_within_boundary(Graph::Rectangle<Geography::LatLng>(1.0, 1.0, 1.0, 1.0));//ここの四角形には始点にしたい範囲をインスタンスで入力
		std::random_device device;
		std::mt19937_64 generator(device());
		std::shuffle(random_pois_list.begin(), random_pois_list.end(), generator);

		std::vector<std::shared_ptr<Map::BasicPoi const>>::const_iterator now_poi = random_pois_list.begin();
		user->set_position_of_phase(phase_id, Graph::MapNodeIndicator((*now_poi)->get_id()), (*now_poi)->data->get_position());
		user->set_random_speed(phase_id, AVERAGE_SPEED, RANGE_OF_SPEED);

		//--------------二個目以降の点を決定．forで，確保したいpoiの個数分をループさせる-------------------
		for (int i = 1; i < POI_NUM; i++)
		{
			//次の候補点の範囲を求める
			Math::Probability generator;
			double distance_between_positions = 500.0;// generator.uniform_distribution(300.0, 1000.0);
			double angle_of_positions = 45;// generator.uniform_distribution(0, 90) or (270,360);
			Geography::LatLng next_candidate_poi_position_range = Geography::GeoCalculation::calc_translated_point((*now_poi)->data->get_position(), distance_between_positions, angle_of_positions);

			//次のPOIの決定
			std::vector<std::shared_ptr<Map::BasicPoi const>> candidate_pois_list = map->find_pois_within_boundary(Graph::Rectangle<Geography::LatLng>());//ここは適切な範囲内の四角形を生成
			std::random_device device2;
			std::mt19937_64 generator2(device2());
			std::shuffle(candidate_pois_list.begin(), candidate_pois_list.end(), generator2);
			std::vector<std::shared_ptr<Map::BasicPoi const>>::const_iterator next_poi = random_pois_list.begin();

			//現在地の停止時間をランダムで設定し，現地点の出発地の速度で，次のPOIまでの最短路で移動した時の時間を求める．
			user->set_random_pause_time(phase_id, MIN_PAUSE_TIME, MAX_PAUSE_TIME);
			time_t moving_time_between_poi_and_next_poi = map->calc_necessary_time((*now_poi)->get_id(), (*next_poi)->get_id(), user->get_speed(phase_id));
			time_t next_arrive_time = moving_time_between_poi_and_next_poi + user->get_pause_time(phase_id);

			//停止時間をphaseに換算し，pause_timeと最短路経路からpathを決定していく
			//intかlongかで値変わってきそうだから，ここは要再検討
			lldiv_t variable_of_converted_pause_time_to_phase = std::lldiv(user->get_pause_time(phase_id), SERVICE_INTERVAL);
			std::vector<Graph::MapNodeIndicator> shortests_path_between_pois = map->get_shortest_path((*now_poi)->get_id(), (*next_poi)->get_id());

			//停止時間分，phaseを登録
			for (int i = 0; i < variable_of_converted_pause_time_to_phase.quot; i++)
			{
				user->set_position_of_phase(phase_id, (*now_poi)->get_id(), (*now_poi)->data->get_position());
				phase_id++;
			}


			int init_flag = 1;//POIを決める最初のフェーズかどうかを示すフラグ
			std::vector<Graph::MapNodeIndicator>::iterator path_iter = shortests_path_between_pois.begin();//pathを検索するためのindex

			//pathを作成．場所は一番近いintersection同士で線形補間する．MapNodeIndicatorのTypeはINVALIDとする．
			for (std::vector<Graph::MapNodeIndicator>::iterator poi_iter = shortests_path_between_pois.begin(); poi_iter != shortests_path_between_pois.end(); poi_iter++) {
				//最初だけ停止時間をphaseに換算した時の余りをtimeとし，それ以外はservice_intervalをtimeとして，現在地から求めたい地点のdistanceを計算
				double distance 
					= init_flag == 1 ? variable_of_converted_pause_time_to_phase.rem * user->get_speed(phase_id) : SERVICE_INTERVAL * user->get_speed(phase_id);

				//最初は停止時間をphaseに換算したときの余り分をdistanceとして，最短路の中で一番近いintersectionを探し，線形補間する．
				Graph::MapNodeIndicator nearest_position  =  user->read_node_pos_info_of_phase(phase_id-1).first;
								
				while (distance < map->shortest_distance(user->read_node_pos_info_of_phase(phase_id - 1).first, *path_iter))
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
					
				init_flag = 0;//フラグを下ろす
			}
			

		}

		//time_managerのtimeってどこで設定する？

	}

	///<summary>
	/// map_dataを生成する
	///</summary>
	void HayashidaSimulator::build_map()
	{
	
	}


	///<summary>
	/// ユーザを生成する
	///</summary>
	void HayashidaSimulator::create_user()
	{
		random_user();
	}



	///<summary>
	/// 要求パラメータを作成．
	/// コンストラクタで具体的な値を入れること
	///</summary>
	void HayashidaSimulator::make_requirement_list()
	{
		//加藤さん卒論手法の要求パラメータ
		//コンストラクタを作成する
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


	///<summary>
	/// 実行
	///</summary>
	void HayashidaSimulator::run()
	{

	}

}