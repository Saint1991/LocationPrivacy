#include "stdafx.h"
#include "KatoMasterMethod.h"

namespace Method
{

	///<summary>
	/// コンストラクタ
	/// これにSimulatorで作成した各種入力への参照を渡す
	///</summary>
	KatoMasterMethod::KatoMasterMethod(std::shared_ptr<Map::HayashidaDbMap const> map, std::shared_ptr<Entity::DifferentMovementUser<Geography::LatLng>> user, std::shared_ptr<Requirement::KatoMethodRequirement const> requirement, std::shared_ptr<Time::TimeSlotManager> time_manager)
		: KatoBachelorMethod(map, user, requirement, time_manager),Tu(0.0)
	{
		//input_userとreal_userの生成．この2つは変更しない
		input_user = entities->get_user()->get_predicted_user();
		real_user = entities->get_user()->get_real_user();

		//predicted_userを変更させていく
		predicted_user = entities->get_user()->get_predicted_user();
	}

	///<summary>
	/// デストラクタ
	///</summary>
	KatoMasterMethod::~KatoMasterMethod()
	{
	}
	
	///<summary>
	/// input_user_planからユーザの行動を予測し，そのユーザを返す．
	/// 加藤さん手法の場合は，各要素の生成確率によって割合を決める．
	/// その割合に従って，実際のユーザを作成する
	///</summary>
	std::shared_ptr<Entity::RevisablePauseMobileEntity<Geography::LatLng>> KatoMasterMethod::copy_predicted_user_plan(std::shared_ptr<Entity::PauseMobileEntity<Geography::LatLng>> input_user)
	{
	
		return predicted_user;
	}


	///<summary>
	/// 速度の最大変化値と最小変化値を計算する
	/// pair(Max, min)
	///</summary>
	std::pair<double, double> KatoMasterMethod::calc_max_variable_speed(double speed)
	{
		double max_speed = requirement->average_speed_of_dummy + requirement->speed_range_of_dummy;
		double min_speed = requirement->average_speed_of_dummy - requirement->speed_range_of_dummy;

		return std::make_pair(MAX_VARIATION_OF_SPEED*(max_speed - speed), MAX_VARIATION_OF_SPEED*(speed - min_speed));
	}

	///<summary>
	/// 停止時間の最大変化値と最小変化値を計算する
	/// pair(Max, min)
	///</summary>
	std::pair<double, double> KatoMasterMethod::calc_max_variable_pause_time(double pause_time)
	{
		return std::make_pair(MAX_VARIATION_OF_PAUSE_TIME*(requirement->max_pause_time - pause_time), MAX_VARIATION_OF_PAUSE_TIME*(pause_time - requirement->min_pause_time));
	}


		
	///<summary>
	/// ユーザの行動プランに含まれる停止地点に向かっているかどうかをチェック
	///</summary>
	bool KatoMasterMethod::check_going_same_poi_as_plan()
	{
		return true;
	}

	///<summary>
	/// ユーザの行動プラン変更の判断
	/// real_userとpredict_userの違いで見る
	/// ユーザがプラン通りに行動している場合はNO_CHANGEをリターン
	///</summary>
	KatoMasterMethod::ChangeParameter KatoMasterMethod::check_user_plan(int now_phase)
	{
		//止まっている→停止時間の変更or停止地点の変更
		//もし片方でも停止していたら→停止時間変更の判断
		if (predicted_user->isPause(now_phase) || real_user->isPause(now_phase)) {
			return check_user_pause_time(now_phase);
		}
		//動いていたら，
		else {
			//もし線形補間した線の上に位置していたら
			if (is_on_the_path(now_phase)) {
				return check_user_speed(now_phase);
			}
			else {
				return check_user_path(now_phase);
			}
		}
	}
	
