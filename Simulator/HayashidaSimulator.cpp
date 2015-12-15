#include "stdafx.h"
#include "HayashidaSimulator.h"

namespace Simulation
{
	///<summary>
	/// コンストラクタ
	///</summary>
	HayashidaSimulator::HayashidaSimulator(const Graph::Rectangle<Geography::LatLng>& boundary) 
		: ISimulator<Map::BasicDbMap, Entity::PauseMobileEntity<Geography::LatLng>, Entity::RevisablePauseMobileEntity<Geography::LatLng>, Requirement::KatoMethodRequirement, Geography::LatLng, Graph::RevisableTrajectory<Geography::LatLng>>(), map_boundary(boundary)
	{
	}

	///<summary>
	/// デストラクタ
	///</summary>
	HayashidaSimulator::~HayashidaSimulator()
	{
	}


	///<summary>
	/// rectの範囲内にあるpoi_listを適当にシャッフルした状態で取得
	/// 一回探索して，見つからなかった場合は，rectの範囲を広げなおして再検索
	///</summary>
	std::vector<std::shared_ptr<Map::BasicPoi const>> HayashidaSimulator::get_pois_list(Graph::Rectangle<Geography::LatLng>& boundary)
	{
		std::vector<std::shared_ptr<Map::BasicPoi const>> pois_list = map->find_pois_within_boundary(boundary);
		double length = 0.005;
		//もし範囲内のPOIが見つからなかったら，範囲を広げて再計算
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
	/// 最短路の中で一番近いintersectionを探し，線形補間する． 
	/// now_poiには速度と停止時間を設定してあること
	/// 最後は目的地の直前の場所を補完した場所が登録されている．
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
		//distanceとmap->shortest_distance((*now_poi)->get_id(), *path_iter)が等しい時は，丁度交差点orPOIに到着する場合
		else {
			(*phase_id)++;
			(*path_iter).type() == Graph::NodeType::POI ?
				user->set_position_of_phase(*phase_id, (*path_iter).id(), map->get_static_poi((*path_iter).id())->data->get_position()) :
				user->set_position_of_phase(*phase_id, (*path_iter).id(), *map->get_static_node((*path_iter).id())->data);
		}*/

		*distance += service_interval * pause_position_speed;
	}


	///<summary>
	/// time_managerの生成
	///</summary>
	std::shared_ptr<Time::TimeSlotManager> HayashidaSimulator::create_time_manager() {
		std::unique_ptr<std::vector<time_t>> timeslots = std::make_unique<std::vector<time_t>>();
		for (int time = 0; time <= end_time; time += SERVICE_INTERVAL) {
			timeslots->push_back(time);
		}

		return std::make_shared<Time::TimeSlotManager>(std::move(timeslots));
	}

	///<summary>
	/// 停止時間と速度0のセット
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
	/// inputするpoiのlistを作成
	///</summary>
	void HayashidaSimulator::input_visit_pois() {
		//POI系列を作成する際の，選択範囲
		Graph::Rectangle<Geography::LatLng> poi_range(BASE_LAT + 0.5*length_of_all_POI_rect, BASE_LNG - 0.5*length_of_all_POI_rect, BASE_LAT - 0.5*length_of_all_POI_rect, BASE_LNG + 0.5*length_of_all_POI_rect);

		std::vector<std::shared_ptr<Map::BasicPoi const>> random_pois_list = get_pois_list(poi_range);
		std::vector<std::shared_ptr<Map::BasicPoi const>>::iterator iter = random_pois_list.begin();
		for (int i = 0; i < POI_NUM; i++, iter++) {
			input_poi_list.push_back(*iter);
		}
	}

	///<summary>
	/// input_elementsの入力POIを基に，巡回セールスマン問題を時，総距離が小さ順にソートしたvectorを返す．
	///</summary>
	std::vector<std::vector<std::shared_ptr<Map::BasicPoi const>>> HayashidaSimulator::all_traveling_salesman_problem(std::vector<std::shared_ptr<Map::BasicPoi const>>& visited_pois)
	{
		std::vector<std::vector<std::shared_ptr<Map::BasicPoi const>>> all_tsp_solution;
		return all_tsp_solution;
	}


