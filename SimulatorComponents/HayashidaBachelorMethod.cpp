#include "stdafx.h"
#include "HayashidaBachelorMethod.h"

namespace Method
{

	///<summary>
	/// コンストラクタ
	/// これにSimulatorで作成した各種入力への参照を渡す
	///</summary>
	HayashidaBachelorMethod::HayashidaBachelorMethod(std::shared_ptr<Map::HayashidaDbMap const> map, std::shared_ptr<Entity::DifferentMovementUser<Geography::LatLng>> user, std::shared_ptr<Requirement::KatoMethodRequirement const> requirement, std::shared_ptr<Time::TimeSlotManager> time_manager)
		: KatoMasterMethod(map, user, requirement, time_manager)
	{
	}

	///<summary>
	/// デストラクタ
	///</summary>
	HayashidaBachelorMethod::~HayashidaBachelorMethod()
	{
	}

	///<summary>
	/// ユーザが訪問予定POIに向かっているかどうかをチェックする
	///</summary>
	bool HayashidaBachelorMethod::is_user_moving_to_POI()
	{
		return true;
	}


	///<summary>
	/// ユーザが訪問予定POIに向かっているかどうかをチェックする
	///</summary>
	Graph::MapNodeIndicator HayashidaBachelorMethod::predict_next_poi() {
		return Graph::MapNodeIndicator(0);
	}

