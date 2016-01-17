#include "stdafx.h"
#include "HayashidaSimulator.h"

namespace Simulation
{
	///<summary>
	/// コンストラクタ
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
	/// time_managerの生成
	/// predicted_userと同じ個数作る！
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

		if (*phase_id == time_manager->phase_count() - 1) return;
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
	/// 停止時間とphaseのセット
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
	/// inputするpoiのlistを作成
	///</summary>
	void HayashidaSimulator::input_visit_pois() {
		//POI系列を作成する際の，選択範囲
		Graph::Rectangle<Geography::LatLng> pois_range = map_boundary.transform_rect_of_latlang_to_x_times(0.4);

		std::vector<std::shared_ptr<Map::BasicPoi const>> random_pois_list = get_pois_list(pois_range);
		std::vector<std::shared_ptr<Map::BasicPoi const>>::iterator iter = random_pois_list.begin();
		for (int i = 0; i < POI_NUM; i++, iter++) {
			input_poi_list.push_back(*iter);
		}
	}

	///<summary>
	/// input_elementsの入力POIを基に，巡回セールスマン問題を解き，総距離が小さい順にソートしたvectorを返す．
	///</summary>
	std::vector<std::pair<std::vector<std::shared_ptr<Map::BasicPoi const>>, double>> HayashidaSimulator::all_traveling_salesman_problem(std::vector<std::shared_ptr<Map::BasicPoi const>>& visited_pois)
	{
		std::vector<std::pair<std::vector<std::shared_ptr<Map::BasicPoi const>>, double>> all_tsp_solution;
		std::shared_ptr<Map::BasicPoi const> start_poi = visited_pois.front();//最初の訪問POI
		
		std::vector<std::shared_ptr<Map::BasicPoi const>> visited_pois_except_start_poi;//最初の訪問POIを除いたPOI_list
		for (std::vector<std::shared_ptr<Map::BasicPoi const>>::iterator iter = visited_pois.begin() + 1; iter != visited_pois.end(); iter++) {
			visited_pois_except_start_poi.push_back(*iter);
		}

		//id順にソート
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

		//表示用
		std::for_each(all_tsp_solution.begin(), all_tsp_solution.end(), [](std::pair<std::vector<std::shared_ptr<Map::BasicPoi const>>, double>& tsp) {
			for (auto iter = tsp.first.begin(); iter != tsp.first.end(); iter++) std::cout << "( Node ID : " << (*iter)->get_id() << " ) → ";
			std::cout << " distance : " << tsp.second << std::endl;
		});

		//ファイル出力用
		std::ofstream ofs("C:/Users/Shuhei/Desktop/Result_Path/tsp_distance.txt");
		std::for_each(all_tsp_solution.begin(), all_tsp_solution.end(), [&ofs](std::pair<std::vector<std::shared_ptr<Map::BasicPoi const>>, double>& tsp) {
			ofs << tsp.second << std::endl;
		});

		return all_tsp_solution;
	}


	///<summary>
	/// input_elementsの入力POIを基に，巡回セールスマン問題を解き，最適解を返す．
	///</summary>
	std::pair<std::vector<std::shared_ptr<Map::BasicPoi const>>, double> HayashidaSimulator::traveling_salesman_problem(std::vector<std::shared_ptr<Map::BasicPoi const>>& visited_pois)
	{
		return all_traveling_salesman_problem(visited_pois).front();
	}