	///<summary>
	/// input_elementsの入力POIを基に，巡回セールスマン問題を解き，最適解を返す．
	///</summary>
	std::vector<std::shared_ptr<Map::BasicPoi const>> HayashidaSimulator::traveling_salesman_problem(std::vector<std::shared_ptr<Map::BasicPoi const>>& visited_pois)
	{
		return all_traveling_salesman_problem(visited_pois).front();
	}

	///<summary>
	/// input_elementsの入力POIを基に，巡回セールスマン問題を解き，ガウス分布に従って一定確率で求めたpoi系列を返す．
	///</summary>
	std::vector<std::shared_ptr<Map::BasicPoi const>> HayashidaSimulator::Gaussian_distribution(std::vector<std::shared_ptr<Map::BasicPoi const>>& visited_pois)
	{
		std::vector<std::vector<std::shared_ptr<Map::BasicPoi const>>> all_tsp_solution = all_traveling_salesman_problem(visited_pois);
		std::vector<std::shared_ptr<Map::BasicPoi const>> gaussian_pois;
		
		return gaussian_pois;
	}



	///<summary>
	/// ランダムムーブを行うユーザを生成する
	/// ただし，現在は最短経路で移動
	///</summary>
	void HayashidaSimulator::make_random_movement_user()
	{
		Math::Probability generator;

		//rect_init_langには始点にしたい範囲をインスタンスで入力
		Graph::Rectangle<Geography::LatLng> rect_init_range(BASE_LAT + 0.5*length_of_rect, BASE_LNG - 0.5*length_of_rect, BASE_LAT - 0.5*length_of_rect, BASE_LNG + 0.5*length_of_rect);
	 
		//----------------------------time_managerとuserの生成-------------------------------------//
		time_manager = create_time_manager();
		user = std::make_shared<Entity::PauseMobileEntity<Geography::LatLng>>(0, time_manager);
		
		//---------------------------最初の点を決定---------------------------------------------
		int phase_id = 0;
		std::vector<std::shared_ptr<Map::BasicPoi const>> random_pois_list = get_pois_list(rect_init_range);
		std::vector<std::shared_ptr<Map::BasicPoi const>>::iterator now_poi = random_pois_list.begin();
		user->set_visited_poi_of_phase(phase_id, Graph::MapNodeIndicator((*now_poi)->get_id()), (*now_poi)->data->get_position());
		user->set_random_starting_speed_at_poi(AVERAGE_SPEED, RANGE_OF_SPEED);
		
		double dest_rest_time = 0;//phaseの到着時間と実際の到着時間の差分.最初だけ0

		//--------------二個目以降の点を決定．forで，確保したいpoiの個数分をループさせる-------------------
		for (int i = 1; i < POI_NUM; i++)
		{
			//次の候補点の範囲を求める
			double distance_between_positions = generator.uniform_distribution(150.0, 350.0);
			double angle_of_positions = generator.uniform_distribution(-(M_PI), M_PI_4);
			Geography::LatLng next_candidate_poi_position_range
				= Geography::GeoCalculation::calc_translated_point((*now_poi)->data->get_position(), distance_between_positions, angle_of_positions);

			Graph::Rectangle<Geography::LatLng> range(next_candidate_poi_position_range.lat() + 0.001, next_candidate_poi_position_range.lng(), next_candidate_poi_position_range.lat(), next_candidate_poi_position_range.lng() + 0.001);
			
			std::vector<std::shared_ptr<Map::BasicPoi const>> candidate_pois_list = get_pois_list(range);
			//次のPOIの決定
			std::vector<std::shared_ptr<Map::BasicPoi const>>::iterator next_poi = candidate_pois_list.begin();

			//現在地の停止時間をランダムで設定し，現地点の出発地の速度で，次のPOIまでの最短路で移動した時の時間を求める．
			user->set_random_pause_time(phase_id, MIN_PAUSE_TIME, MAX_PAUSE_TIME);
			user->set_dest_rest_time(dest_rest_time);

			double moving_time_between_poi_and_next_poi = map->calc_necessary_time((*now_poi)->get_id(), (*next_poi)->get_id(), user->get_now_speed(phase_id));
			int next_arrive_time = moving_time_between_poi_and_next_poi + user->get_pause_time();

			int rest_pause_time = user->get_pause_time() - dest_rest_time;

			//停止時間をphaseに換算し，pause_timeと最短路経路からpathを決定していく
			div_t variable_of_converted_pause_time_to_phase = std::div(rest_pause_time, SERVICE_INTERVAL);
			
			//出発するときの余り時間を登録
			user->set_rest_pause_time_when_departing(variable_of_converted_pause_time_to_phase.rem);

			//停止時間分，各phaseに停止場所と移動速度(0)を登録
			set_pause_time_and_phases_of_visited_POI(&phase_id, variable_of_converted_pause_time_to_phase, now_poi);
			

			std::vector<Graph::MapNodeIndicator> shortests_path_between_pois = map->get_shortest_path((*now_poi)->get_id(), (*next_poi)->get_id());
			std::vector<Graph::MapNodeIndicator>::iterator path_iter = shortests_path_between_pois.begin();//pathを検索するためのindex
			//速度はphaseで埋める前を参照しなければならないことに注意
			double pause_position_speed = user->get_starting_speed();
			
			//最初だけ停止時間をphaseに換算した時の余りをtimeとし，それ以外はservice_intervalをtimeとして，現在地から求めたい地点のdistanceを計算
			double distance = (SERVICE_INTERVAL - variable_of_converted_pause_time_to_phase.rem) * pause_position_speed;
			double distance_between_now_and_next_poi = map->shortest_distance((*now_poi)->get_id(), (*next_poi)->get_id());

			Graph::MapNodeIndicator nearest_position = (*now_poi)->get_id();

			//pathを作成．場所は一番近いintersection同士で線形補間する．MapNodeIndicatorのTypeはINVALIDとする．
			while (distance < distance_between_now_and_next_poi) {
				//最短路の中で一番近いintersectionを探し，線形補間する．		
				set_path_between_poi(now_poi, path_iter, nearest_position, pause_position_speed, SERVICE_INTERVAL, &distance, &phase_id);
			}
			
			//destinationのところまで補完できたら，rest_timeを保持しておく！
			double distance_between_arrive_position_and_dest_position = distance - distance_between_now_and_next_poi;
			dest_rest_time = distance_between_arrive_position_and_dest_position / pause_position_speed;
			//目的地の登録
			phase_id++;
			user->increment_visited_pois_info_list_id();
			user->set_visited_poi_of_phase(phase_id, (*next_poi)->get_id(), map->get_static_poi((*next_poi)->get_id())->data->get_position());
			user->set_random_starting_speed_at_poi(AVERAGE_SPEED, RANGE_OF_SPEED);

			//現在位置の更新
			*now_poi = *next_poi;

		}

		//---------------------------------end_timeまで適当に経路を決める！---------------------------------------------------

		//最終地点は少し遠くにとる(1.5倍～2倍)．ただし，マップの限界範囲に注意
		double last_distance = 1.3 * (end_time - time_manager->time_of_phase(phase_id)) * user->get_now_speed(phase_id);

		//次の候補点の範囲を求める
		double last_angle = generator.uniform_distribution(-(M_PI), M_PI_2);
		Geography::LatLng last_candidate_poi_position_range
			= Geography::GeoCalculation::calc_translated_point((*now_poi)->data->get_position(), last_distance, last_angle);

		Graph::Rectangle<Geography::LatLng> last_range(last_candidate_poi_position_range.lat() + 0.001, last_candidate_poi_position_range.lng(), last_candidate_poi_position_range.lat(), last_candidate_poi_position_range.lng() + 0.001);

		std::vector<std::shared_ptr<Map::BasicPoi const>> last_candidate_pois_list = get_pois_list(last_range);
		//次のPOIの決定
		std::vector<std::shared_ptr<Map::BasicPoi const>>::iterator last_poi = last_candidate_pois_list.begin();
		
		//現在地の停止時間をランダムで設定し，現地点の出発地の速度で，次のPOIまでの最短路で移動した時の時間を求める．
		user->set_random_pause_time(phase_id, MIN_PAUSE_TIME, MAX_PAUSE_TIME);
		double moving_time_between_poi_and_next_poi = map->calc_necessary_time((*now_poi)->get_id(), (*last_poi)->get_id(), user->get_now_speed(phase_id));
		int next_arrive_time = moving_time_between_poi_and_next_poi + user->get_pause_time();

		
		//停止時間をphaseに換算し，pause_timeと最短路経路からpathを決定していく
		int rest_pause_time = user->get_pause_time() - dest_rest_time;
		div_t last_variable_of_converted_pause_time_to_phase = std::div(rest_pause_time, SERVICE_INTERVAL);

		std::vector<Graph::MapNodeIndicator> last_shortests_path = map->get_shortest_path((*now_poi)->get_id(), (*last_poi)->get_id());

		//停止時間分，各phaseに停止場所と移動速度(0)を登録
		set_pause_time_and_phases_of_visited_POI(&phase_id, last_variable_of_converted_pause_time_to_phase, now_poi);

		std::vector<Graph::MapNodeIndicator>::iterator last_path_iter = last_shortests_path.begin();//pathを検索するためのindex
		//速度はphaseで埋める前を参照しなければならないことに注意
		double last_pause_position_speed = user->get_now_speed(phase_id - last_variable_of_converted_pause_time_to_phase.quot);

		//最初だけ停止時間をphaseに換算した時の余りをtimeとし，それ以外はservice_intervalをtimeとして，現在地から求めたい地点のdistanceを計算
		double distance = (SERVICE_INTERVAL - last_variable_of_converted_pause_time_to_phase.rem) * last_pause_position_speed;
		double distance_between_now_and_next_poi = map->shortest_distance((*now_poi)->get_id(), (*last_poi)->get_id());

		Graph::MapNodeIndicator last_nearest_position = (*now_poi)->get_id();

		//pathを作成．場所は一番近いintersection同士で線形補間する．MapNodeIndicatorのTypeはINVALIDとする．
		//last_phaseまで埋める！
		while (phase_id != (time_manager->phase_count() - 1)) {
			set_path_between_poi(now_poi, last_path_iter, last_nearest_position, last_pause_position_speed, SERVICE_INTERVAL, &distance, &phase_id);
		}

		std::cout << "Success Creating Random User" << std::endl;
	}

	