	///<summary>
	/// ユーザの行動を再予測する
	///</summary>
	void HayashidaBachelorMethod::repredicting_user_trajectory(int now_phase)
	{
		//---------------------------POI訪問順序を決定---------------------------------------------
		std::vector<Graph::MapNodeIndicator> rest_poi_list;
		//次に向かうPOIを取得
		Graph::MapNodeIndicator first_poi = predict_next_poi();
		rest_poi_list.push_back(first_poi);

		//直前のvisited_poi_id
		int prev_id = predicted_user->get_visited_pois_info_list_id() - 1;
		
		//残りの訪問POIをpush_back
		for (int id = predicted_user->get_visited_pois_info_list_id(); id < predicted_user->get_visited_pois_num(); id++) {
			if (predicted_user->get_any_visited_poi(id).first == first_poi) continue;
			rest_poi_list.push_back(predicted_user->get_any_visited_poi(id).first);
		}
		
		//再度ユーザの訪問POI順序をT.S.P.を用いて予測
		std::vector<Graph::MapNodeIndicator> order_visited_poi = repredict_visited_poi_order_by_tsp(rest_poi_list);
		Graph::MapNodeIndicator prev_position = predicted_user->get_any_visited_poi(prev_id).first.id();
		//先頭に直前の訪問POIを登録しておく
		auto iter = order_visited_poi.begin();
		order_visited_poi.insert(iter, prev_position);
		std::vector<Graph::MapNodeIndicator>::iterator now_poi = order_visited_poi.begin();
		std::vector<Graph::MapNodeIndicator>::iterator next_poi = order_visited_poi.begin() + 1;//これはfirst_poiと等しい！

		//VisitedPoiInfoのリセット
		//ただし，リセットする前に訪問POIの数と到着フェーズを記録しておく
		const int POI_NUM = predicted_user->get_visited_pois_num();
		int prev_arrive_phase = predicted_user->get_arrive_phase_of_any_visited_poi(prev_id);
		
		for (int id = predicted_user->get_visited_pois_info_list_id(); id < POI_NUM; id++) {
			predicted_user->get_visited_poi_info_list().pop_back();
		}
				
		double dest_rest_time = predicted_user->get_dest_rest_time_of_any_visited_poi(prev_id);//phaseの到着時間と実際の到着時間の差分.最初だけ0
		int phase_id = prev_arrive_phase;
		//-------------残りの点を決定．forで，確保したいpoiの個数分をループさせる-------------------
		for (int i = predicted_user->get_visited_pois_info_list_id(); i < POI_NUM; i++)
		{
			double moving_time_between_poi_and_next_poi = map->calc_necessary_time((*now_poi).id(), (*next_poi).id(), predicted_user->get_now_speed(now_phase));
			int next_arrive_time = moving_time_between_poi_and_next_poi + predicted_user->get_pause_time();

			double rest_pause_time = predicted_user->get_pause_time_of_any_visited_poi(i-1) - dest_rest_time;

			//停止時間をphaseに換算し，pause_timeと最短路経路からpathを決定していく
			lldiv_t variable_of_converted_pause_time_to_phase = std::lldiv(rest_pause_time, requirement->service_interval);

			//出発するときの余り時間を登録
			predicted_user->set_rest_pause_time_when_departing(variable_of_converted_pause_time_to_phase.rem);

			//停止時間分，各phaseに停止場所と移動速度(0)を登録
			set_predicted_user_pause_time_and_phases(&phase_id, rest_pause_time, variable_of_converted_pause_time_to_phase.quot, *now_poi);

			//現在地．ただし，線形補間した場所なので，そのままshortest_distanceは使えないため，もう一回計算する．
			std::vector<Graph::MapNodeIndicator> shortests_path_between_pois = map->get_shortest_path(predicted_user->get_any_visited_poi(prev_id).first.id(), first_poi.id());
			std::vector<Graph::MapNodeIndicator>::iterator path_iter = shortests_path_between_pois.begin();//pathを検索するためのindex

			//速度は現在の速度を参照
			double pause_position_speed = predicted_user->get_starting_speed_using_arrive_phase(phase_id);

			//最初だけ停止時間をphaseに換算した時の余りをtimeとし，それ以外はservice_intervalをtimeとして，現在地から求めたい地点のdistanceを計算
			double distance = (requirement->service_interval - predicted_user->get_rest_pause_time_when_departing_of_any_visited_poi(prev_id)) * pause_position_speed;
			double distance_between_now_and_next_poi = map->shortest_distance(prev_position.id(), first_poi.id());

			Graph::MapNodeIndicator nearest_position = prev_position.id();

			//pathを作成．場所は一番近いintersection同士で線形補間する．MapNodeIndicatorのTypeはINVALIDとする．
			while (distance < distance_between_now_and_next_poi && phase_id != time_manager->phase_count() - 1) {
				//最短路の中で一番近いintersectionを探し，線形補間する．     
				set_predicted_user_path(now_poi, path_iter, nearest_position, pause_position_speed, requirement->service_interval, &distance, &phase_id);
			}

			//destinationのところまで補完できたら，rest_timeを保持しておく！
			double distance_between_arrive_position_and_dest_position = distance - distance_between_now_and_next_poi;
			dest_rest_time = distance_between_arrive_position_and_dest_position / pause_position_speed;

			//目的地の登録
			if (phase_id == time_manager->phase_count() - 1) return;//もし経路生成中に全て埋まってしまったら，その時点で終了

			phase_id++;
			predicted_user->increment_visited_pois_info_list_id();
			predicted_user->set_visited_poi_of_phase(phase_id, (*next_poi).id(), map->get_static_poi((*next_poi).id())->data->get_position());
			predicted_user->set_random_starting_speed_at_poi(requirement->average_speed_of_dummy, requirement->speed_range_of_dummy);

			now_poi++;
			next_poi++;
			predicted_user->set_dest_rest_time(dest_rest_time);//到着時の余り分を登録
			predicted_user->set_random_pause_time(phase_id, requirement->min_pause_time, requirement->max_pause_time);
		}

		//---------------------------------end_timeまでの振る舞いを決める！---------------------------------------------------

		last_behavior_of_predicted_user(phase_id, dest_rest_time, *now_poi);
		
		std::cout << "Success Re-Predicting User Plan" << std::endl;
	}


	///<summary>
	/// ユーザの，最後の訪問予定POI到達後の最終フェーズまでの振る舞い
	///</summary>
	void HayashidaBachelorMethod::last_behavior_of_predicted_user(int phase_id, double dest_rest_time, Graph::MapNodeIndicator& now_poi)
	{
		pause_until_last_phase(phase_id, dest_rest_time, now_poi);
		//go_to_random_position(phase_id);
	}

	///<summary>
	/// 最後のフェーズまで停止を続ける
	///</summary>
	void HayashidaBachelorMethod::pause_until_last_phase(int phase_id, double dest_rest_time, Graph::MapNodeIndicator& now_poi)
	{
		//現在地の停止時間をランダムで設定し，現地点の出発地の速度で，次のPOIまでの最短路で移動した時の時間を求める．
		predicted_user->set_random_pause_time(phase_id, requirement->min_pause_time, requirement->max_pause_time);
		predicted_user->set_dest_rest_time(dest_rest_time);

		double rest_pause_time = predicted_user->get_pause_time() - dest_rest_time;
		lldiv_t last_variable_of_converted_pause_time_to_phase = std::lldiv(rest_pause_time, requirement->service_interval);

		predicted_user->set_rest_pause_time_when_departing(last_variable_of_converted_pause_time_to_phase.rem);
		//停止時間分，各phaseに停止場所と停止phaseを登録
		set_predicted_user_pause_time_and_phases(&phase_id, rest_pause_time, last_variable_of_converted_pause_time_to_phase.quot, now_poi);

	}