		///<summary>
		/// ユーザの行動プラン変更の判断
		/// real_userとpredicted_userの違いで見る
		/// pause_timeのチェック
		///</summary>
		KatoMasterMethod::ChangeParameter KatoMasterMethod::check_user_pause_time(int now_phase)
		{
			//もしプラン通り停止していたら，NO_CHANGE
			if (predicted_user->isPause(now_phase) && real_user->isPause(now_phase)) {
				return NO_CHANGE;
			}
			else if (predicted_user->isPause(now_phase) || real_user->isPause(now_phase)) {
				//もしrealのほうが停止フラグが0で(出発していて)，predictが1なら,予定より早く出発
				if (predicted_user->isPause(now_phase) == true && real_user->isPause(now_phase) == false) {
					//change_timeの差分を求める
					Tu -= requirement->service_interval;
					return SHORTER_PAUSE_TIME;
				}
				else {
					//change_timeの差分を求める
					Tu += requirement->service_interval;
					return LONGER_PAUSE_TIME;
				}
			}
			//両方フラグが0なら(速度変更からジャンプして飛んできた場合)，速度変更でなく，rest_time分をずれを検知
			else{
				throw std::invalid_argument("This situation can't happen!!");
				if (now_phase == 0) throw std::invalid_argument(" It must set pause time at init visited POI !! ");
				double real_user_dist = Geography::GeoCalculation::lambert_distance(*real_user->read_position_of_phase(now_phase - 1), *real_user->read_position_of_phase(now_phase));
				double predicted_user_dist = Geography::GeoCalculation::lambert_distance(*predicted_user->read_position_of_phase(now_phase - 1), *predicted_user->read_position_of_phase(now_phase));
				//移動距離がrealのほうが大きいなら
				if (real_user_dist > predicted_user_dist) {
					//change_timeの差分を求める
					Tu -= (real_user_dist - predicted_user_dist) / predicted_user->get_starting_speed();
					return SHORTER_PAUSE_TIME;
				}
				else {
					//change_timeの差分を求める
					Tu -= (predicted_user_dist - real_user_dist) / predicted_user->get_starting_speed();
					return LONGER_PAUSE_TIME;
				}
			}
		}
	

		///<summary>
		/// ユーザの行動プラン変更の判断
		/// real_userとpredicted_userの違いで見る
		/// 速度のチェック
		///</summary>
		KatoMasterMethod::ChangeParameter KatoMasterMethod::check_user_speed(int now_phase)
		{
			Geography::LatLng previous_real_user_position = *real_user->read_node_pos_info_of_phase(now_phase - 1).second;
			Geography::LatLng previous_predicted_user_position = *predicted_user->read_node_pos_info_of_phase(now_phase - 1).second;
			Geography::LatLng now_real_user_position = *real_user->read_node_pos_info_of_phase(now_phase).second;
			Geography::LatLng now_predicted_user_position = *predicted_user->read_node_pos_info_of_phase(now_phase).second;

			//そもそも，同じ場所にいたら，NO_CHANGE
			if (now_predicted_user_position == now_real_user_position) return NO_CHANGE;

			//実際には差分で計算する必要があるが，実装上は値を参照しても同義
			double real_user_speed = real_user->get_now_speed(now_phase);
			double predicted_user_speed = predicted_user->get_now_speed(now_phase);

			if (real_user_speed == predicted_user_speed) {
				return check_user_pause_time(now_phase);
			}
			else {
				//realの方が速度が大きい場合
				if (real_user_speed > predicted_user_speed) {
					double change_time =
						map->calc_necessary_time(real_user->read_node_pos_info_of_phase(now_phase - 1).first, real_user->read_node_pos_info_of_phase(real_user->get_next_poi_arrive_phase_when_moving(now_phase)).first, real_user_speed)
						- map->calc_necessary_time(predicted_user->read_node_pos_info_of_phase(now_phase - 1).first, predicted_user->read_node_pos_info_of_phase(predicted_user->get_next_poi_arrive_phase_when_moving(now_phase)).first, predicted_user_speed);
					Tu += change_time;
					return FASTER_SPEED;
				}
				//predictedの方が速度が大きい場合
				else {
					double change_time =
						map->calc_necessary_time(predicted_user->read_node_pos_info_of_phase(now_phase - 1).first, predicted_user->read_node_pos_info_of_phase(predicted_user->get_next_poi_arrive_phase_when_moving(now_phase)).first, predicted_user_speed)
						- map->calc_necessary_time(real_user->read_node_pos_info_of_phase(now_phase - 1).first, real_user->read_node_pos_info_of_phase(real_user->get_next_poi_arrive_phase_when_moving(now_phase)).first, real_user_speed);
						Tu += change_time;
					return SLOER_SPEED;
				}
			}
		}
		