	///<summary>
	/// input_poisを基に，t.s.p.を用い，予想ユーザを作成する．
	///</summary>
	void HayashidaSimulator::make_predicted_user() {
		
		time_manager = create_time_manager();//time_managerの生成
		user = std::make_shared<Entity::PauseMobileEntity<Geography::LatLng>>(0, time_manager);
				
		input_visit_pois();//userのinput情報を入れる

		//---------------------------POI訪問順序を決定---------------------------------------------
		int phase_id = 0;
		
		//userのvisited_poisを用いて，始点がstart_poiの巡回セールスマン問題を解く
		std::vector<std::shared_ptr<Map::BasicPoi const>> order_visited_poi = traveling_salesman_problem(input_poi_list);
		
		int dest_rest_time = 0;//phaseの到着時間と実際の到着時間の差分.最初だけ0
		//最初の点を登録
		std::vector<std::shared_ptr<Map::BasicPoi const>>::iterator now_poi = order_visited_poi.begin();
		std::vector<std::shared_ptr<Map::BasicPoi const>>::iterator next_poi = order_visited_poi.begin() + 1;

		user->set_position_of_phase(phase_id, Graph::MapNodeIndicator((*now_poi)->get_id()), (*now_poi)->data->get_position());
		user->set_random_now_speed(phase_id, AVERAGE_SPEED, RANGE_OF_SPEED);
		user->set_random_pause_time(phase_id, MIN_PAUSE_TIME, MAX_PAUSE_TIME);
     	//--------------二個目以降の点を決定．forで，確保したいpoiの個数分をループさせる-------------------
		for (int i = 1; i < POI_NUM; i++)
		{
			double moving_time_between_poi_and_next_poi = map->calc_necessary_time((*now_poi)->get_id(), (*next_poi)->get_id(), user->get_now_speed(phase_id));
			int next_arrive_time = moving_time_between_poi_and_next_poi + user->get_pause_time();

			int rest_pause_time = user->get_pause_time() - dest_rest_time;

			//停止時間をphaseに換算し，pause_timeと最短路経路からpathを決定していく
			div_t variable_of_converted_pause_time_to_phase = std::div(rest_pause_time, SERVICE_INTERVAL);

			//停止時間分，各phaseに停止場所と移動速度(0)を登録
			set_pause_time_and_phases_of_visited_POI(&phase_id, variable_of_converted_pause_time_to_phase, now_poi);

			std::vector<Graph::MapNodeIndicator> shortests_path_between_pois = map->get_shortest_path((*now_poi)->get_id(), (*next_poi)->get_id());
			std::vector<Graph::MapNodeIndicator>::iterator path_iter = shortests_path_between_pois.begin();//pathを検索するためのindex
			//速度はphaseで埋める前を参照しなければならないことに注意
			double pause_position_speed = user->get_now_speed(phase_id - variable_of_converted_pause_time_to_phase.quot);

			//最初だけ停止時間をphaseに換算した時の余りをtimeとし，それ以外はservice_intervalをtimeとして，現在地から求めたい地点のdistanceを計算
			double distance = (SERVICE_INTERVAL - variable_of_converted_pause_time_to_phase.rem) * pause_position_speed;
			double distance_between_now_and_next_poi = map->shortest_distance((*now_poi)->get_id(), (*next_poi)->get_id());

			Graph::MapNodeIndicator nearest_position = (*now_poi)->get_id();

			//pathを作成．場所は一番近いintersection同士で線形補間する．MapNodeIndicatorのTypeはINVALIDとする．
			while (distance < distance_between_now_and_next_poi) {
				//最短路の中で一番近いintersectionを探し，線形補間する．		
				set_path_between_poi(now_poi, path_iter, nearest_position, pause_position_speed, SERVICE_INTERVAL, &distance, &phase_id);
			}

			//destinationのところまで補完できたら，rest_timeを保持しておく！
			double distance_between_arrive_position_and_dest_position = distance - distance_between_now_and_next_poi;
			dest_rest_time = distance_between_arrive_position_and_dest_position / pause_position_speed;
			
			//目的地の登録
			phase_id++;
			user->set_position_of_phase(phase_id, (*next_poi)->get_id(), map->get_static_poi((*next_poi)->get_id())->data->get_position());
			user->set_random_pause_time(phase_id, MIN_PAUSE_TIME, MAX_PAUSE_TIME);
			user->set_random_now_speed(phase_id, AVERAGE_SPEED, RANGE_OF_SPEED);

			now_poi++;
			next_poi++;
		}

		//---------------------------------end_timeまで適当に経路を決める！---------------------------------------------------

		Math::Probability generator;
		//最終地点は少し遠くにとる(1.5倍～2倍)．ただし，マップの限界範囲に注意
		double last_distance = 1.3 * (end_time - time_manager->time_of_phase(phase_id)) * user->get_now_speed(phase_id);

		//次の候補点の範囲を求める
		double last_angle = generator.uniform_distribution(-(M_PI), M_PI_2);
		Geography::LatLng last_candidate_poi_position_range
			= Geography::GeoCalculation::calc_translated_point((*now_poi)->data->get_position(), last_distance, last_angle);

		Graph::Rectangle<Geography::LatLng> last_range(last_candidate_poi_position_range.lat() + 0.001, last_candidate_poi_position_range.lng(), last_candidate_poi_position_range.lat(), last_candidate_poi_position_range.lng() + 0.001);

		std::vector<std::shared_ptr<Map::BasicPoi const>> last_candidate_pois_list = get_pois_list(last_range);
		//次のPOIの決定
		std::vector<std::shared_ptr<Map::BasicPoi const>>::iterator last_poi = last_candidate_pois_list.begin();

		//現在地の停止時間をランダムで設定し，現地点の出発地の速度で，次のPOIまでの最短路で移動した時の時間を求める．
		double moving_time_between_poi_and_next_poi = map->calc_necessary_time((*now_poi)->get_id(), (*last_poi)->get_id(), user->get_now_speed(phase_id));
		int next_arrive_time = moving_time_between_poi_and_next_poi + user->get_pause_time();
		
		//停止時間をphaseに換算し，pause_timeと最短路経路からpathを決定していく
		int rest_pause_time = user->get_pause_time() - dest_rest_time;
		div_t last_variable_of_converted_pause_time_to_phase = std::div(rest_pause_time, SERVICE_INTERVAL);

		std::vector<Graph::MapNodeIndicator> last_shortests_path = map->get_shortest_path((*now_poi)->get_id(), (*last_poi)->get_id());

		//停止時間分，各phaseに停止場所と移動速度(0)を登録
		set_pause_time_and_phases_of_visited_POI(&phase_id, last_variable_of_converted_pause_time_to_phase, now_poi);

		std::vector<Graph::MapNodeIndicator>::iterator last_path_iter = last_shortests_path.begin();//pathを検索するためのindex
		//速度はphaseで埋める前を参照しなければならないことに注意
		double last_pause_position_speed = user->get_now_speed(phase_id - last_variable_of_converted_pause_time_to_phase.quot);

		//最初だけ停止時間をphaseに換算した時の余りをtimeとし，それ以外はservice_intervalをtimeとして，現在地から求めたい地点のdistanceを計算
		double distance = (SERVICE_INTERVAL - last_variable_of_converted_pause_time_to_phase.rem) * last_pause_position_speed;
		double distance_between_now_and_next_poi = map->shortest_distance((*now_poi)->get_id(), (*last_poi)->get_id());

		Graph::MapNodeIndicator last_nearest_position = (*now_poi)->get_id();

		//pathを作成．場所は一番近いintersection同士で線形補間する．MapNodeIndicatorのTypeはINVALIDとする．
		//last_phaseまで埋める！
		while (phase_id != (time_manager->phase_count() - 1)) {
			set_path_between_poi(now_poi, last_path_iter, last_nearest_position, last_pause_position_speed, SERVICE_INTERVAL, &distance, &phase_id);
		}

		std::cout << "Success Creating Input User" << std::endl;

	}

	
	///<summary>
	/// input_poisを基に，実際のユーザを作成する．
	/// t.s.p.の解の距離が近い順に正規分布で重み付けして，確率的に変化させる．
	///</summary>
	void HayashidaSimulator::make_real_user() {

		time_manager = create_time_manager();//time_managerの生成
		user = std::make_shared<Entity::PauseMobileEntity<Geography::LatLng>>(0, time_manager);

		input_visit_pois();//userのinput情報を入れる

		//---------------------------POI訪問順序を決定---------------------------------------------
		int phase_id = 0;

		//userのvisited_poisを用いて，始点がstart_poiの巡回セールスマン問題の解のうち，ガウス分布に従い，ある結果を抽出する．
		std::vector<std::shared_ptr<Map::BasicPoi const>> order_visited_poi = Gaussian_distribution(input_poi_list);

		int dest_rest_time = 0;//phaseの到着時間と実際の到着時間の差分.最初だけ0
		//最初の点を登録
		std::vector<std::shared_ptr<Map::BasicPoi const>>::iterator now_poi = order_visited_poi.begin();
		std::vector<std::shared_ptr<Map::BasicPoi const>>::iterator next_poi = order_visited_poi.begin() + 1;

		user->set_position_of_phase(phase_id, Graph::MapNodeIndicator((*now_poi)->get_id()), (*now_poi)->data->get_position());
		user->set_random_now_speed(phase_id, AVERAGE_SPEED, RANGE_OF_SPEED);
		user->set_random_pause_time(phase_id, MIN_PAUSE_TIME, MAX_PAUSE_TIME);
		//--------------二個目以降の点を決定．forで，確保したいpoiの個数分をループさせる-------------------
		for (int i = 1; i < POI_NUM; i++)
		{
			double moving_time_between_poi_and_next_poi = map->calc_necessary_time((*now_poi)->get_id(), (*next_poi)->get_id(), user->get_now_speed(phase_id));
			int next_arrive_time = moving_time_between_poi_and_next_poi + user->get_pause_time();

			int rest_pause_time = user->get_pause_time() - dest_rest_time;

			//停止時間をphaseに換算し，pause_timeと最短路経路からpathを決定していく
			div_t variable_of_converted_pause_time_to_phase = std::div(rest_pause_time, SERVICE_INTERVAL);

			//停止時間分，各phaseに停止場所と移動速度(0)を登録
			set_pause_time_and_phases_of_visited_POI(&phase_id, variable_of_converted_pause_time_to_phase, now_poi);

			std::vector<Graph::MapNodeIndicator> shortests_path_between_pois = map->get_shortest_path((*now_poi)->get_id(), (*next_poi)->get_id());
			std::vector<Graph::MapNodeIndicator>::iterator path_iter = shortests_path_between_pois.begin();//pathを検索するためのindex
																										   //速度はphaseで埋める前を参照しなければならないことに注意
			double pause_position_speed = user->get_now_speed(phase_id - variable_of_converted_pause_time_to_phase.quot);

			//最初だけ停止時間をphaseに換算した時の余りをtimeとし，それ以外はservice_intervalをtimeとして，現在地から求めたい地点のdistanceを計算
			double distance = (SERVICE_INTERVAL - variable_of_converted_pause_time_to_phase.rem) * pause_position_speed;
			double distance_between_now_and_next_poi = map->shortest_distance((*now_poi)->get_id(), (*next_poi)->get_id());

			Graph::MapNodeIndicator nearest_position = (*now_poi)->get_id();

			//pathを作成．場所は一番近いintersection同士で線形補間する．MapNodeIndicatorのTypeはINVALIDとする．
			while (distance < distance_between_now_and_next_poi) {
				//最短路の中で一番近いintersectionを探し，線形補間する．		
				set_path_between_poi(now_poi, path_iter, nearest_position, pause_position_speed, SERVICE_INTERVAL, &distance, &phase_id);
			}

			//destinationのところまで補完できたら，rest_timeを保持しておく！
			double distance_between_arrive_position_and_dest_position = distance - distance_between_now_and_next_poi;
			dest_rest_time = distance_between_arrive_position_and_dest_position / pause_position_speed;

			//目的地の登録
			phase_id++;
			user->set_position_of_phase(phase_id, (*next_poi)->get_id(), map->get_static_poi((*next_poi)->get_id())->data->get_position());
			user->set_random_pause_time(phase_id, MIN_PAUSE_TIME, MAX_PAUSE_TIME);
			user->set_random_now_speed(phase_id, AVERAGE_SPEED, RANGE_OF_SPEED);

			now_poi++;
			next_poi++;
		}

		//---------------------------------end_timeまで適当に経路を決める！---------------------------------------------------

		Math::Probability generator;
		//最終地点は少し遠くにとる(1.5倍～2倍)．ただし，マップの限界範囲に注意
		double last_distance = 1.3 * (end_time - time_manager->time_of_phase(phase_id)) * user->get_now_speed(phase_id);

		//次の候補点の範囲を求める
		double last_angle = generator.uniform_distribution(-(M_PI), M_PI_2);
		Geography::LatLng last_candidate_poi_position_range
			= Geography::GeoCalculation::calc_translated_point((*now_poi)->data->get_position(), last_distance, last_angle);

		Graph::Rectangle<Geography::LatLng> last_range(last_candidate_poi_position_range.lat() + 0.001, last_candidate_poi_position_range.lng(), last_candidate_poi_position_range.lat(), last_candidate_poi_position_range.lng() + 0.001);

		std::vector<std::shared_ptr<Map::BasicPoi const>> last_candidate_pois_list = get_pois_list(last_range);
		//次のPOIの決定
		std::vector<std::shared_ptr<Map::BasicPoi const>>::iterator last_poi = last_candidate_pois_list.begin();

		//現在地の停止時間をランダムで設定し，現地点の出発地の速度で，次のPOIまでの最短路で移動した時の時間を求める．
		double moving_time_between_poi_and_next_poi = map->calc_necessary_time((*now_poi)->get_id(), (*last_poi)->get_id(), user->get_now_speed(phase_id));
		int next_arrive_time = moving_time_between_poi_and_next_poi + user->get_pause_time();

		//停止時間をphaseに換算し，pause_timeと最短路経路からpathを決定していく
		int rest_pause_time = user->get_pause_time() - dest_rest_time;
		div_t last_variable_of_converted_pause_time_to_phase = std::div(rest_pause_time, SERVICE_INTERVAL);

		std::vector<Graph::MapNodeIndicator> last_shortests_path = map->get_shortest_path((*now_poi)->get_id(), (*last_poi)->get_id());

		//停止時間分，各phaseに停止場所と移動速度(0)を登録
		set_pause_time_and_phases_of_visited_POI(&phase_id, last_variable_of_converted_pause_time_to_phase, now_poi);

		std::vector<Graph::MapNodeIndicator>::iterator last_path_iter = last_shortests_path.begin();//pathを検索するためのindex
																									//速度はphaseで埋める前を参照しなければならないことに注意
		double last_pause_position_speed = user->get_now_speed(phase_id - last_variable_of_converted_pause_time_to_phase.quot);

		//最初だけ停止時間をphaseに換算した時の余りをtimeとし，それ以外はservice_intervalをtimeとして，現在地から求めたい地点のdistanceを計算
		double distance = (SERVICE_INTERVAL - last_variable_of_converted_pause_time_to_phase.rem) * last_pause_position_speed;
		double distance_between_now_and_next_poi = map->shortest_distance((*now_poi)->get_id(), (*last_poi)->get_id());

		Graph::MapNodeIndicator last_nearest_position = (*now_poi)->get_id();

		//pathを作成．場所は一番近いintersection同士で線形補間する．MapNodeIndicatorのTypeはINVALIDとする．
		//last_phaseまで埋める！
		while (phase_id != (time_manager->phase_count() - 1)) {
			set_path_between_poi(now_poi, last_path_iter, last_nearest_position, last_pause_position_speed, SERVICE_INTERVAL, &distance, &phase_id);
		}

		std::cout << "Success Creating Real User" << std::endl;

	}

	
	///<summary>
	/// ダミーのtrajectoryを出力する
	///</summary>
	void HayashidaSimulator::export_dummy_trajectory(std::shared_ptr<Entity::EntityManager<Entity::RevisablePauseMobileEntity<Geography::LatLng>, Entity::PauseMobileEntity<Geography::LatLng>, Geography::LatLng>> entities, std::shared_ptr<Time::Timer> timer, int dummy_id)
	{
		IO::FileExporter dummy_exporter({
			{ Geography::LatLng::LATITUDE, "緯度" },
			{ Geography::LatLng::LONGITUDE, "経度" }
		}, DUMMY_TRAJECTORT_OUT_PATH);

		std::list<std::shared_ptr<IO::FileExportable const>> dummy_exportable_positions;
		time_manager->for_each_time([&](time_t time, long interval, int phase) {
			dummy_exportable_positions.push_back(entities->read_dummy_by_id(dummy_id)->read_position_of_phase(phase));
		});

	}


