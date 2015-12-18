#include "stdafx.h"
#include "KatoMasterMethod.h"

namespace Method
{

	///<summary>
	/// コンストラクタ
	/// これにSimulatorで作成した各種入力への参照を渡す
	///</summary>
	KatoMasterMethod::KatoMasterMethod(std::shared_ptr<Map::HayashidaDbMap const> map, std::shared_ptr<Entity::DifferentMovementUser<Geography::LatLng>> user, std::shared_ptr<Requirement::KatoMethodRequirement const> requirement, std::shared_ptr<Time::TimeSlotManager> time_manager)
		:KatoBachelorMethod(map, user, requirement, time_manager),
		real_user(nullptr), predicted_user(nullptr),Tu(0.0)
	{
		//ユーザが2つ持てるように，データ構造を修正する．input_userの方はget_user()ではないので，注意
		input_user = entities->get_user();
		real_user = entities->get_user();
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
		std::shared_ptr<std::vector<std::shared_ptr<Geography::LatLng>>> input_user_positions = input_user->read_trajectory()->read_positions();
		std::shared_ptr<std::vector<std::shared_ptr<Geography::LatLng>>> predicted_user_positions;

		std::copy(input_user_positions->begin(), input_user_positions->end(), predicted_user_positions->begin());
		
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

		return std::make_pair(requirement->max_variation_of_speed*(max_speed - speed), requirement->max_variation_of_speed*(speed - min_speed));
	}