	///<summary>
	/// input_elementsの入力POIを基に，巡回セールスマン問題を解き，距離の比の逆数の分布に従って一定確率で求めたpoi系列を返す．
	///</summary>
	std::pair<std::vector<std::shared_ptr<Map::BasicPoi const>>, double> HayashidaSimulator::the_power_alpha_of_the_reciprocal_of_the_ratio_of_the_optimal_distance(std::vector<std::shared_ptr<Map::BasicPoi const>>& visited_pois, int alpha)
	{
		std::vector<std::pair<std::vector<std::shared_ptr<Map::BasicPoi const>>, double>> all_tsp_solution = all_traveling_salesman_problem(visited_pois);
		double optimal_distance = all_tsp_solution.front().second;
				
		std::vector<double> distance_ratio_list;//距離の比の逆数を格納
		std::vector<double> normalization_ratio_list;//距離の比の逆数を正規化したものを格納

		//最適解に対する距離の比を計算
		//alphaを忘れずに
		for (size_t i = 0; i < all_tsp_solution.size(); i++) {
			alpha != 0 ? distance_ratio_list.push_back((1.0/(all_tsp_solution.at(i).second/ optimal_distance)) * alpha) : distance_ratio_list.push_back(1.0);
		}

		//距離の比の逆数を正規化
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
	/// ランダムムーブを行うユーザを生成する
	/// ただし，現在は最短経路で移動
	///</summary>
	void HayashidaSimulator::make_random_movement_user()
	{
		Math::Probability generator;

		//rect_init_langには始点にしたい範囲をインスタンスで入力
		Graph::Rectangle<Geography::LatLng> rect_init_range(BASE_LAT + 0.5*length_of_rect, BASE_LNG - 0.5*length_of_rect, BASE_LAT - 0.5*length_of_rect, BASE_LNG + 0.5*length_of_rect);
	 
		//----------------------------time_managerの生成-------------------------------------//
		time_manager = create_time_manager();
		
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

			double moving_time_between_poi_and_next_poi = map->calc_necessary_time((*now_poi)->get_id(), (*next_poi)->get_id(), user->get_starting_speed());
			int next_arrive_time = moving_time_between_poi_and_next_poi + user->get_pause_time();

			double rest_pause_time = user->get_pause_time() - dest_rest_time;

			//停止時間をphaseに換算し，pause_timeと最短路経路からpathを決定していく
			lldiv_t variable_of_converted_pause_time_to_phase = std::lldiv(rest_pause_time, SERVICE_INTERVAL);
			
			//出発するときの余り時間を登録
			user->set_rest_pause_time_when_departing(variable_of_converted_pause_time_to_phase.rem);

			//停止時間分，各phaseに停止場所と移動速度(0)を登録
			set_pause_time_and_phases_of_visited_POI(&phase_id, rest_pause_time, variable_of_converted_pause_time_to_phase.quot, now_poi);
			
			std::vector<Graph::MapNodeIndicator> shortests_path_between_pois = map->get_shortest_path((*now_poi)->get_id(), (*next_poi)->get_id());
			std::vector<Graph::MapNodeIndicator>::iterator path_iter = shortests_path_between_pois.begin();//pathを検索するためのindex
			//速度はphaseで埋める前を参照しなければならないことに注意
			double pause_position_speed = user->get_starting_speed();
			
			//最初だけ停止時間をphaseに換算した時の余りをtimeとし，それ以外はservice_intervalをtimeとして，現在地から求めたい地点のdistanceを計算
			double distance = (SERVICE_INTERVAL - variable_of_converted_pause_time_to_phase.rem) * pause_position_speed;
			double distance_between_now_and_next_poi = map->shortest_distance((*now_poi)->get_id(), (*next_poi)->get_id());

			Graph::MapNodeIndicator nearest_position = (*now_poi)->get_id();

			//pathを作成．場所は一番近いintersection同士で線形補間する．MapNodeIndicatorのTypeはINVALIDとする．
			while (distance < distance_between_now_and_next_poi && phase_id != time_manager->phase_count() - 1 ) {
				//最短路の中で一番近いintersectionを探し，線形補間する．
				set_path_between_poi(now_poi, path_iter, nearest_position, pause_position_speed, SERVICE_INTERVAL, &distance, &phase_id);
			}
			
			//destinationのところまで補完できたら，rest_timeを保持しておく！
			double distance_between_arrive_position_and_dest_position = distance - distance_between_now_and_next_poi;
			dest_rest_time = distance_between_arrive_position_and_dest_position / pause_position_speed;
			//目的地の登録
			if (phase_id == time_manager->phase_count() - 1) return;//もし経路生成中に全て埋まってしまったら，その時点で終了

			phase_id++;
			user->increment_visited_pois_info_list_id();
			user->set_visited_poi_of_phase(phase_id, (*next_poi)->get_id(), map->get_static_poi((*next_poi)->get_id())->data->get_position());
			user->set_random_starting_speed_at_poi(AVERAGE_SPEED, RANGE_OF_SPEED);

			//現在位置の更新
			*now_poi = *next_poi;

		}

		//---------------------------------end_timeまで適当に経路を決める！---------------------------------------------------
		//last_behavior_of_user(phase_id, dest_rest_time, now_poi);
		
		std::cout << "Success Creating Random User" << std::endl;
	}

	