	///<summary>
	/// どこか一点ランダムに位置を決定し，そこに向かって進む．
	///</summary>
	void HayashidaBachelorMethod::go_to_random_position(int phase_id, double dest_rest_time, Graph::MapNodeIndicator& now_poi)
	{
		/*
		Math::Probability generator;
		int end_time = time_manager->time_of_phase(time_manager->last_phase());

		//最終地点は少し遠くにとる(1.5倍～2倍)．ただし，マップの限界範囲に注意
		double last_distance = 1.4 * (end_time - time_manager->time_of_phase(phase_id)) * predicted_user->get_now_speed(phase_id);

		//次の候補点の範囲を求める
		Geography::LatLng center = map_boundary.center();
		double last_angle = Geography::GeoCalculation::lambert_azimuth_angle((*now_poi)->data->get_position(), center);
		Geography::LatLng last_candidate_poi_position_range
			= Geography::GeoCalculation::calc_translated_point((*now_poi)->data->get_position(), last_distance, last_angle);

		Graph::Rectangle<Geography::LatLng> last_range(last_candidate_poi_position_range.lat() + 0.001, last_candidate_poi_position_range.lng(), last_candidate_poi_position_range.lat(), last_candidate_poi_position_range.lng() + 0.001);

		std::vector<std::shared_ptr<Map::BasicPoi const>> last_candidate_pois_list = get_pois_list(last_range);
		//次のPOIの決定
		std::vector<std::shared_ptr<Map::BasicPoi const>>::iterator last_poi = last_candidate_pois_list.begin();


		//現在地の停止時間をランダムで設定し，現地点の出発地の速度で，次のPOIまでの最短路で移動した時の時間を求める．
		predicted_user->set_random_pause_time(phase_id, requirement->min_pause_time, requirement->max_pause_time);
		predicted_user->set_dest_rest_time(dest_rest_time);


		//現在地の停止時間をランダムで設定し，現地点の出発地の速度で，次のPOIまでの最短路で移動した時の時間を求める．
		double moving_time_between_poi_and_next_poi = map->calc_necessary_time((*now_poi)->get_id(), (*last_poi)->get_id(), user->get_now_speed(phase_id));
		int next_arrive_time = moving_time_between_poi_and_next_poi + user->get_pause_time();

		//停止時間をphaseに換算し，pause_timeと最短路経路からpathを決定していく
		double rest_pause_time = user->get_pause_time() - dest_rest_time;
		lldiv_t last_variable_of_converted_pause_time_to_phase = std::lldiv(rest_pause_time, requirement->service_interval);

		user->set_rest_pause_time_when_departing(last_variable_of_converted_pause_time_to_phase.rem);

		std::vector<Graph::MapNodeIndicator> last_shortests_path = map->get_shortest_path((*now_poi)->get_id(), (*last_poi)->get_id());

		//停止時間分，各phaseに停止場所と停止phaseを登録
		set_pause_time_and_phases_of_visited_POI_of_predicted_user(&phase_id, rest_pause_time, last_variable_of_converted_pause_time_to_phase.quot, now_poi);

		std::vector<Graph::MapNodeIndicator>::iterator last_path_iter = last_shortests_path.begin();//pathを検索するためのindex
		//速度はphaseで埋める前を参照しなければならないことに注意
		double last_pause_position_speed = user->get_starting_speed();

		//最初だけ停止時間をphaseに換算した時の余りをtimeとし，それ以外はservice_intervalをtimeとして，現在地から求めたい地点のdistanceを計算
		double distance = (requirement->service_interval - last_variable_of_converted_pause_time_to_phase.rem) * last_pause_position_speed;
		double distance_between_now_and_next_poi = map->shortest_distance((*now_poi)->get_id(), (*last_poi)->get_id());

		Graph::MapNodeIndicator last_nearest_position = (*now_poi)->get_id();

		//pathを作成．場所は一番近いintersection同士で線形補間する．MapNodeIndicatorのTypeはINVALIDとする．
		//last_phaseまで埋める！
		while (phase_id != (time_manager->phase_count() - 1)) {
			set_path_between_poi_of_predicted_user(now_poi, last_path_iter, last_nearest_position, last_pause_position_speed, requirement->service_interval, &distance, &phase_id);
		}
		*/
	}