		///<summary>
		/// ユーザの行動プラン変更の判断
		/// real_userとpredicted_userの違いで見る
		/// pathのチェック
		///</summary>
		KatoMasterMethod::ChangeParameter KatoMasterMethod::check_user_path(int now_phase)
		{
			//change_timeを求める
			//Tu = 
			return PATH;
		}


		///<summary>
		/// ユーザの行動プラン変更の判断
		/// real_userとpredicted_userの違いで見る
		/// 停止位置のチェック
		///</summary>
		KatoMasterMethod::ChangeParameter KatoMasterMethod::check_user_position(int now_phase)
		{
			return VISIT_POI;
		}

		///<summary>
		/// ダミーがパス上に存在するかどうかをチェック
		/// predicted_userの現停止POIから次の停止POIまでの経路を取ってきて，その直線上に乗っているかどうかをチェック
		///</summary>
		bool KatoMasterMethod::is_on_the_path(int phase_id)
		{
			Graph::MapNodeIndicator source;
			Graph::MapNodeIndicator destination;

			std::shared_ptr<std::vector<std::shared_ptr<Geography::LatLng>>> path_between_pois = predicted_user->get_trajectory()->get_positions();
			std::vector<std::shared_ptr<Geography::LatLng>>::iterator path_iter = path_between_pois->begin();//pathを検索するためのindex
						
			Graph::MapNodeIndicator nearest_position = source;

			return true;
		}



	///<summary>
	/// ユーザの次の停止地点の到着時間を予測する
	///</summary>
	void KatoMasterMethod::update_user_plan(ChangeParameter check_parameter, int phase_id)
	{
		switch (check_parameter) {
		case LONGER_PAUSE_TIME:
			revise_user_trajectory_when_LONGER_PAUSE_TIME(phase_id);
			break;
		case SHORTER_PAUSE_TIME:
			revise_user_trajectory_when_SHORTER_PAUSE_TIME(phase_id);
			break;

		case FASTER_SPEED:
			revise_user_trajectory_when_FASTER_SPEED(phase_id);
			break;
		case SLOER_SPEED:
			revise_user_trajectory_when_SLOER_SPEED(phase_id);
			break;

		case PATH:
			revise_user_trajectory_when_PATH(phase_id);
			break;
		case VISIT_POI:
			revise_user_trajectory_when_VISIT_POI(phase_id);
			break;
		}
	}

	///<summary>
	/// ユーザの停止時間が予定より長い時のpredicted_userの修正
	///</summary>
	void KatoMasterMethod::revise_user_trajectory_when_LONGER_PAUSE_TIME(int phase_id)
	{
		//trajectoryとspeed,pause_timeをそれぞれずらすことで対応
		predicted_user->get_trajectory()->insert_positions_to_trajectory(phase_id, 1);
		predicted_user->insert_speed_list_and_pause_time(phase_id, 1);		
	}


	///<summary>
	/// ユーザの停止時間が予定より短い時のpredicted_userの修正
	///</summary>
	void KatoMasterMethod::revise_user_trajectory_when_SHORTER_PAUSE_TIME(int phase_id)
	{
		//trajectoryとspeed,pause_timeをずらすことで対応
		predicted_user->get_trajectory()->delete_positions_to_trajectory(phase_id);
		predicted_user->delete_speed_list_and_pause_time(phase_id);
	}

	///<summary>
	/// ユーザのパスの修正
	///</summary>
	void KatoMasterMethod::revise_user_trajectory_when_PATH(int phase_id)
	{

	}

	///<summary>
	/// ユーザの移動速度が予定より早い時の修正
	///</summary>
	void KatoMasterMethod::revise_user_trajectory_when_FASTER_SPEED(int phase_id)
	{
		double real_user_speed = real_user->get_now_speed(phase_id);

	}

