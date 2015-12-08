#include "stdafx.h"
#include "KatoMasterMethod.h"

namespace Method
{

	///<summary>
	/// コンストラクタ
	/// これにSimulatorで作成した各種入力への参照を渡す
	///</summary>
	KatoMasterMethod::KatoMasterMethod(std::shared_ptr<Map::BasicDbMap const> map, std::shared_ptr<Entity::PauseMobileEntity<Geography::LatLng>> user, std::shared_ptr<Requirement::KatoMethodRequirement const> requirement, std::shared_ptr<Time::TimeSlotManager> time_manager)
		:KatoBachelorMethod(map, user, requirement, time_manager),
		real_user(nullptr), predicted_user(nullptr)
	{
		input_user = entities->get_user();
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
	std::shared_ptr<Entity::PauseMobileEntity<Geography::LatLng>> KatoMasterMethod::make_predicted_user_plan(std::shared_ptr<Entity::PauseMobileEntity<Geography::LatLng>> input_user)
	{
		std::shared_ptr<std::vector<std::shared_ptr<Geography::LatLng>>> input_user_positions = input_user->read_trajectory()->read_positions();
		std::shared_ptr<std::vector<std::shared_ptr<Geography::LatLng>>> predicted_user_positions;

		std::copy(input_user_positions->begin(), input_user_positions->end(), predicted_user_positions->begin());

		return predicted_user;
	}

		
	///<summary>
	/// ユーザの行動プランに含まれる停止地点に向かっているかどうかをチェック
	///</summary>
	bool KatoMasterMethod::check_going_pause_position_in_plan()
	{
		return true;
	}

	///<summary>
	/// ユーザの行動プラン変更の判断
	/// real_userとpredict_userの違いで見る
	/// ユーザがプラン通りに行動している場合はNO_CHANGEをリターン
	///</summary>
	std::pair<KatoMasterMethod::ChangeParameter, double> KatoMasterMethod::check_user_plan(int now_phase)
	{
		//止まっている→停止時間の変更or停止地点の変更
		//もし片方でも停止していたら→停止時間変更の判断
		if (predicted_user->check_pause_flag() || real_user->check_pause_flag()) {
			return check_user_pause_time(now_phase);
		}
		//動いていたら，
		else {
			if (/*もし線形補間した線の上に位置していたら，*/true) {
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
		std::pair<KatoMasterMethod::ChangeParameter, double> KatoMasterMethod::check_user_pause_time(int now_phase)
		{
			double change_time = 0.0;
			//もしプラン通り停止していたら(両者の停止フラグが１)，NO_CHANGE
			if (predicted_user->check_pause_flag() && real_user->check_pause_flag()) {
				return std::make_pair(NO_CHANGE, change_time);
			}
			else if(predicted_user->check_pause_flag() || real_user->check_pause_flag()){
				//もしrealのほうが停止フラグが0で(出発していて)，predictが1なら,予定より早く出発
				if (predicted_user->check_pause_flag() == true && real_user->check_pause_flag() == false) {
					//change_timeの差分を求める
					change_time -= requirement->service_interval;
					return std::make_pair(SHORTER_PAUSE_TIME, change_time);
				}
				else {
					//change_timeの差分を求める
					change_time += requirement->service_interval;
					return std::make_pair(LONGER_PAUSE_TIME, change_time);
				}
			}
			//両方フラグが0なら(速度変更からジャンプして飛んできた場合)，速度変更でなく，rest_time分をずれを検知
			else {
				//移動距離がrealのほうが大きいなら
				if (true) {
					//change_timeの差分を求める
					return std::make_pair(SHORTER_PAUSE_TIME, change_time);
				}
				else {
					//change_timeの差分を求める
					return std::make_pair(LONGER_PAUSE_TIME, change_time);
				}
			}
		}
	

		///<summary>
		/// ユーザの行動プラン変更の判断
		/// real_userとpredicted_userの違いで見る
		/// 速度のチェック
		///</summary>
		std::pair<KatoMasterMethod::ChangeParameter, double> KatoMasterMethod::check_user_speed(int now_phase)
		{
			Geography::LatLng previous_real_user_position = *real_user->read_node_pos_info_of_phase(now_phase - 1).second;
			Geography::LatLng previous_predicted_user_position = *predicted_user->read_node_pos_info_of_phase(now_phase - 1).second;
			Geography::LatLng now_real_user_position = *real_user->read_node_pos_info_of_phase(now_phase).second;
			Geography::LatLng now_predicted_user_position = *predicted_user->read_node_pos_info_of_phase(now_phase).second;
			
			double change_time = 0.0;
			//もし同じ場所にいたら，NO_CHANGE
			if (now_predicted_user_position == now_real_user_position) {
				return std::make_pair(NO_CHANGE, change_time);
			}
			else {
				if (/*もしrest_time分を考慮して，逆算して，速度が同じなら*/true) {
					return check_user_pause_time(now_phase);
				}
				else {
					if (/*移動距離がrealのほうが大きいなら*/true) {
						//change_timeの差分を求める
						return std::make_pair(SLOER_SPEED, change_time);
					}
					else {
						//change_timeの差分を求める
						return std::make_pair(FASTER_SPEED, change_time);
					}
				}
			}
		}

		///<summary>
		/// ユーザの行動プラン変更の判断
		/// real_userとpredicted_userの違いで見る
		/// pathのチェック
		///</summary>
		std::pair<KatoMasterMethod::ChangeParameter, double> KatoMasterMethod::check_user_path(int now_phase)
		{
			double change_time = 0.0;
			//change_timeを求める
			return std::make_pair(PATH, change_time);
		}


		///<summary>
		/// ユーザの行動プラン変更の判断
		/// real_userとpredicted_userの違いで見る
		/// 停止位置のチェック
		///</summary>
		std::pair<KatoMasterMethod::ChangeParameter, double> KatoMasterMethod::check_user_position(int now_phase)
		{
			return std::make_pair(POSITION, 0.0);
		}

		

	///<summary>
	/// ユーザの次の停止地点の到着時間を予測する
	///</summary>
	void KatoMasterMethod::update_user_plan(std::pair<ChangeParameter, double> check_parameter, int phase_id)
	{
		
	}

	///<summary>
	/// ユーザの停止時間の修正
	///</summary>
	void KatoMasterMethod::update_user_pause_time(std::pair<ChangeParameter, double> check_parameter, int phase_id)
	{
		/*
		if (check_parameter.first == SHORTER_PAUSE_TIME) {
			//直前まで停止していた停止地点から，
			//実際のユーザの位置まで，行動プラン通りの移動速度と移動経路で移動したものとみなし，移動時間を求め，実際の停止時間を逆算して求める．
		}
		else() {

		}
		Geography::LatLng previous_real_user_position = *real_user->read_node_pos_info_of_phase(phase_id - 1).second;
		Geography::LatLng previous_predicted_user_position = *predicted_user->read_node_pos_info_of_phase(phase_id - 1).second;
		Geography::LatLng now_real_user_position = *real_user->read_node_pos_info_of_phase(phase_id).second;
		Geography::LatLng now_predicted_user_position = *predicted_user->read_node_pos_info_of_phase(phase_id).second;

		//userがプランより長く止まっている場合
		//→predicted_userも現在phaseまで停止しているとする
		if (previous_real_user_position == previous_predicted_user_position) {
			//predicted_user->get_trajectory();
			//trajectoryを修正する．

		}
		else {
			
		}
		*/
	}

	///<summary>
	/// ユーザのパスの修正
	///</summary>
	void KatoMasterMethod::update_user_path(std::pair<ChangeParameter, double> check_parameter, int phase_id)
	{

	}

	///<summary>
	/// ユーザの移動速度の修正
	///</summary>
	void KatoMasterMethod::update_user_speed(std::pair<ChangeParameter, double> check_parameter, int phase_id)
	{

	}

	///<summary>
	/// ユーザの停止位置の修正
	///</summary>
	void KatoMasterMethod::update_user_pause_position(std::pair<ChangeParameter, double> check_parameter, int phase_id)
	{

	}



	///<summary>
	/// ダミーの行動プランを修正する
	///</summary>
	void KatoMasterMethod::revise_dummy_movement_plan(int phase_id)
	{

	}


	///<summary>
	/// ダミーの停止時間の修正
	///</summary>
	void KatoMasterMethod::revise_dummy_pause_time(int phase_id)
	{

		//前の値の保持
		double previous_pause_time = revising_dummy->get_pause_time(phase_id);
		double previous_arrive_time = time_manager->time_of_phase(phase_id + 1);

		if (std::abs(changes_in_arrival_time) > requirement->max_variation_of_pause_time)
		{
			double new_pause_time = changes_in_arrival_time > 0 ? previous_pause_time + requirement->max_variation_of_pause_time : previous_pause_time - requirement->max_variation_of_pause_time;
			revising_dummy->set_pause_time(phase_id, new_pause_time);
		}
		else
		{
			revising_dummy->set_pause_time(phase_id, previous_pause_time + changes_in_arrival_time);
		}

		if (requirement->max_pause_time < time_manager->time_of_phase(phase_id)) revising_dummy->set_pause_time(phase_id, requirement->max_pause_time);
		if (requirement->min_pause_time > time_manager->time_of_phase(phase_id)) revising_dummy->set_pause_time(phase_id, requirement->min_pause_time);

		//停止時間の変化量を求める
		int variation_of_pause_time = revising_dummy->get_pause_time(phase_id) - previous_pause_time;

		for (int i = phase_id + 1; i <= time_manager->phase_count(); i++)
		{
			//revise_time_manager->time_of_phase(i) += variation_of_pause_time;
		}
		if (time_manager->time_of_phase(phase_id + 1) == previous_arrive_time + changes_in_arrival_time) return;

		changes_in_arrival_time -= variation_of_pause_time;


	}


	///<summary>
	/// ダミーの移動経路の修正
	///</summary>
	void KatoMasterMethod::revise_dummy_path(int phase_id)
	{
		/*
		time_t previous_arrive_time = time_manager->time_of_phase(phase_id + 1);
		time_t tempT = 10000000000000000000;

		while (全てのトラジェクトリをチェックし終えるまで)
		{
		phase_idのときの存在地点;
		time_manager->time_of_phase(phase_id + 1) = time_manager->time_of_phase(phase_id) + revising_dummy->get_pause_time(phase_id) + distance_of_Tri / revising_dummy->get_speed(phase_id);

		}
		*/
	}


	///<summary>
	/// ダミーの行動速度の修正
	///</summary>
	void KatoMasterMethod::revise_dummy_speed(int phase_id)
	{

		int previous_arrive_time = time_manager->time_of_phase(phase_id + 1);
		double previous_speed = revising_dummy->get_speed(phase_id);
		double distance = map->shortest_distance(revising_dummy->read_node_pos_info_of_phase(phase_id).first, revising_dummy->read_node_pos_info_of_phase(phase_id).first);
		int time = distance / (time_manager->time_of_phase(phase_id + 1) + changes_in_arrival_time - time_manager->time_of_phase(phase_id));
		revising_dummy->set_speed(phase_id, time);

		if (std::abs(revising_dummy->get_speed(phase_id) - previous_speed) > requirement->max_variation_of_speed)
		{
			double new_speed = revising_dummy->get_speed(phase_id) - previous_speed > 0 ? previous_speed + requirement->max_variation_of_speed : previous_speed - requirement->max_variation_of_speed;
			revising_dummy->set_pause_time(phase_id, new_speed);
		}
		double max_speed = requirement->average_speed_of_dummy + 0.5* requirement->speed_range_of_dummy;
		double min_speed = requirement->average_speed_of_dummy - 0.5* requirement->speed_range_of_dummy;
		if (max_speed < revising_dummy->get_speed(phase_id)) revising_dummy->set_speed(phase_id, max_speed);
		if (min_speed > revising_dummy->get_speed(phase_id)) revising_dummy->set_speed(phase_id, min_speed);

		//time_manager->time_of_phase(phase_id + 1) = time_manager->time_of_phase(phase_id) + revising_dummy->get_pause_time(phase_id) + (time_t)(distance / revising_dummy->get_speed(phase_id));
		time_t variation_of_arrive_time = time_manager->time_of_phase(phase_id + 1) - previous_arrive_time;

		for (int i = phase_id + 1; i <= time_manager->phase_count(); i++)
		{
			//revise_time_manager->time_of_phase(i) += variation_of_pause_time;
		}
		if (time_manager->time_of_phase(phase_id + 1) == previous_arrive_time + changes_in_arrival_time) return;

	}


	///<summary>
	/// ダミーの停止位置の修正
	///</summary>
	void KatoMasterMethod::revise_dummy_pause_position(int phase_id)
	{

	}




	///<summary>
	/// 初期化
	///</summary>
	void KatoMasterMethod::initialize()
	{
		//ユーザの動きの変更→新しく作る．
		predicted_user = make_predicted_user_plan(entities->get_user());
	}


	///<summary>
	/// ここが提案手法の核になる部分.ダミーの行動を修正する
	///</summary>
	void KatoMasterMethod::revise_dummy_positions()
	{
		for (size_t dummy_id = 1; dummy_id <= entities->get_dummy_count(); dummy_id++)
		{
			revising_dummy = entities->get_dummy_by_id(dummy_id);
			for (int phase_id = 0; phase_id < time_manager->phase_count(); phase_id++)
			{
				if (check_going_pause_position_in_plan()) {
					if (check_user_plan(phase_id).second != 0.0) {
						update_user_plan(check_user_plan(phase_id), phase_id);//次の停止地点の到着時間を予測
						revise_dummy_movement_plan(phase_id);//dummyの行動プランの更新
						//update_user_plan();//ユーザの行動プランを更新
					}
				}
			}
		}
	}

	/*
	///<summary>
	/// 決定した位置を基にMTC等各種評価値を算出する
	///</summary>
	void KatoMasterMethod::evaluate()
	{

	}


	///<summary>
	/// 結果のファイルへのエクスポート
	///</summary>
	void KatoMasterMethod::export_results()
	{

	}


	///<summary>
	/// 終了処理 (今回はスマートポインタを利用しているので，特にやることはない)
	///</summary>
	void KatoMasterMethod::terminate()
	{

	}
	*/

	void KatoMasterMethod::run()
	{
		//ここで実行時間の計測を開始
		timer->start();

		//初期化.行動変更後のユーザの生成
		initialize();

		//ここが実行部分(各時刻のダミー位置を計算する)(加藤さん卒論手法)[Kato 13]
		decide_dummy_positions();

		//ここでユーザの行動の予測やダミーの行動を修正する(加藤さん修論手法)[Kato 14]
		revise_dummy_positions();

		//ここで計測を終了
		timer->end();

		//終了処理
		terminate();
	}

}