	///<summary>
	/// 提案手法の核になる部分
	/// ユーザの交差を再設定する
		/*
		パターン１
		現在が移動中で，設定時も移動or 現在が移動中で，設定時はPOIに停止中
		現在→設定場所，設定場所でmin止まる→次のPOI

		パターン２
		現在がPOI停止中で，設定時は移動中or現在がPOI停止中で，設定時もPOI停止中
		現在いるPOIの最後の停止フェーズ→設定場所，設定場所でmin止まる→次のPOI

		パターン3
		現在がPOI停止中で，設定時も同じPOIに停止中
		設定無理

		------------以下は現在フェーズが寄与しないパターン→prevのprevが現在フェーズより大きい------------------------------

		パターン４
		設定フェーズが移動中or設定フェーズで停止中
		前回のPOI→設定場所，設定場所でmin止まる→次のPOI

		※いずれの場合も，次のPOIが最終フェーズになるときはnextでエラーになるので，注意！！

		設定時の停止の有無で判定？？
		*/
	///</summary>
	void HayashidaBachelorMethod::re_setting_of_user_cross(int now_phase)
	{
		//元々のユーザの交差回数を記録(参照するユーザはプラン変更前のinput_userであることに注意)
		//現在phaseまでの交差回数は考慮しないことに注意
		int original_cross_num = input_user->get_cross_count() - input_user->get_cross_count_until_the_phase(now_phase);
		
		//現在phase以降の全停止phaseを取得
		std::vector<int> all_pause_phases = predicted_user->get_all_pause_phases_since_current_phase(now_phase);
		
		//ユーザとユーザの全停止時間中に存在する全ダミーとの距離を計算
		//既に距離別にsort済み
		//pair(pair(dummy_id, phase), distance)
		std::vector<std::pair<std::pair<entity_id, int>, double>> candidate_cross_dummies_ordered_by_dist = calc_distance_between_user_and_dummies(all_pause_phases);

		//元々の交差予定回数分，設定を試みていく
		for (int i = 0; i < original_cross_num; i++) {
			for (auto iter = candidate_cross_dummies_ordered_by_dist.begin(); iter != candidate_cross_dummies_ordered_by_dist.end(); iter++) {
				//対象エンティティとフェーズを取得
				int target_phase = iter->first.second;
				entity_id target_dummy_id = iter->first.first;
				std::shared_ptr<Entity::RevisablePauseMobileEntity<Geography::LatLng>> target_dummy = entities->get_dummy_by_id(target_dummy_id);
				Graph::MapNodeIndicator target_pos = predicted_user->get_trajectory()->get_visited_node_id(target_phase);
				
				//スコープ確保のため，現在フェーズが起用しない場合についてはif文の外に出しておく
				int prev_phase = target_dummy->get_prev_poi_arrive_phase_when_pausing(target_phase);
				int next_phase = target_dummy->get_next_poi_arrive_phase_when_pausing(target_phase);
				Graph::MapNodeIndicator prev_pos = target_dummy->get_trajectory()->get_visited_node_id(prev_phase);
				Graph::MapNodeIndicator next_pos = target_dummy->get_trajectory()->get_visited_node_id(next_phase);
				Math::Probability generator;
				double speed_at_target_phase = generator.uniform_distribution(requirement->average_speed_of_dummy - 0.5 * requirement->speed_range_of_dummy, requirement->average_speed_of_dummy + 0.5 * requirement->speed_range_of_dummy);

				//現在フェーズが寄与しない場合
				if (target_dummy->get_prev_phase_when_visiting_poi(target_dummy->get_prev_phase_when_visiting_poi(target_phase) - 1) > now_phase){
					//対象フェーズで移動中？
					if (!target_dummy->isPause(target_phase)) {
						prev_phase = target_dummy->get_prev_phase_when_visiting_poi(target_phase);
						next_phase = target_dummy->get_next_phase_when_visiting_poi(target_phase);
					}
				}
				//現在フェーズが寄与する場合
				else {
					//現在フェーズが停止中
					if (target_dummy->isPause(now_phase)) {
						prev_phase = target_dummy->get_next_phase_when_visiting_poi(now_phase);
						//対象フェーズが停止しているか否かで場合分け
						next_phase =
							target_dummy->isPause(target_phase) ? target_dummy->get_next_poi_arrive_phase_when_pausing(target_phase) : target_dummy->get_next_phase_when_visiting_poi(target_phase);

						prev_pos = target_dummy->get_trajectory()->get_visited_node_id(prev_phase);
						next_pos = target_dummy->get_trajectory()->get_visited_node_id(next_phase);

						//現在位置と設定位置が同じ場合はそもそも交差は出来ない．
						if (prev_pos == target_pos) continue;
					}
					//現在フェーズで移動中
					else {
						prev_phase = now_phase;
						next_phase =
							target_dummy->isPause(target_phase) ? target_dummy->get_next_poi_arrive_phase_when_pausing(target_phase) : target_dummy->get_next_phase_when_visiting_poi(target_phase);

						//現在フェーズはOTHERSなはずなので，一番近い点を補完する
						prev_pos = *map->get_nearest_node_of_now_position(*target_dummy->get_trajectory()->get_positions()->at(now_phase));
						next_pos = target_dummy->get_trajectory()->get_visited_node_id(next_phase);
					}
					//time_limitはpreviousでの停止時間を考慮しなければならないことに注意
					double previous_time_limit = time_manager->time_of_phase(target_phase) - time_manager->time_of_phase(prev_phase);;
					double next_time_limit = time_manager->time_of_phase(next_phase) - time_manager->time_of_phase(target_phase) - requirement->min_pause_time;

					//共有場所に到達可能ならその位置を設定し，到達不能ならばもう一度別のフェーズを検討
					if (previous_time_limit < 0 || next_time_limit < 0) continue;
					if (!map->is_reachable(prev_pos, target_pos, target_dummy->get_now_speed(prev_phase+1), previous_time_limit)
						|| !map->is_reachable(target_pos, next_pos, speed_at_target_phase, next_time_limit)) {
						continue;
					}
				}
				//経路の再設定を行う．
				//交差情報のクリアと設定が必要だと思うけど，とりあえず今は必要ないから無視
				double dest_rest_time = 0;
				int phase_id = prev_phase;
				calc_dummy_path(target_dummy_id, prev_pos, target_pos, &phase_id, &dest_rest_time, target_dummy->get_now_speed(prev_phase + 1));
				set_now_pause_time_and_phase(target_dummy_id, prev_pos, target_pos, &phase_id, &dest_rest_time, next_phase, target_phase, speed_at_target_phase);
				calc_dummy_path(target_dummy_id, target_pos, next_pos, &phase_id, &dest_rest_time, speed_at_target_phase);
				break;
			}
		}
		
		std::cout << "Success Re-Setting Cross" << std::endl;
	}

