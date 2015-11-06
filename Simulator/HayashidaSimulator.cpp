#include "stdafx.h"
#include "HayashidaSimulator.h"

namespace Simulation
{
	///<summary>
	/// コンストラクタ
	///</summary>
	HayashidaSimulator::HayashidaSimulator(const Graph::Rectangle<Geography::LatLng>& boundary) 
		: ISimulator<Map::BasicDbMap, Entity::PauseMobileEntity<Geography::LatLng>, Entity::PauseMobileEntity<Geography::LatLng>, Requirement::KatoMethodRequirement, Geography::LatLng, Graph::Trajectory<Geography::LatLng>>(), map_boundary(boundary)
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
		//time_managerのtimeってどこで設定する？
		
		//rect_init_langには始点にしたい範囲をインスタンスで入力
		Graph::Rectangle<Geography::LatLng> rect_init_range(base_lat + 0.5*length_of_rect, base_lng - 0.5*length_of_rect, base_lat - 0.5*length_of_rect, base_lng + 0.5*length_of_rect);
	 
		//-----time_managerの生成-------//
		int phase_id = 0;
		std::unique_ptr<std::vector<time_t>> timeslots = std::make_unique<std::vector<time_t>>();
		for (int time = 0; time <= end_time; time += SERVICE_INTERVAL) {
			timeslots->push_back(time);
		}

		time_manager = std::make_shared<Time::TimeSlotManager>(std::move(timeslots));
		user = std::make_shared<Entity::PauseMobileEntity<Geography::LatLng>>(0, time_manager);

		//---------------------------最初の点を決定---------------------------------------------
		std::vector<std::shared_ptr<Map::BasicPoi const>> random_pois_list = map->find_pois_within_boundary(rect_init_range);
		
		//もし範囲内のPOIが見つからなかったら，範囲を広げて再計算
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

		std::vector<std::shared_ptr<Map::BasicPoi const>>::iterator now_poi = random_pois_list.begin();
		user->set_position_of_phase(phase_id, Graph::MapNodeIndicator((*now_poi)->get_id()), (*now_poi)->data->get_position());
		user->set_random_speed(phase_id, AVERAGE_SPEED, RANGE_OF_SPEED);

		int dest_rest_time = 0;//phaseの到着時間と実際の到着時間の差分.最初だけ0

		//--------------二個目以降の点を決定．forで，確保したいpoiの個数分をループさせる-------------------
		for (int i = 1; i < POI_NUM; i++)
		{
			//次の候補点の範囲を求める
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
			
			//次のPOIの決定
			std::random_device device2;
			std::mt19937_64 generator2(device2());
			std::shuffle(candidate_pois_list.begin(), candidate_pois_list.end(), generator2);
			std::vector<std::shared_ptr<Map::BasicPoi const>>::iterator next_poi = candidate_pois_list.begin();

		
			//現在地の停止時間をランダムで設定し，現地点の出発地の速度で，次のPOIまでの最短路で移動した時の時間を求める．
			user->set_random_pause_time(phase_id, MIN_PAUSE_TIME, MAX_PAUSE_TIME);
			double moving_time_between_poi_and_next_poi = map->calc_necessary_time((*now_poi)->get_id(), (*next_poi)->get_id(), user->get_speed(phase_id));
			int next_arrive_time = moving_time_between_poi_and_next_poi + user->get_pause_time(phase_id);

			//停止時間をphaseに換算し，pause_timeと最短路経路からpathを決定していく
			div_t variable_of_converted_pause_time_to_phase = std::div(user->get_pause_time(phase_id) - dest_rest_time, SERVICE_INTERVAL);
			std::vector<Graph::MapNodeIndicator> shortests_path_between_pois = map->get_shortest_path((*now_poi)->get_id(), (*next_poi)->get_id());

			//停止時間分，各phaseに停止場所と移動速度(0)を登録
			for (int i = 0; i < variable_of_converted_pause_time_to_phase.quot; i++)
			{
				phase_id++;
				user->set_position_of_phase(phase_id, (*now_poi)->get_id(), (*now_poi)->data->get_position());
				user->set_speed(phase_id, 0);
			}
			
			std::vector<Graph::MapNodeIndicator>::iterator path_iter = shortests_path_between_pois.begin();//pathを検索するためのindex
			//速度はphaseで埋める前を参照しなければならないことに注意
			double pause_position_speed = user->get_speed(phase_id - variable_of_converted_pause_time_to_phase.quot);
			
			//最初だけ停止時間をphaseに換算した時の余りをtimeとし，それ以外はservice_intervalをtimeとして，現在地から求めたい地点のdistanceを計算
			double distance = variable_of_converted_pause_time_to_phase.rem * pause_position_speed;
			double distance_between_now_and_next_poi = map->shortest_distance((*now_poi)->get_id(), (*next_poi)->get_id());

			Graph::MapNodeIndicator nearest_position = (*now_poi)->get_id();

			//pathを作成．場所は一番近いintersection同士で線形補間する．MapNodeIndicatorのTypeはINVALIDとする．
			while (distance <= distance_between_now_and_next_poi) {
				
				//最短路の中で一番近いintersectionを探し，線形補間する．		
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
				
				phase_id++;
				user->set_position_of_phase(phase_id, Graph::MapNodeIndicator(Graph::NodeType::INVALID, Graph::NodeType::INVALID), arrive_position);
									
				distance += SERVICE_INTERVAL * pause_position_speed;

			}

			//destinationのところまで補完できたら，rest_timeを保持しておく！
			double distance_between_arrive_position_and_dest_position = distance - distance_between_now_and_next_poi;
			dest_rest_time = distance_between_arrive_position_and_dest_position / pause_position_speed;
			//目的地の登録
			user->set_position_of_phase(phase_id, (*next_poi)->get_id(), map->get_static_poi((*next_poi)->get_id())->data->get_position());
			user->set_random_speed(phase_id, AVERAGE_SPEED, RANGE_OF_SPEED);

			//現在位置の更新
			*now_poi = *next_poi;

		}
	}

	///<summary>
	/// map_dataを生成する
	///</summary>
	void HayashidaSimulator::build_map(const Graph::Rectangle<Geography::LatLng>& boundary)
	{
		map = std::make_shared<Map::BasicDbMap>(std::make_shared<Graph::Dijkstra<Map::BasicMapNode, Map::BasicRoad>>(), "../settings/mydbsettings.xml", "map_tokyo");
		map->load(boundary);
	}


	///<summary>
	/// ユーザを生成する
	///</summary>
	void HayashidaSimulator::create_trajectories()
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
		/*-------------↓要求パラメータ---------------
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
	/// 実行
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

	void HayashidaSimulator::export_evaluation_result()
	{
		IO::FileExporter exporter({
			{Geography::LatLng::LATITUDE, "緯度"},
			{Geography::LatLng::LONGITUDE, "経度"}
		}, USER_TRAJECTORY_OUT_PATH);

		std::list<std::shared_ptr<IO::FileExportable const>> exportable_positions;
		time_manager->for_each_time([&](time_t time, long interval, int phase) {
			exportable_positions.push_back(user->read_position_of_phase(phase));
		});
		
		exporter.export_lines(exportable_positions);
	}

}