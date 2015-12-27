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
	bool HayashidaBachelorMethod::check_user_going_to_sheduled_POI()
	{
		return true;
	}

	///<summary>
	/// ユーザの行動を再予測する
	///</summary>
	void HayashidaBachelorMethod::re_predicted_user_trajectory()
	{
		//---------------------------POI訪問順序を決定---------------------------------------------
		int phase_id = 0;
		/*
		//再度ユーザの訪問POI順序をT.S.P.を用いて予測
		std::pair<std::vector<std::shared_ptr<Map::BasicPoi const>>, double> order_visited_poi = re_traveling_salesman_problem(input_poi_list);

		//最初の点を登録
		std::vector<std::shared_ptr<Map::BasicPoi const>>::iterator now_poi = order_visited_poi.first.begin();
		std::vector<std::shared_ptr<Map::BasicPoi const>>::iterator next_poi = order_visited_poi.first.begin() + 1;

		user->set_visited_poi_of_phase(phase_id, Graph::MapNodeIndicator((*now_poi)->get_id()), (*now_poi)->data->get_position());
		user->set_random_starting_speed_at_poi(AVERAGE_SPEED, RANGE_OF_SPEED);

		double dest_rest_time = 0.0;//phaseの到着時間と実際の到着時間の差分.最初だけ0

									//--------------二個目以降の点を決定．forで，確保したいpoiの個数分をループさせる-------------------
		for (int i = 1; i < POI_NUM; i++)
		{
			user->set_dest_rest_time(dest_rest_time);//到着時の余り分を登録
			user->set_random_pause_time(phase_id, MIN_PAUSE_TIME, MAX_PAUSE_TIME);

			double moving_time_between_poi_and_next_poi = map->calc_necessary_time((*now_poi)->get_id(), (*next_poi)->get_id(), user->get_now_speed(phase_id));
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
			while (distance < distance_between_now_and_next_poi && phase_id != time_manager->phase_count() - 1) {
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

			now_poi++;
			next_poi++;
		}

		//---------------------------------end_timeまで適当に経路を決める！---------------------------------------------------

		Math::Probability generator;
		//最終地点は少し遠くにとる(1.5倍～2倍)．ただし，マップの限界範囲に注意
		double last_distance = 1.4 * (end_time - time_manager->time_of_phase(phase_id)) * user->get_now_speed(phase_id);

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
		user->set_random_pause_time(phase_id, MIN_PAUSE_TIME, MAX_PAUSE_TIME);
		user->set_dest_rest_time(dest_rest_time);


		//現在地の停止時間をランダムで設定し，現地点の出発地の速度で，次のPOIまでの最短路で移動した時の時間を求める．
		double moving_time_between_poi_and_next_poi = map->calc_necessary_time((*now_poi)->get_id(), (*last_poi)->get_id(), user->get_now_speed(phase_id));
		int next_arrive_time = moving_time_between_poi_and_next_poi + user->get_pause_time();

		//停止時間をphaseに換算し，pause_timeと最短路経路からpathを決定していく
		double rest_pause_time = user->get_pause_time() - dest_rest_time;
		lldiv_t last_variable_of_converted_pause_time_to_phase = std::lldiv(rest_pause_time, SERVICE_INTERVAL);

		user->set_rest_pause_time_when_departing(last_variable_of_converted_pause_time_to_phase.rem);

		std::vector<Graph::MapNodeIndicator> last_shortests_path = map->get_shortest_path((*now_poi)->get_id(), (*last_poi)->get_id());

		//停止時間分，各phaseに停止場所と停止phaseを登録
		set_pause_time_and_phases_of_visited_POI(&phase_id, rest_pause_time, last_variable_of_converted_pause_time_to_phase.quot, now_poi);

		std::vector<Graph::MapNodeIndicator>::iterator last_path_iter = last_shortests_path.begin();//pathを検索するためのindex
		//速度はphaseで埋める前を参照しなければならないことに注意
		double last_pause_position_speed = user->get_starting_speed();

		//最初だけ停止時間をphaseに換算した時の余りをtimeとし，それ以外はservice_intervalをtimeとして，現在地から求めたい地点のdistanceを計算
		double distance = (SERVICE_INTERVAL - last_variable_of_converted_pause_time_to_phase.rem) * last_pause_position_speed;
		double distance_between_now_and_next_poi = map->shortest_distance((*now_poi)->get_id(), (*last_poi)->get_id());

		Graph::MapNodeIndicator last_nearest_position = (*now_poi)->get_id();

		//pathを作成．場所は一番近いintersection同士で線形補間する．MapNodeIndicatorのTypeはINVALIDとする．
		//last_phaseまで埋める！
		while (phase_id != (time_manager->phase_count() - 1)) {
			set_path_between_poi(now_poi, last_path_iter, last_nearest_position, last_pause_position_speed, SERVICE_INTERVAL, &distance, &phase_id);
		}
		*/
		std::cout << "Success Making Re-Predicted User" << std::endl;
	}

	///<summary>
	/// 提案手法の核になる部分
	/// ユーザの交差を再設定する
	///</summary>
	void HayashidaBachelorMethod::re_setting_of_user_cross(int now_phase)
	{
		//元々のユーザの交差回数を記録(参照するユーザはプラン変更前のinput_userであることに注意)
		//現在phaseまでの交差回数は考慮しないことに注意
		int original_cross_num = input_user->get_cross_count() - input_user->get_cross_count_until_the_phase(now_phase);
		
		//全停止phaseを取得
		std::vector<int> all_pause_phases = predicted_user->get_all_pause_phases();
		
		//ユーザとユーザの全停止時間中に存在する全ダミーとの距離を計算
		//既に距離別にsort済み
		//pair(pair(dummy_id, phase), distance)
		std::vector<std::pair<std::pair<entity_id, int>, double>> candidate_cross_dummies_ordered_by_dist = distance_between_user_and_dummies_at_pause_phases(all_pause_phases);

		//元々の交差予定回数分，設定を試みていく
		for (int i = 0; i < original_cross_num; i++) {
			for (auto iter = candidate_cross_dummies_ordered_by_dist.begin(); iter != candidate_cross_dummies_ordered_by_dist.end(); iter++) {
				std::shared_ptr<Entity::RevisablePauseMobileEntity<Geography::LatLng>> target_dummy = entities->get_dummy_by_id(iter->first.first);
				//対象ダミーがPOIにPOIに停止中の場合
				if (target_dummy->read_node_pos_info_of_phase(iter->first.second).first.type() == Graph::POI)
				{
					//追跡可能性低下のための経路設定と同様な方法で，三通りに場合分け
				}
				//対象ダミーが対象phase中に訪問POI以外に存在している場合
				else
				{
					//到達可能性がどの程度あるかどうかによるが，とりあえずシンプルに３つに場合分け
				}
			}
		}

	}


	///<summary>
	/// ユーザとユーザの全停止時間中に存在する全ダミーとの距離を計算
	/// 距離の小さい順にソートして返す
	/// pair(pair(dummy_id, phase), distance)
	///</summary>
	std::vector<std::pair<std::pair<Method::KatoBachelorMethod::entity_id, int>, double>> HayashidaBachelorMethod::distance_between_user_and_dummies_at_pause_phases(std::vector<int> all_pause_phases)
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
	void HayashidaBachelorMethod::re_traveling_salesman_problem()
	{

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

				if (!check_user_going_to_sheduled_POI()) {
					re_predicted_user_trajectory();
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