	///<summary>
	/// ダミーの経路を再計算
	///</summary>
	void HayashidaBachelorMethod::calc_dummy_path(entity_id target_dummy_id, const Graph::MapNodeIndicator& source, const Graph::MapNodeIndicator& destination, int *phase_id, double *dest_rest_time, double speed)
	{
		std::shared_ptr<Entity::RevisablePauseMobileEntity<Geography::LatLng>> target_dummy = entities->get_dummy_by_id(target_dummy_id);
		
		std::vector<Graph::MapNodeIndicator> shortests_path_between_pois = map->get_shortest_path(source, destination);
		std::vector<Graph::MapNodeIndicator>::iterator path_iter = shortests_path_between_pois.begin();//pathを検索するためのindex
				
		//sourceからの距離
		//最初だけ停止時間をphaseに換算した時の余りをtimeとし，それ以外はservice_intervalをtimeとして，現在地から求めたい地点のdistanceを計算
		//速度はphaseで埋める前を参照しなければならないことに注意
		double now_time = requirement->service_interval - target_dummy->get_now_pause_time(*phase_id);
		double total_time_from_source_to_destination = map->calc_necessary_time(source, destination, speed);
		Graph::MapNodeIndicator nearest_position = source;
		//pathを作成．場所は一番近いintersection同士で線形補間する．MapNodeIndicatorのTypeはINVALIDとする．
		while (now_time < total_time_from_source_to_destination)
		{
			//最初は停止時間をphaseに換算したときの余り分をdistanceとして，最短路の中で一番近いintersectionを探し，線形補間する．
			//double total_path_length = map->shortest_distance(source, *path_iter);
			while (now_time > map->calc_necessary_time(source, *path_iter, speed))
			{
				nearest_position = *path_iter;
				path_iter++;
			}

			double distance = now_time * speed;
			double distance_between_nearest_intersection_and_arrive_position = distance - map->shortest_distance(source, nearest_position);
			Geography::LatLng nearest_latlng
				= nearest_position.type() == Graph::NodeType::POI ? map->get_static_poi(nearest_position.id())->data->get_position() : *map->get_static_node(nearest_position.id())->data;
			Geography::LatLng next_nearest_latlang
				= (*path_iter).type() == Graph::NodeType::POI ? map->get_static_poi((*path_iter).id())->data->get_position() : *map->get_static_node((*path_iter).id())->data;
			double angle = Geography::GeoCalculation::lambert_azimuth_angle(nearest_latlng, next_nearest_latlang);

			Geography::LatLng arrive_position = Geography::GeoCalculation::calc_translated_point(nearest_latlng, distance_between_nearest_intersection_and_arrive_position, angle);

			if (*phase_id == time_manager->phase_count() - 1) return;//残りのpathを決める時の終了条件
			(*phase_id)++;
			target_dummy->set_now_speed(*phase_id, speed);
			target_dummy->set_now_pause_time(*phase_id, 0.0);
			target_dummy->set_position_of_phase(*phase_id, Graph::MapNodeIndicator(Graph::NodeType::OTHERS, Graph::NodeType::OTHERS), arrive_position);
			
			now_time += requirement->service_interval;
		}

		//destinationのところまで補完できたら，rest_timeを保持しておく！
		double time_between_arrive_position_and_dest_position = now_time - total_time_from_source_to_destination;
		*dest_rest_time
			= time_between_arrive_position_and_dest_position == requirement->service_interval ? 0 : time_between_arrive_position_and_dest_position;
		//目的地の登録
		(*phase_id)++;
		target_dummy->set_position_of_phase(*phase_id, destination, map->get_static_poi(destination.id())->data->get_position());
		target_dummy->set_now_speed(*phase_id, 0.0);
	}