	///<summary>
	/// ダミーのtrajectoryを出力する
	///</summary>
	void HayashidaSimulator::export_dummies_trajectory(std::shared_ptr<Entity::EntityManager<Entity::RevisablePauseMobileEntity<Geography::LatLng>, Entity::PauseMobileEntity<Geography::LatLng>, Geography::LatLng>> entities, std::shared_ptr<Requirement::KatoMethodRequirement const> requirement, std::shared_ptr<Time::Timer> timer)
	{
		
		entities->for_each_dummy([&](int dummy_id, std::shared_ptr<Entity::RevisablePauseMobileEntity<Geography::LatLng>> dummy) {
			std::string file_name = DUMMY_TRAJECTORT_OUT_PATH + std::to_string(dummy_id);
			std::cout << file_name << std::endl;
			IO::FileExporter dummies_exporter({
				{ Geography::LatLng::LATITUDE, "緯度" },
				{ Geography::LatLng::LONGITUDE, "経度" }
			}, file_name);
			
			std::list<std::shared_ptr<IO::FileExportable const>> dummy_exportable_positions;
			time_manager->for_each_time([&](time_t time, long interval, int phase) {
				dummy_exportable_positions.push_back(entities->read_dummy_by_id(dummy_id)->read_position_of_phase(phase));
			});
			dummies_exporter.export_lines(dummy_exportable_positions);
		});
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
		make_random_movement_user();
		//make_predicted_user();
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
			int min_pause_time = 300,
			double average_speed = 1.5,
			double speed_range = 0.5
		---------------------------------------------*/

		//匿名領域決定の周期のパラメータはユーザの総移動時間/周期T個！


		requirements = 
		{
			std::make_shared<Requirement::KatoMethodRequirement>(600 * 600, 4, 90, 5, 3, 500, 200),
			//std::make_shared<Requirement::KatoMethodRequirement>(1000 * 1000, 16, 90, 5, 2)
		};
	}

	///<summary>
	/// map,user,requirementの作成
	///</summary>
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
			kato_bachelor_method.set_execution_callback(std::bind(&HayashidaSimulator::export_dummies_trajectory,this,std::placeholders::_1, std::placeholders::_2, std::placeholders::_3));
			kato_bachelor_method.run();
		}
	}

	///<summary>
	/// 評価部分実装
	///</summary>
	void HayashidaSimulator::evaluate()
	{
	
	}

	///<summary>
	/// userのトラジェクトリファイルエクスポート
	///</summary>
	void HayashidaSimulator::export_evaluation_result()
	{
		IO::FileExporter user_exporter({
			{Geography::LatLng::LATITUDE, "緯度"},
			{Geography::LatLng::LONGITUDE, "経度"}
		}, USER_TRAJECTORY_OUT_PATH);

		std::list<std::shared_ptr<IO::FileExportable const>> user_exportable_positions;
		time_manager->for_each_time([&](time_t time, long interval, int phase) {
			user_exportable_positions.push_back(user->read_position_of_phase(phase));
		});
				
		user_exporter.export_lines(user_exportable_positions);
	}

	
}