	///<summary>
	/// ユーザの移動速度が予定より遅い時の修正
	///</summary>
	void KatoMasterMethod::revise_user_trajectory_when_SLOER_SPEED(int phase_id)
	{
		revise_user_trajectory_when_FASTER_SPEED(phase_id);
	}


	///<summary>
	/// ユーザの停止位置の修正
	///</summary>
	void KatoMasterMethod::revise_user_trajectory_when_VISIT_POI(int phase_id)
	{

	}


	///<summary>
	/// ダミーの行動プランを修正する
	///</summary>
	void KatoMasterMethod::revise_dummy_trajectory(int phase_id)
	{
		revise_dummy_pause_time(phase_id);
		if (Tu != 0.0) revise_dummy_path(phase_id);
		if (Tu != 0.0) revise_dummy_speed(phase_id);
		if (Tu != 0.0) revise_dummy_visit_poi(phase_id);

		/*		
		//加藤さんの修論手法のアルゴリズムでは，
		//poiに停止している時と，現在移動中の時は別にして考えているけど，
		//visited_pois_info_list_idを取得する時に，その2つは区別しているので，
		//場合分けする必要はない(はず)
		//全ての停止地点の到着時間を変更し，Tu分変更させる．
		//visited_poi情報の更新
		update_visited_pois_info_of_dummy();
		if (revising_dummy->isPause(phase_id)) {
			revise_dummy_pause_time(phase_id);
			if (Tu != 0.0) revise_dummy_path(phase_id);
			if (Tu != 0.0) revise_dummy_speed(phase_id);
			if (Tu != 0.0) revise_dummy_visit_poi(phase_id);
		}
		else {
			//全ての停止地点の到着時間を変更し，Tu分変更させる．
			revise_dummy_pause_time(phase_id);
			if (Tu != 0.0)revise_dummy_path(phase_id);
			if (Tu != 0.0) revise_dummy_speed(phase_id);
			if (Tu != 0.0) revise_dummy_visit_poi(phase_id);
		}
		*/
		
	}