	///<summary>
	/// input_poisを基に，T.S.P.を用い，予想ユーザを作成する．
	///</summary>
	std::shared_ptr<Entity::RevisablePauseMobileEntity<Geography::LatLng>> HayashidaSimulator::create_artificial_user(std::pair<std::vector<std::shared_ptr<Map::BasicPoi const>>, double>& order_visited_poi) {
		int phase_id = 0;
		Math::Probability generator;

		//以下のリストを作って，コンストラクタにつっこむ
		std::shared_ptr<std::vector<std::shared_ptr<Geography::LatLng const>>> positions = std::make_shared<std::vector<std::shared_ptr<Geography::LatLng const>>>();
		std::shared_ptr<std::vector<Graph::MapNodeIndicator>> visited_node_ids	= std::make_shared<std::vector<Graph::MapNodeIndicator>>();
		std::shared_ptr<std::vector<std::string>> venue_names = std::make_shared<std::vector<std::string>>();
		std::vector<double> now_speed_list;
		std::vector<double> current_pause_time_list;
		
		std::vector<Entity::VisitedPoiInfo> visited_pois_info_list;
		Entity::VisitedPoiInfo visited_poi_info;
		
		//---------------------------POI訪問順序を決定---------------------------------------------

		//userのvisited_poisを用いて，始点がstart_poiの巡回セールスマン問題を解く
		
		//最初の情報を色々決める
		//最初の点を登録
		std::vector<std::shared_ptr<Map::BasicPoi const>>::iterator now_poi = order_visited_poi.first.begin();
		double pause_time = generator.uniform_distribution(MIN_PAUSE_TIME, MAX_PAUSE_TIME);
		double speed = generator.uniform_distribution(MIN_SPEED, MAX_SPEED);

		double dest_rest_time = 0.0;//phaseの到着時間と実際の到着時間の差分.最初だけ0
		double rest_pause_time = pause_time - dest_rest_time;
		lldiv_t variable_of_converted_pause_time_to_phase = std::lldiv(rest_pause_time, SERVICE_INTERVAL);

		//visited_poi_infoに	情報を登録
		visited_poi_info.visited_poi = std::make_pair(Graph::MapNodeIndicator((*now_poi)->get_id(), Graph::POI), (*now_poi)->data->get_position());
		for (int id = 0; id <= variable_of_converted_pause_time_to_phase.quot; id++) visited_poi_info.pause_phases.push_back(phase_id + id);
		visited_poi_info.arrive_phase = phase_id;
		visited_poi_info.pause_time = pause_time;
		visited_poi_info.starting_speed = speed;
		visited_poi_info.rest_pause_time_when_departing = variable_of_converted_pause_time_to_phase.rem;
		visited_poi_info.dest_rest_time = dest_rest_time;
		visited_pois_info_list.push_back(visited_poi_info);
		
		//位置，node_ids, venue_name，停止時間，停止フェーズを登録
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

		//--------------二個目以降の点を決定．forで，確保したいpoiの個数分をループさせる-------------------
		for (int i = 1; i < POI_NUM; i++)
		{	

			double moving_time_between_poi_and_next_poi = map->calc_necessary_time((*now_poi)->get_id(), (*next_poi)->get_id(), speed);
			double next_arrive_time = moving_time_between_poi_and_next_poi + pause_time;
						
			std::vector<Graph::MapNodeIndicator> shortests_path_between_pois = map->get_shortest_path((*now_poi)->get_id(), (*next_poi)->get_id());
			std::vector<Graph::MapNodeIndicator>::iterator path_iter = shortests_path_between_pois.begin();//pathを検索するためのindex
			
			//最初だけ停止時間をphaseに換算した時の余りをtimeとし，それ以外はservice_intervalをtimeとして，現在地から求めたい地点のdistanceを計算
			//double distance = (SERVICE_INTERVAL - variable_of_converted_pause_time_to_phase.rem) * speed;
			double distance = SERVICE_INTERVAL * speed;
			double distance_between_now_and_next_poi = map->shortest_distance((*now_poi)->get_id(), (*next_poi)->get_id());

			Graph::MapNodeIndicator nearest_position = (*now_poi)->get_id();

			//pathを作成．場所は一番近いintersection同士で線形補間する．MapNodeIndicatorのTypeはINVALIDとする．
			while (distance < distance_between_now_and_next_poi) {
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

				//登録
				positions->push_back(std::make_shared<Geography::LatLng>(arrive_position));
				visited_node_ids->push_back(Graph::MapNodeIndicator(Graph::NodeType::OTHERS, Graph::NodeType::OTHERS));
				venue_names->push_back("road");
				now_speed_list.push_back(speed);
				current_pause_time_list.push_back(0.0);
				
				distance += SERVICE_INTERVAL * speed;
				phase_id++;				
			}
			//destinationのところまで補完できたら，rest_timeを保持しておく！
			double distance_between_arrive_position_and_dest_position = distance - distance_between_now_and_next_poi;
			
			//次の情報を色々決める
			pause_time = generator.uniform_distribution(MIN_PAUSE_TIME, MAX_PAUSE_TIME);
			speed = generator.uniform_distribution(MIN_SPEED, MAX_SPEED);
			rest_pause_time = pause_time - dest_rest_time;
			variable_of_converted_pause_time_to_phase = std::lldiv(rest_pause_time, SERVICE_INTERVAL);
			dest_rest_time = distance_between_arrive_position_and_dest_position / speed;


			//visited_poi_infoに	情報を登録
			visited_poi_info.visited_poi = std::make_pair(Graph::MapNodeIndicator((*next_poi)->get_id(), Graph::POI), (*next_poi)->data->get_position());
			while (!visited_poi_info.pause_phases.empty()) visited_poi_info.pause_phases.pop_back();
			for (int id = 0; id <= variable_of_converted_pause_time_to_phase.quot; id++) visited_poi_info.pause_phases.push_back(phase_id + id);
			visited_poi_info.arrive_phase = phase_id;
			visited_poi_info.pause_time = pause_time;
			visited_poi_info.starting_speed = speed;
			visited_poi_info.rest_pause_time_when_departing = variable_of_converted_pause_time_to_phase.rem;
			visited_poi_info.dest_rest_time = dest_rest_time;
			visited_pois_info_list.push_back(visited_poi_info);


			//位置，node_ids, venue_name，停止時間，停止フェーズを登録
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
		//最後にtimeslotsの作成＋end_timeの作成
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
	/// ユーザとダミーのtrajectoryを出力する
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
			{ Geography::LatLng::LATITUDE, "緯度" },
			{ Geography::LatLng::LONGITUDE, "経度" },
			//{ Graph::TrajectoryState<>::TIME, "タイムスタンプ" },
			//{ Graph::SemanticTrajectoryState<>::CATEGORY, "カテゴリID" },
			//{ Graph::SemanticTrajectoryState<>::CATEGORY_NAME, "カテゴリ名" },
			//{ Graph::TrajectoryState<>::VENUE_NAME, "POI名" }
		};

		//実際のユーザトラジェクトリのエクスポート
		IO::FileExporter real_user_exporter(export_name_map_of_entities, REAL_USER_TRAJECTORY_OUT_PATH);
		std::list<std::shared_ptr<IO::FileExportable const>> real_user_trajectory = entities->get_user()->get_real_user()->read_trajectory()->get_export_data();
		real_user_exporter.export_lines(real_user_trajectory);


		//予測したユーザトラジェクトリのエクスポート
		IO::FileExporter predicted_user_exporter(export_name_map_of_entities, PREDICTED_USER_TRAJECTORY_OUT_PATH);
		std::list<std::shared_ptr<IO::FileExportable const>> predicted_user_trajectory = entities->get_user()->read_trajectory()->get_export_data();
		predicted_user_exporter.export_lines(predicted_user_trajectory);
		

		/*
		//実際のユーザの訪問POIのエクスポート
		std::ofstream ofs_of_real_user_visited_pois(REAL_USER_VISITED_POIS_OUT_PATH);
		for (int id = 0; real_user->get_visited_pois_num(); id++) {
			Graph::node_id poi_id = entities->get_user()->get_real_user()->get_any_visited_poi(id).first.id();
			ofs_of_real_user_visited_pois << poi_id << std::endl;
		}
		
		//予想ユーザの訪問POIのエクスポート
		std::ofstream ofs_of_predicted_user_visited_pois(REAL_USER_VISITED_POIS_OUT_PATH);
		for (int id = 0; real_user->get_visited_pois_num(); id++) {
			Graph::node_id poi_id = entities->get_user()->get_predicted_user()->get_any_visited_poi(id).first.id();
			ofs_of_predicted_user_visited_pois << poi_id << std::endl;
		}

		//全ダミーの訪問POIのエクスポート
		std::ofstream ofs_of_dummies_visited_pois(DUMMIES_VISITED_POIS_OUT_PATH);
		for (int dummy_id = 1; dummy_id < requirement->dummy_num; dummy_id++) {
			for (int id = 0; real_user->get_visited_pois_num(); id++) {
				Graph::node_id poi_id = entities->get_dummy_by_id(dummy_id)->get_any_visited_poi(id).first.id();
				ofs_of_dummies_visited_pois << poi_id << std::endl;
			}
			ofs_of_dummies_visited_pois << "?n";
		}

		//各ダミーのエクスポート
		for (int dummy_id = 1; dummy_id <= entities->get_dummy_count(); dummy_id++) {
			std::shared_ptr<Entity::RevisablePauseMobileEntity<> const> dummy = entities->read_dummy_by_id(dummy_id);
			std::list<std::shared_ptr<IO::FileExportable const>> dummy_trajectory = dummy->read_trajectory()->get_export_data();
			IO::FileExporter dummy_exporter(export_name_map_of_entities, DUMMY_TRAJECTORT_OUT_PATH + std::to_string(dummy_id));
			dummy_exporter.export_lines(dummy_trajectory);
		}
		*/

		//AR系の評価
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
	/// input_listを読み取り専用で取得
	///</summary>
	std::vector<std::shared_ptr<Map::BasicPoi const>> HayashidaSimulator::read_input_poi_list() const
	{
		return input_poi_list;
	}

	///<summary>
	/// map_dataを生成する
	///</summary>
	void HayashidaSimulator::build_map(const Graph::Rectangle<Geography::LatLng>& boundary)
	{
		map = std::make_shared<Map::HayashidaDbMap>(std::make_shared<Graph::Dijkstra<Map::BasicMapNode, Map::BasicRoad>>(), "../settings/mydbsettings.xml", "map_tokyo");
		map->load(boundary);
	}


	///<summary>
	/// ユーザを生成する
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


		//このtime_managerはpredictedのtimeslotにする！
		time_manager = create_time_manager();

		user = std::make_shared<Entity::DifferentMovementUser<Geography::LatLng>>(0, time_manager, real_user, predicted_user);
	}