	///<summary>
	///
	///</summary>
	void HayashidaBachelorMethod::set_now_pause_time_and_phase(entity_id target_dummy_id, Graph::MapNodeIndicator& target_pos, Graph::MapNodeIndicator& next_pos, int *phase_id, double *dest_rest_time, int next_phase, int target_phase, double speed) {
		std::shared_ptr<Entity::RevisablePauseMobileEntity<Geography::LatLng>> target_dummy = entities->get_dummy_by_id(target_dummy_id);
		int time_between_target_phase_and_next_phase = time_manager->time_of_phase(next_phase) - time_manager->time_of_phase(target_phase);
		double pause_time_at_target_phase
			= time_between_target_phase_and_next_phase - map->calc_necessary_time(target_pos, next_pos, speed);

		if (pause_time_at_target_phase < 0) throw std::invalid_argument("Pause Time is negative number!!");

		double rest_pause_time = pause_time_at_target_phase - *dest_rest_time;
		lldiv_t variable_of_converted_pause_time_to_phase = std::lldiv(rest_pause_time, requirement->service_interval);

		int arrive_phase = *phase_id;
		target_dummy->set_now_pause_time(*phase_id, rest_pause_time);

		for (int i = 0; i < variable_of_converted_pause_time_to_phase.quot; i++)
		{
			if (*phase_id == time_manager->phase_count() - 1) return;
			(*phase_id)++;
			rest_pause_time -= requirement->service_interval;
			target_dummy->set_now_pause_time(*phase_id, rest_pause_time);
			target_dummy->set_now_speed(*phase_id, 0.0);
			target_dummy->set_position_of_phase(*phase_id, target_pos, map->get_static_poi(target_pos.id())->data->get_position());
		}
	}