	///<summary>
	/// ダミーの停止時間の修正
	/// 許容範囲まで修正を試みる
	/// もし調整しきれなかったら，次のpathの調整をする
	///</summary>
	void KatoMasterMethod::revise_dummy_pause_time(int phase_id)
	{
		int changed_poi_num_id = 0;//停止時間が変更されたpoiの数を記録するためのid
		
		//変更前のtrajectoryとスピードリストと停止時間リストを保持
		std::shared_ptr<Graph::RevisableTrajectory<>> prev_trajectory = revising_dummy->get_trajectory();
		std::vector<double> prev_now_speed_list = revising_dummy->get_now_speed_list();
		std::vector<double> prev_now_pause_time_list = revising_dummy->get_now_pause_time_list();

		//修正対象のフェーズを取得．現在停止中→現在のフェーズ，移動中→次の予定到着フェーズ
		int revise_phase = revising_dummy->isPause(phase_id) ? phase_id : revising_dummy->get_next_poi_arrive_phase_when_moving(phase_id);
		int next_poi_arrive_phase = revising_dummy->get_next_poi_arrive_phase_when_moving((revising_dummy->get_poi_last_pause_phase_when_pausing(revise_phase) + 1));

		//プログラムを組みやすいように，予め，出発速度と停止時間を抽出しておく
		std::vector<double> speed_list;
		int phase = revise_phase;
		phase = revising_dummy->get_poi_last_pause_phase_when_pausing(phase);
		while(phase != time_manager->last_phase()) {
			phase++;
			speed_list.push_back(revising_dummy->get_now_speed(phase));
			phase = revising_dummy->get_next_poi_arrive_phase_when_moving(phase);
			phase = revising_dummy->get_poi_last_pause_phase_when_pausing(phase);
		}

		std::vector<double> pause_time_list;
		int phase2 = time_manager->last_phase();
		phase2 = revising_dummy->get_poi_init_pause_phase_when_pausing(phase2);
		while (phase2 <= revise_phase) {
			phase2--;
			pause_time_list.push_back(revising_dummy->get_now_pause_time(phase2));
			phase2 = revising_dummy->get_prev_poi_depart_phase_when_moving(phase2);
			phase2 = revising_dummy->get_poi_init_pause_phase_when_pausing(phase2);
		}


		//全停止時間分，ダミーの停止時間を修正する．
		for (int id = revise_phase; id < time_manager->last_phase(); changed_poi_num_id++)
		{
			int changed_poi_num = 0;

			//停止中or向かっているPOIの停止時間を取得
			//現状，dest_timeを考慮していないから，実際の停止時間より若干短くなっている．
			double pause_time 
				= revising_dummy->isPause(phase_id) 
				? revising_dummy->get_now_pause_time(revising_dummy->get_poi_init_pause_phase_when_pausing(phase_id))
				: revising_dummy->get_now_pause_time(revising_dummy->get_next_poi_arrive_phase_when_moving(phase_id));
			
			//最大変化量
			double max_variable_value = calc_max_variable_pause_time(pause_time).first;
			double min_variable_value = calc_max_variable_pause_time(pause_time).second;

			//変更時間．初期値は，変更分Tu
			double change_time = Tu;

			//修正幅 ＜ 最大変化量　を満たし，かつ，修正幅 < 最大停止時間とする．
			if (Tu > 0) {
				if (Tu > max_variable_value) change_time = max_variable_value;
				if (max_variable_value + pause_time > requirement->max_pause_time) change_time = requirement->max_pause_time - pause_time;
			}
			else {
				if (std::abs(Tu) > min_variable_value) change_time = - min_variable_value;
				if (pause_time - min_variable_value < requirement->min_pause_time) change_time = requirement->min_pause_time - pause_time;
			}

			double new_pause_time = change_time + pause_time;
			
			//change_timeを現在の残り停止時間に記録
			revising_dummy->add_now_pause_time(revise_phase, change_time);
			
			recalculation_path(revising_dummy->get_trajectory()->get_visited_node_id(revise_phase), revising_dummy->get_trajectory()->get_visited_node_id(next_poi_arrive_phase), &revise_phase, speed_list.at(changed_poi_num), pause_time_list.at(changed_poi_num));

			Tu -= change_time;
			if (Tu == 0.0) break;
		}
				
		//変更されなかった分はprev_trajectoryをcopyさせる	
		//同じ位置をコピーするために，その位置を示すフェーズを取得
		int same_phase = prev_trajectory->search_phase_same_id(revising_dummy->get_trajectory()->get_visited_node_id(revise_phase));
		int last_copied_phase = same_phase + revising_dummy->get_trajectory()->last_phase() - revise_phase;
		int laset_phase = prev_trajectory->get_positions()->size();
		//revising_dummy->get_trajectory()->copy_trajectory(revise_phase, laset_phase, same_phase, laset_phase, *prev_trajectory);
		
		//now_speed_listとnow_pause_listのコピーも行う
		std::copy(prev_now_speed_list.begin() + same_phase, prev_now_speed_list.begin() + last_copied_phase, revising_dummy->get_now_speed_list().begin() + revise_phase);
		std::copy(prev_now_pause_time_list.begin() + same_phase, prev_now_pause_time_list.begin() + last_copied_phase, revising_dummy->get_now_pause_time_list().begin() + revise_phase);

	}
	

	///<summary>
	/// ダミーの移動経路の修正
	///</summary>
	void KatoMasterMethod::revise_dummy_path(int phase_id)
	{
		
	}