	///<summary>
	/// 要求パラメータを作成．
	/// コンストラクタで具体的な値を入れること
	///</summary>
	void HayashidaSimulator::make_requirement_list()
	{
		//加藤さん手法の要求パラメータ
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
			std::make_shared<Requirement::KatoMethodRequirement>(600 * 600, 2, 90, 5, 2, 600, 250)
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
			/*
			//加藤さん卒論手法
			Method::KatoBachelorMethod kato_bachelor_method(map,user,*requirement,time_manager);
			kato_bachelor_method.set_execution_callback(std::bind(&HayashidaSimulator::export_users_and_dummies_trajectory,this,std::placeholders::_1, std::placeholders::_2, std::placeholders::_3));
			kato_bachelor_method.run();
			*/
			
			/*
			//加藤さん修論手法
			Method::KatoMasterMethod kato_master_method(map, user, *requirement, time_manager);
			kato_master_method.set_execution_callback(std::bind(&HayashidaSimulator::export_users_and_dummies_trajectory, this, std::placeholders::_1, std::placeholders::_2, std::placeholders::_3));
			kato_master_method.run();
			*/

			//林田さん卒論手法
			Method::HayashidaBachelorMethod hayashida_bachelor_method(map, user, *requirement, time_manager);
			hayashida_bachelor_method.set_execution_callback(std::bind(&HayashidaSimulator::export_users_and_dummies_trajectory, this, std::placeholders::_1, std::placeholders::_2, std::placeholders::_3));
			hayashida_bachelor_method.run();
		}
	}

	///<summary>
	/// 評価部分実装
	/// ここにMapとUserだけ使って出来る実験があれば実装する
	///</summary>


	///<summary>
	/// userのトラジェクトリファイルエクスポート
	///</summary>
	void HayashidaSimulator::export_evaluation_result(const Requirement::KatoMethodRequirement& requirement)
	{
	

		/*
		IO::FileExporter user_exporter({
			{Geography::LatLng::LATITUDE, "緯度"},
			{Geography::LatLng::LONGITUDE, "経度"},
			{Graph::TrajectoryState<>::TIME, "タイムスタンプ" }
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
		{ Geography::LatLng::LATITUDE, "緯度" },
		{ Geography::LatLng::LONGITUDE, "経度" }
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