	///<summary>
	/// ユーザとユーザの全停止時間中に存在する全ダミーとの距離を計算
	/// 距離の小さい順にソートして返す
	/// pair(pair(dummy_id, phase), distance)
	///</summary>
	std::vector<std::pair<std::pair<Method::KatoBachelorMethod::entity_id, int>, double>> HayashidaBachelorMethod::calc_distance_between_user_and_dummies(std::vector<int> all_pause_phases)
	{
		std::vector<std::pair<std::pair<entity_id, int>, double>> candidate_cross_dummies;

		//ユーザとユーザの全停止時間中に存在する全ダミーとの距離を計算
		for (std::vector<int>::iterator iter = all_pause_phases.begin(); iter != all_pause_phases.end(); iter++) {
			for (entity_id dummy_id = 0; dummy_id < requirement->dummy_num; dummy_id++) {
				//|user-dummy[dummy_id](*iter)|を計算するメソッド
				double dist = map->shortest_distance(predicted_user->read_node_pos_info_of_phase(*iter).first, entities->get_dummy_by_id(dummy_id)->read_node_pos_info_of_phase(*iter).first);
				candidate_cross_dummies.push_back(std::make_pair(std::make_pair(dummy_id, *iter), dist));
			}
		}
		//修正距離の小さい順にソート
		//ただし，同じ修正距離の場合(POIに停止中)，phaseが早い方を優先
		std::sort(candidate_cross_dummies.begin(), candidate_cross_dummies.end(),
			[](std::pair<std::pair<entity_id, int>, double>& candidate1, std::pair<std::pair<entity_id, int>, double>& candidate2) {
			if (candidate1.second == candidate2.second) return candidate1.first.second < candidate2.first.second;
			return candidate1.second < candidate2.second;
		});

		return candidate_cross_dummies;
	}

	///<summary>
	///　ユーザの訪問POI順序を再予測する．
	///</summary>
	std::vector<Graph::MapNodeIndicator> HayashidaBachelorMethod::repredict_visited_poi_order_by_tsp(std::vector<Graph::MapNodeIndicator>& input_poi_list)
	{
		std::vector<std::pair<std::vector<Graph::MapNodeIndicator>, double>> all_tsp_solution;
		Graph::MapNodeIndicator start_poi = input_poi_list.front();//最初の訪問POI

		std::vector<int> visited_pois_except_start_poi;//最初の訪問POIを除いたPOI_list
		for (std::vector<Graph::MapNodeIndicator>::iterator iter = input_poi_list.begin() + 1; iter != input_poi_list.end(); iter++) {
			visited_pois_except_start_poi.push_back((*iter).id());
		}

		//id順にソート
		std::sort(visited_pois_except_start_poi.begin(), visited_pois_except_start_poi.end(), [](int& poi1, int& poi2) {
			return poi1 < poi2;
		});

		do {
			double distance = map->shortest_distance(start_poi.id(), visited_pois_except_start_poi.front());
			std::vector<int>::iterator iter = visited_pois_except_start_poi.begin();
			for (int i = 0; i < visited_pois_except_start_poi.size() - 1; i++, iter++) {
				distance += map->shortest_distance(*iter, *(iter + 1));
			}
			std::vector<Graph::MapNodeIndicator> visited_pois_except_start_poi_convert_to_mapnodeindicator;
			for (auto iter = visited_pois_except_start_poi.begin(); iter != visited_pois_except_start_poi.end(); iter++) {
				visited_pois_except_start_poi_convert_to_mapnodeindicator.push_back(Graph::MapNodeIndicator(*iter, Graph::POI));
			}
			all_tsp_solution.push_back(std::make_pair(visited_pois_except_start_poi_convert_to_mapnodeindicator, distance));
			all_tsp_solution.back().first.insert(all_tsp_solution.back().first.begin(), start_poi);
		} while (std::next_permutation(visited_pois_except_start_poi.begin(), visited_pois_except_start_poi.end()));

		std::sort(all_tsp_solution.begin(), all_tsp_solution.end(), [](std::pair<std::vector<Graph::MapNodeIndicator>, double>& poi1, std::pair<std::vector<Graph::MapNodeIndicator>, double>& poi2) {
			return poi1.second < poi2.second;
		});

		return all_tsp_solution.front().first;
	}