	///<summary>
	/// ダミーの行動速度の修正
	///</summary>
	void KatoMasterMethod::revise_dummy_speed(int phase_id)
	{
		int changed_poi_num_id = 0;

		//変更前のtrajectoryとスピードリストと停止時間リストを保持
		std::shared_ptr<Graph::RevisableTrajectory<>> prev_trajectory = revising_dummy->get_trajectory();
		std::vector<double> prev_now_speed_list = revising_dummy->get_now_speed_list();
		std::vector<double> prev_now_pause_time_list = revising_dummy->get_now_pause_time_list();

		//修正対象のフェーズを取得．現在停止中→現在のフェーズ，移動中→次の予定到着フェーズ
		int revise_phase = revising_dummy->isPause(phase_id) ? phase_id : revising_dummy->get_next_poi_arrive_phase_when_moving(phase_id);
		int next_poi_arrive_phase = revising_dummy->get_next_poi_arrive_phase_when_moving((revising_dummy->get_poi_last_pause_phase_when_pausing(revise_phase) + 1));

		//プログラムを組みやすいように，予め，出発速度と停止時間を抽出しておく
		std::vector<double> speed_list;
		int phase = revise_phase;
		phase = revising_dummy->get_poi_last_pause_phase_when_pausing(phase);
		while (phase != time_manager->last_phase()) {
			phase++;
			speed_list.push_back(revising_dummy->get_now_speed(phase));
			phase = revising_dummy->get_next_poi_arrive_phase_when_moving(phase);
			phase = revising_dummy->get_poi_last_pause_phase_when_pausing(phase);
		}

		std::vector<double> pause_time_list;
		int phase2 = time_manager->last_phase();
		phase2 = revising_dummy->get_poi_init_pause_phase_when_pausing(phase2);
		while (phase2 <= revise_phase) {
			phase2--;
			pause_time_list.push_back(revising_dummy->get_now_pause_time(phase2));
			phase2 = revising_dummy->get_prev_poi_depart_phase_when_moving(phase2);
			phase2 = revising_dummy->get_poi_init_pause_phase_when_pausing(phase2);
		}

		//全停止時間分，ダミーの停止時間を修正する．
		for (int id = revise_phase; id < time_manager->last_phase(); changed_poi_num_id++)
		{
			double distance = map->shortest_distance(revising_dummy->get_trajectory()->get_visited_node_id(revise_phase), revising_dummy->get_trajectory()->get_visited_node_id(next_poi_arrive_phase));
			double time_between_visited_poi = time_manager->time_of_phase(next_poi_arrive_phase) - time_manager->time_of_phase(revise_phase);

			double new_speed = distance / (time_between_visited_poi + Tu);
			double check = revising_dummy->get_now_speed(next_poi_arrive_phase - 1);//prevと現在速度の関係性をチェック
			double prev_speed = revising_dummy->get_now_speed(next_poi_arrive_phase - 1);

			//速度の変化分
			double variable_speed = new_speed - prev_speed;

			//最大変化量・最小変化量
			double max_variable_speed = calc_max_variable_speed(new_speed).first;
			double min_variable_speed = calc_max_variable_speed(new_speed).second;

			//最大最小速度
			double max_speed = requirement->average_speed_of_dummy + 0.5 * requirement->speed_range_of_dummy;
			double min_speed = requirement->average_speed_of_dummy - 0.5 * requirement->speed_range_of_dummy;

			//変更速度．初期値は変更分
			double change_speed = new_speed;

			//修正幅 ＜ 最大変化量　を満たし，かつ，修正幅 < 最大速度とする．
			if (variable_speed > 0) {
				if (variable_speed > max_variable_speed) change_speed = new_speed + max_variable_speed;
				if (change_speed > max_speed) change_speed = max_speed;
			}
			else {
				if (std::abs(variable_speed) < min_variable_speed) change_speed = new_speed + min_variable_speed;
				if (change_speed < min_speed) change_speed = min_speed;
			}
			//change_timeを現在の残り停止時間に記録
			recalculation_path(revising_dummy->get_trajectory()->get_visited_node_id(revise_phase), revising_dummy->get_trajectory()->get_visited_node_id(next_poi_arrive_phase), &revise_phase, change_speed, pause_time_list.at(changed_poi_num_id));

			change_speed -= new_speed;
			if (change_speed == 0.0) break;
		}

		//変更されなかった分はprev_trajectoryをcopyさせる	
		//同じ位置をコピーするために，その位置を示すフェーズを取得
		int same_phase = prev_trajectory->search_phase_same_id(revising_dummy->get_trajectory()->get_visited_node_id(revise_phase));
		int last_copied_phase = same_phase + revising_dummy->get_trajectory()->last_phase() - revise_phase;
		int last_phase = prev_trajectory->get_positions()->size() - 1;
		//revising_dummy->get_trajectory()->copy_trajectory(revise_phase, last_phase, same_phase, last_phase, *prev_trajectory);

		//now_speed_listとnow_pause_listのコピーも行う
		std::copy(prev_now_speed_list.begin() + same_phase, prev_now_speed_list.begin() + last_copied_phase, revising_dummy->get_now_speed_list().begin() + revise_phase);
		std::copy(prev_now_pause_time_list.begin() + same_phase, prev_now_pause_time_list.begin() + last_copied_phase, revising_dummy->get_now_pause_time_list().begin() + revise_phase);
	}