	///<summary>
	/// 停止時間の最大変化値と最小変化値を計算する
	/// pair(Max, min)
	///</summary>
	std::pair<double, double> KatoMasterMethod::calc_max_variable_pause_time(double pause_time)
	{
		return std::make_pair(requirement->max_variation_of_pause_time*(requirement->max_pause_time - pause_time), requirement->max_variation_of_pause_time*(pause_time - requirement->min_pause_time));
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
		if (predicted_user->check_pause_condition(now_phase) || real_user->check_pause_condition(now_phase)) {
			return check_user_pause_time(now_phase);
		}
		//動いていたら，
		else {
			//もし線形補間した線の上に位置していたら
			if (check_on_the_path(now_phase)) {
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
			//もしプラン通り停止していたら(両者の停止フラグが１)，NO_CHANGE
			if (predicted_user->check_pause_condition(now_phase) && real_user->check_pause_condition(now_phase)) {
				return NO_CHANGE;
			}
			else if(predicted_user->check_pause_condition(now_phase) || real_user->check_pause_condition(now_phase)){
				//もしrealのほうが停止フラグが0で(出発していて)，predictが1なら,予定より早く出発
				if (predicted_user->check_pause_condition(now_phase) == true && real_user->check_pause_condition(now_phase) == false) {
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
			else {
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
			
			//もし同じ場所にいたら，NO_CHANGE
			if (now_predicted_user_position == now_real_user_position) {
				Tu = 0.0;
				return NO_CHANGE;
			}
			else {
				if (/*もしrest_time分を考慮して，逆算して，速度が同じなら*/true) {
					return check_user_pause_time(now_phase);
				}
				else {
					double real_user_dist = Geography::GeoCalculation::lambert_distance(*real_user->read_position_of_phase(now_phase - 1), *real_user->read_position_of_phase(now_phase));
					double predicted_user_dist = Geography::GeoCalculation::lambert_distance(*predicted_user->read_position_of_phase(now_phase - 1), *predicted_user->read_position_of_phase(now_phase));
					//移動距離がrealのほうが大きいなら
					if (real_user_dist > predicted_user_dist) {
						//change_timeの差分を求める
						//ここで，どれくらい速度が大きくなったかも求めてしまう．
						//ここは計算量に関わってきそうだから，要検討
						double real_speed = real_user_dist / requirement->service_interval;
						Tu += map->calc_necessary_time(real_user->read_node_pos_info_of_phase(now_phase).first, real_user->get_poi().first, real_speed);
						return FASTER_SPEED;
					}
					else {
						//change_timeの差分を求める
						//ここで，どれくらい速度が小さくなったかも求めてしまう．	
						double real_speed = real_user_dist / requirement->service_interval;
						Tu -= map->calc_necessary_time(real_user->read_node_pos_info_of_phase(now_phase).first, real_user->get_poi().first, real_speed);;
						return SLOER_SPEED;
					}
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
			double change_time = 0.0;
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
		bool KatoMasterMethod::check_on_the_path(int phase_id)
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
			modification_of_user_trajectory_when_LONGER_PAUSE_TIME(check_parameter, phase_id);
			break;
		case SHORTER_PAUSE_TIME:
			modification_of_user_trajectory_when_SHORTER_PAUSE_TIME(check_parameter, phase_id);
			break;
		case PATH:
			modification_of_user_trajectory_when_PATH(check_parameter, phase_id);
			break;
		case FASTER_SPEED:
			modification_of_user_trajectory_when_FASTER_SPEED(check_parameter, phase_id);
			break;
		case SLOER_SPEED:
			modification_of_user_trajectory_when_SLOER_SPEED(check_parameter, phase_id);
			break;
		case VISIT_POI:
			modification_of_user_trajectory_when_VISIT_POI(check_parameter, phase_id);
			break;
		}
	}

	///<summary>
	/// ユーザの停止時間が予定より長い時のpredicted_userの修正
	///</summary>
	void KatoMasterMethod::modification_of_user_trajectory_when_LONGER_PAUSE_TIME(ChangeParameter check_parameter, int phase_id)
	{
		if (Tu == requirement->service_interval) {
			//trajectoryをずらすことで対応
			predicted_user->get_trajectory()->insert_positions_to_trajectory(phase_id, 1);
			
			//停止時間の修正を行う．
			predicted_user->revise_now_pause_time(phase_id, Tu);
		}
		else {
			//停止時間の修正を行う．
			predicted_user->revise_now_pause_time(phase_id, Tu);

			//経路を再計算
			//recalculation_path(phase_id);
		}
	}


	///<summary>
	/// ユーザの停止時間が予定より短い時のpredicted_userの修正
	///</summary>
	void KatoMasterMethod::modification_of_user_trajectory_when_SHORTER_PAUSE_TIME(ChangeParameter check_parameter, int phase_id)
	{
		if (Tu == requirement->service_interval) {
			//trajectoryをずらすことで対応
			predicted_user->get_trajectory()->delete_positions_to_trajectory(phase_id, 1);

			//停止時間の修正を行う．
			predicted_user->revise_now_pause_time(phase_id, Tu);
		}
		else {
			//経路を再計算
		}
	}

	///<summary>
	/// ユーザのパスの修正
	///</summary>
	void KatoMasterMethod::modification_of_user_trajectory_when_PATH(ChangeParameter check_parameter, int phase_id)
	{

	}

	///<summary>
	/// ユーザの移動速度が予定より早い時の修正
	///</summary>
	void KatoMasterMethod::modification_of_user_trajectory_when_FASTER_SPEED(ChangeParameter check_parameter, int phase_id)
	{
		double real_user_speed = 0.0;

	}

	///<summary>
	/// ユーザの移動速度が予定より遅い時の修正
	///</summary>
	void KatoMasterMethod::modification_of_user_trajectory_when_SLOER_SPEED(ChangeParameter check_parameter, int phase_id)
	{

	}


	///<summary>
	/// ユーザの停止位置の修正
	///</summary>
	void KatoMasterMethod::modification_of_user_trajectory_when_VISIT_POI(ChangeParameter check_parameter, int phase_id)
	{

	}



	///<summary>
	/// ダミーの行動プランを修正する
	///</summary>
	void KatoMasterMethod::revise_dummy_trajectory(int phase_id)
	{
		//poiに止まっている時
		if (revising_dummy->check_pause_condition(phase_id)) {
			//全ての停止地点の到着時間を変更し，Tu分変更させる．
			revise_dummy_pause_time(phase_id);
			revise_dummy_path(phase_id);
			revise_dummy_speed(phase_id);
			if (Tu != 0) revise_dummy_visit_poi(phase_id);
			
		}
		//現在移動中で，プランと同じpoiに向かっている場合，向かっているpoiに停止中だとして考える
		else {
			//全ての停止地点の到着時間を変更し，Tu分変更させる．
			revise_dummy_pause_time(phase_id);
			revise_dummy_path(phase_id);
			revise_dummy_speed(phase_id);
			if (Tu != 0) revise_dummy_visit_poi(phase_id);

		}
	}


	///<summary>
	/// ダミーの停止時間の修正
	///</summary>
	void KatoMasterMethod::revise_dummy_pause_time(int phase_id)
	{
		//前の値の保持
		int pause_phase = revising_dummy->get_arrive_phase();
		double pause_time = revising_dummy->get_now_pause_time(pause_phase);
	
		double max_variable_value = calc_max_variable_pause_time(pause_phase).first;
		double min_variable_value = calc_max_variable_pause_time(pause_phase).second;
		double new_pause_time = Tu;//初期値は，変更分Tu

		//修正幅 ＜ 最大変化量　を満たし，かつ，修正幅 < 最大停止時間とする．
		if (Tu > 0) {
			if (Tu > max_variable_value) new_pause_time = max_variable_value;
			if (max_variable_value > requirement->max_pause_time) new_pause_time = requirement->max_pause_time;
		}
		else {
			if (std::abs(Tu) < min_variable_value) new_pause_time = min_variable_value;
			if (min_variable_value < requirement->min_pause_time) new_pause_time = requirement->min_pause_time;
		}		
				
		//調整した分，ダミーの停止時間を修正する．
		for (int i = phase_id + 1; i <= time_manager->phase_count(); i++)
		{
			//ここに次の訪問POIの到着時間と出発時刻を考慮して，修正する関数をラムダ式で記述．		
		}
		//if (time_manager->time_of_phase(phase_id + 1) == next_arrive_time + Tu) return;
		
		//もし調整しきれなかったら，次のpathの調整をする
		Tu -= new_pause_time;
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
		//前の値の保持
		int next_pause_phase = revising_dummy->get_arrive_phase();
		int next_next_pause_phase = revising_dummy->get_arrive_phase();
		double next_departing_speed = revising_dummy->get_now_speed(next_pause_phase);
		double next_arrive_time = time_manager->time_of_phase(next_pause_phase);
		
		double distance = map->shortest_distance(revising_dummy->read_node_pos_info_of_phase(next_pause_phase).first, revising_dummy->read_node_pos_info_of_phase(next_next_pause_phase).first);
		
		double new_speed = distance / (time_manager->time_of_phase(next_next_pause_phase) + Tu - time_manager->time_of_phase(next_pause_phase));
		revising_dummy->set_now_speed(next_pause_phase, new_speed);

		if (std::abs(revising_dummy->get_now_speed(next_pause_phase) - next_departing_speed) > requirement->max_variation_of_speed)
		{
			new_speed = revising_dummy->get_now_speed(next_pause_phase) - next_departing_speed > 0 ? next_departing_speed + requirement->max_variation_of_speed : next_departing_speed - requirement->max_variation_of_speed;
			revising_dummy->set_pause_time(next_pause_phase, new_speed);
		}
		double max_speed = requirement->average_speed_of_dummy + 0.5 * requirement->speed_range_of_dummy;
		double min_speed = requirement->average_speed_of_dummy - 0.5 * requirement->speed_range_of_dummy;
		if (max_speed < revising_dummy->get_now_speed(next_pause_phase)) revising_dummy->set_now_speed(next_pause_phase, max_speed);
		if (min_speed > revising_dummy->get_now_speed(next_pause_phase)) revising_dummy->set_now_speed(next_pause_phase, min_speed);

		//time_manager->time_of_phase(next_next_pause_phase) = time_manager->time_of_phase(phase_id) + revising_dummy->get_pause_time(phase_id) + (time_t)(distance / revising_dummy->get_speed(phase_id));
		double variation_of_arrive_time = time_manager->time_of_phase(next_next_pause_phase) - next_arrive_time;

		for (int i = phase_id + 1; i <= time_manager->phase_count(); i++)
		{
			//ここに，速度が変化した際のダミーの行動修正を記述
		}
		if (time_manager->time_of_phase(phase_id + 1) == next_arrive_time + Tu) return;

	}


	///<summary>
	/// ダミーの停止位置の修正
	///</summary>
	void KatoMasterMethod::revise_dummy_visit_poi(int phase_id)
	{

	}
	

	///<summary>
	/// ダミーの停止位置の修正
	///</summary>
	void KatoMasterMethod::recalculation_path(const Graph::MapNodeIndicator& source, const Graph::MapNodeIndicator& destination, int phase_id)
	{
		double pause_position_speed = revising_dummy->get_now_speed(phase_id);

		//現在phaseから，now_puase_timeが0以下になるまで，停止情報を登録
		phase_id++;
		for (double rest_pause_time = revising_dummy->get_now_pause_time(phase_id); time < 0; phase_id++)
		{
			rest_pause_time -= requirement->service_interval;
			revising_dummy->set_now_pause_time(phase_id, rest_pause_time);
			revising_dummy->set_position_of_phase(phase_id, source, map->get_static_poi(source.id())->data->get_position());
		}

		std::vector<Graph::MapNodeIndicator> shortests_path_between_pois = map->get_shortest_path(source, destination);
		std::vector<Graph::MapNodeIndicator>::iterator path_iter = shortests_path_between_pois.begin();//pathを検索するためのindex

		//sourceからの距離
		//最初だけ停止時間をphaseに換算した時の余りをtimeとし，それ以外はservice_intervalをtimeとして，現在地から求めたい地点のdistanceを計算
		//速度はphaseで埋める前を参照しなければならないことに注意
		double now_time = requirement->service_interval - revising_dummy->get_now_pause_time(phase_id);
		double total_time_from_source_to_destination = map->calc_necessary_time(source, destination, pause_position_speed);
		Graph::MapNodeIndicator nearest_position = source;
		//pathを作成．場所は一番近いintersection同士で線形補間する．MapNodeIndicatorのTypeはINVALIDとする．
		while (now_time < total_time_from_source_to_destination)
		{
			//最初は停止時間をphaseに換算したときの余り分をdistanceとして，最短路の中で一番近いintersectionを探し，線形補間する．
			//double total_path_length = map->shortest_distance(source, *path_iter);
			while (now_time > map->calc_necessary_time(source, *path_iter, pause_position_speed))
			{
				nearest_position = *path_iter;
				path_iter++;
			}

			double distance = now_time * pause_position_speed;
			double distance_between_nearest_intersection_and_arrive_position = distance - map->shortest_distance(source, nearest_position);
			Geography::LatLng nearest_latlng
				= nearest_position.type() == Graph::NodeType::POI ? map->get_static_poi(nearest_position.id())->data->get_position() : *map->get_static_node(nearest_position.id())->data;
			Geography::LatLng next_nearest_latlang
				= (*path_iter).type() == Graph::NodeType::POI ? map->get_static_poi((*path_iter).id())->data->get_position() : *map->get_static_node((*path_iter).id())->data;
			double angle = Geography::GeoCalculation::lambert_azimuth_angle(nearest_latlng, next_nearest_latlang);

			Geography::LatLng arrive_position = Geography::GeoCalculation::calc_translated_point(nearest_latlng, distance_between_nearest_intersection_and_arrive_position, angle);

			if (phase_id == time_manager->phase_count() - 1) return;//残りのpathを決める時の終了条件
			(phase_id)++;
			revising_dummy->set_position_of_phase(phase_id, Graph::MapNodeIndicator(Graph::NodeType::OTHERS, Graph::NodeType::OTHERS), arrive_position);
			revising_dummy->set_now_speed(phase_id, pause_position_speed);

			now_time += requirement->service_interval;
		}

		//destinationのところまで補完できたら，rest_timeを保持しておく！
		double time_between_arrive_position_and_dest_position = now_time - total_time_from_source_to_destination;
		double dest_rest_time
			= time_between_arrive_position_and_dest_position == requirement->service_interval ? 0 : time_between_arrive_position_and_dest_position;
		//目的地の登録
		//speedは別途設定のため不要
		(phase_id)++;
		revising_dummy->set_position_of_phase(phase_id, destination, map->get_static_poi(destination.id())->data->get_position());

	}

	
	///<summary>
	/// 初期化
	///</summary>
	void KatoMasterMethod::initialize()
	{
		//ユーザの動きの変更→新しく作る．
		predicted_user = copy_predicted_user_plan(entities->get_user());
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
				if (check_going_same_poi_as_plan()) {
					if (check_user_plan(phase_id) != NO_CHANGE) {
						revise_dummy_trajectory(phase_id);//dummyの行動プランの更新
						update_user_plan(check_user_plan(phase_id), phase_id);//次の停止地点の到着時間を予測し，ユーザの行動プランを更新
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