	///<summary>
	/// 最短路の中で一番近いintersectionを探し，線形補間する． 
	/// now_poiには速度と停止時間を設定してあること
	/// 最後は目的地の直前の場所を補完した場所が登録されている．
	///</summary>
	void HayashidaBachelorMethod::set_predicted_user_path(std::vector<Graph::MapNodeIndicator>::iterator& now_poi, std::vector<Graph::MapNodeIndicator>::iterator& path_iter, Graph::MapNodeIndicator& nearest_position, double pause_position_speed, double service_interval, double *distance, int *phase_id)
	{
		while (*distance > map->shortest_distance((*now_poi).id(), *path_iter))
		{
			nearest_position = *path_iter;
			path_iter++;
		}

		double distance_between_start_and_nearest_position = map->shortest_distance((*now_poi).id(), nearest_position);
		double distance_between_nearest_intersection_and_arrive_position = *distance - distance_between_start_and_nearest_position;

		Geography::LatLng nearest_latlng
			= nearest_position.type() == Graph::NodeType::POI ? map->get_static_poi(nearest_position.id())->data->get_position() : *map->get_static_node(nearest_position.id())->data;
		Geography::LatLng next_nearest_latlang
			= (*path_iter).type() == Graph::NodeType::POI ? map->get_static_poi((*path_iter).id())->data->get_position() : *map->get_static_node((*path_iter).id())->data;
		double angle = Geography::GeoCalculation::lambert_azimuth_angle(nearest_latlng, next_nearest_latlang);

		Geography::LatLng arrive_position = Geography::GeoCalculation::calc_translated_point(nearest_latlng, distance_between_nearest_intersection_and_arrive_position, angle);

		if (*phase_id == time_manager->phase_count() - 1) return;
		(*phase_id)++;
		predicted_user->set_now_speed(*phase_id, pause_position_speed);
		predicted_user->set_position_of_phase(*phase_id, Graph::MapNodeIndicator(Graph::NodeType::OTHERS, Graph::NodeType::OTHERS), arrive_position);

		*distance += service_interval * pause_position_speed;
	}


	///<summary>
	/// 停止時間とphaseのセット
	///</summary>
	void HayashidaBachelorMethod::set_predicted_user_pause_time_and_phases(int *phase_id, double rest_pause_time, int total_pause_phase, Graph::MapNodeIndicator now_poi){
		predicted_user->set_now_pause_time(*phase_id, rest_pause_time);

		for (int i = 0; i < total_pause_phase; i++)
		{
			if (*phase_id == time_manager->phase_count() - 1) break;
			(*phase_id)++;
			predicted_user->set_pause_phases(*phase_id);
			rest_pause_time -= requirement->service_interval;
			predicted_user->set_now_pause_time(*phase_id, rest_pause_time);
			predicted_user->set_position_of_phase(*phase_id, now_poi.id(), map->get_static_poi(now_poi.id())->data->get_position());
		}
	}

	/*
	///<summary>
	/// 初期化 (今回は特にやることはない)
	///</summary>
	void HayashidaBachelorMethod::initialize()
	{
		//ユーザの動きの変更→新しく作る．
	}
	*/

	///<summary>
	/// ここが提案手法の核になる部分.ダミーの行動を修正する
	///</summary>
	void HayashidaBachelorMethod::adjust_dummy_positions()
	{
		clear_visited_pois_info_list_id_of_users();//usersのvisited_pois_info_list_idのクリア
		for (int phase_id = 0; phase_id < time_manager->phase_count(); phase_id++) {
			for (size_t dummy_id = 1; dummy_id <= entities->get_dummy_count(); dummy_id++) {
				revising_dummy = entities->get_dummy_by_id(dummy_id);
				//ダミーのvisited_pois_info_list_idの更新
				if ((phase_id - 1) == revising_dummy->get_pause_phases().back()) revising_dummy->increment_visited_pois_info_list_id();
				//ユーザのvisited_pois_info_list_idの更新
				increment_visited_pois_info_list_id_of_users(phase_id);

				if (!is_user_moving_to_POI()) {
					repredicting_user_trajectory(phase_id);
					re_setting_of_user_cross(phase_id);
				}
				revise_dummy_positions(phase_id, dummy_id);
			}
		}
	}

	/*
	///<summary>
	/// 決定した位置を基にMTC等各種評価値を算出する
	///</summary>
	void HayashidaBachelorMethod::evaluate()
	{

	}


	///<summary>
	/// 結果のファイルへのエクスポート
	///</summary>
	void HayashidaBachelorMethod::export_results()
	{

	}


	///<summary>
	/// 終了処理 (今回はスマートポインタを利用しているので，特にやることはない)
	///</summary>
	void HayashidaBachelorMethod::terminate()
	{

	}
	*/

	void HayashidaBachelorMethod::run()
	{
		//ここで実行時間の計測を開始
		timer->start();

		//初期化
		Method::KatoMasterMethod::initialize();

		//ここが実行部分(加藤さん卒論手法[Kato 13])
		Method::KatoBachelorMethod::decide_dummy_positions();

		//ここでユーザの行動の予測やダミーの行動を修正する(林田さん卒論手法[Hayashida 14])
		adjust_dummy_positions();

		//ここで計測を終了
		timer->end();

		//終了処理
		terminate();
	}


}