	///<summary>
	/// ダミーの停止位置の修正
	///</summary>
	void KatoMasterMethod::revise_dummy_visit_poi(int phase_id)
	{

	}
	

	///<summary>
	/// ダミーの停止位置の修正
	/// 1:現在の残り停止時間を基に，停止phaseを埋める
	/// 2:pathを再計算(現在停止時間を0でセットし直すことに注意)
	///</summary>
	void KatoMasterMethod::recalculation_path(const Graph::MapNodeIndicator& source, const Graph::MapNodeIndicator& destination, int *revise_phase, double speed, double next_pause_time)
	{
		double dest_time = 0.0;
		
		//負の場合の対応
		if (revising_dummy->get_now_pause_time(*revise_phase) < 0) {
			dest_time = -revising_dummy->get_now_pause_time(*revise_phase);
			revising_dummy->set_now_pause_time(*revise_phase, 0.0);
		}
		double rest_pause_time = revising_dummy->get_now_pause_time(*revise_phase);
		lldiv_t variable_of_converted_pause_time_to_phase = std::lldiv(rest_pause_time, requirement->service_interval);
				
		//現在phaseから，now_puase_timeが0以下になるまで，停止情報を登録
		for (int i = 0; i < variable_of_converted_pause_time_to_phase.quot; i++)
		{
			(*revise_phase)++;
			rest_pause_time -= (double)requirement->service_interval;
			revising_dummy->set_now_pause_time(*revise_phase, rest_pause_time);
			revising_dummy->set_position_of_phase(*revise_phase, source, map->get_static_poi(source.id())->data->get_position());
		}
		
		std::vector<Graph::MapNodeIndicator> shortests_path_between_pois = map->get_shortest_path(source, destination);
		std::vector<Graph::MapNodeIndicator>::iterator path_iter = shortests_path_between_pois.begin();//pathを検索するためのindex

		//sourceからの距離
		//最初だけ停止時間をphaseに換算した時の余りをtimeとし，それ以外はservice_intervalをtimeとして，現在地から求めたい地点のdistanceを計算
		//速度はphaseで埋める前を参照しなければならないことに注意
		double now_time = requirement->service_interval - revising_dummy->get_now_pause_time(*revise_phase) - dest_time;
		
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

			if (*revise_phase == time_manager->phase_count() - 1) return;//残りのpathを決める時の終了条件
			//以下，停止位置に関する情報を登録
			(*revise_phase)++;
			revising_dummy->set_position_of_phase(*revise_phase, Graph::MapNodeIndicator(Graph::NodeType::OTHERS, Graph::NodeType::OTHERS), arrive_position);
			revising_dummy->set_now_speed(*revise_phase, speed);
			revising_dummy->set_now_pause_time(*revise_phase, 0.0);
			now_time += requirement->service_interval;
		}

		//destinationのところまで補完できたら，rest_timeを保持しておく！
		double time_between_arrive_position_and_dest_position = now_time - total_time_from_source_to_destination;
		double dest_rest_time
			= time_between_arrive_position_and_dest_position == requirement->service_interval ? 0 : time_between_arrive_position_and_dest_position;
		
		//目的地の登録
		//登録情報(到着phase,dest_rest_time)を更新
		(*revise_phase)++;
		revising_dummy->set_position_of_phase(*revise_phase, destination, map->get_static_poi(destination.id())->data->get_position());
		revising_dummy->set_now_pause_time(*revise_phase, next_pause_time);
	}

	///<summary>
	/// ダミーの訪問POI情報を更新する
	/// pause_phases,
	///</summary>
	void KatoMasterMethod::update_visited_pois_info_of_dummy()
	{
		/*
		std::vector<Entity::VisitedPoiInfo> visited_poi_list = revising_dummy->get_visited_poi_info_list();
		int id = 0;
		int phase_id = 0;
		int num = 0;
		//pause_phasesの更新
		for (std::vector<double>::iterator iter = revising_dummy->get_now_pause_time_list().begin(); *iter != 0.0; iter++) {
			if (*iter != 0.0) {
				int start_phase = std::distance(revising_dummy->get_now_pause_time_list().begin(), iter);

			}
			revising_dummy->revise_pause_phases();
			if (*(iter + 1) == 0.0) {}

		}
		*/
	}

	///<summary>
	/// ダミーの訪問POI情報を更新する
	///</summary>
	void KatoMasterMethod::increment_visited_pois_info_list_id_of_users(int phase_id)
	{
		if ((phase_id - 1) == input_user->get_pause_phases().back()) input_user->increment_visited_pois_info_list_id();
		if ((phase_id - 1) == real_user->get_pause_phases().back()) real_user->increment_visited_pois_info_list_id();
		if ((phase_id - 1) == predicted_user->get_pause_phases().back()) predicted_user->increment_visited_pois_info_list_id();
	}


	///<summary>
	/// ダミーの訪問POI情報を更新する
	///</summary>
	void KatoMasterMethod::clear_visited_pois_info_list_id_of_users()
	{
		input_user->clear_visited_pois_info_list_id();
		real_user->clear_visited_pois_info_list_id();
		predicted_user->clear_visited_pois_info_list_id();
	}
	

	///<summary>
	/// 初期化
	///</summary>
	void KatoMasterMethod::initialize()
	{
	}


	///<summary>
	/// ここが提案手法の核になる部分.ダミーの行動を修正する
	///</summary>
	void KatoMasterMethod::revise_dummy_positions(int phase_id, entity_id dummy_id)
	{
		//ユーザの行動判定及びダミーの修正
		if (check_going_same_poi_as_plan()) {
			Method::KatoMasterMethod::ChangeParameter parmeter = check_user_plan(phase_id);
			if (parmeter != NO_CHANGE) {
				revise_dummy_trajectory(phase_id);//dummyの行動プランの更新
				update_user_plan(parmeter, phase_id);//次の停止地点の到着時間を予測し，ユーザの行動プランを更新
			}
		}
	}
		

	void KatoMasterMethod::run()
	{
		//ここで実行時間の計測を開始
		timer->start();

		//初期化.行動変更後のユーザの生成
		initialize();

		//ここが実行部分(各時刻のダミー位置を計算する)(加藤さん卒論手法)[Kato 13]
		Method::KatoBachelorMethod::decide_dummy_positions();
		clear_visited_pois_info_list_id_of_users();//usersのvisited_pois_info_list_idのクリア
		
		//ここでユーザの行動の予測やダミーの行動を修正する(加藤さん修論手法)[Kato 14]
		for (int phase_id = 0; phase_id < real_user->get_trajectory()->phase_count(); phase_id++){
			for (size_t dummy_id = 1; dummy_id <= entities->get_dummy_count(); dummy_id++){
				revising_dummy = entities->get_dummy_by_id(dummy_id);
				//ダミーのvisited_pois_info_list_idの更新
				if ((phase_id - 1) == revising_dummy->get_pause_phases().back()) revising_dummy->increment_visited_pois_info_list_id();
				//ユーザのvisited_pois_info_list_idの更新
				increment_visited_pois_info_list_id_of_users(phase_id);
				//ダミーとユーザの行動プランの修正
				revise_dummy_positions(phase_id, dummy_id);
			}
		}

		//ここで計測を終了
		timer->end();

		//終了処理
		terminate();
	}

}