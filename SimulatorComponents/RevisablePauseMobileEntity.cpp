#include "stdafx.h"
#include "RevisablePauseMobileEntity.h"

namespace Entity
{
	///<summary>
	/// コンストラクタ
	///</summary>
	template <typename POSITION_TYPE, typename TRAJECTORY_TYPE>
	RevisablePauseMobileEntity<POSITION_TYPE, TRAJECTORY_TYPE>::RevisablePauseMobileEntity(entity_id id, std::shared_ptr<Time::TimeSlotManager const> timeslot)
		: PauseMobileEntity<POSITION_TYPE, TRAJECTORY_TYPE>(id, timeslot)
	{
	}

	///<summary>
	/// デストラクタ
	///</summary>
	template <typename POSITION_TYPE, typename TRAJECTORY_TYPE>
	RevisablePauseMobileEntity<POSITION_TYPE, TRAJECTORY_TYPE>::~RevisablePauseMobileEntity()
	{
	}

	///<summary>
	/// pause_timeを修正する．
	/// 修正する前に，一旦popし，リセットする．
	///</summary>
	template <typename POSITION_TYPE, typename TRAJECTORY_TYPE>
	void RevisablePauseMobileEntity<POSITION_TYPE, TRAJECTORY_TYPE>::revise_pause_phases(int start_pause_phase, int num)
	{
		while (!visited_pois_info_list.at(visited_pois_info_list_id).pause_phases.empty()) visited_pois_info_list.at(visited_pois_info_list_id).pause_phases.pop_back();
		for (int i = 0; i < num; start_pause_phase++) visited_pois_info_list.at(visited_pois_info_list_id).pause_phases.push_back(start_pause_phase);
	}

	
	///<summary>
	/// 任意の訪問POIのpause_timeを修正する．
	/// 現在停止中の場合は，到着フェーズから現在フェーズまで補完することを忘れずに
	///</summary>
	template <typename POSITION_TYPE, typename TRAJECTORY_TYPE>
	void RevisablePauseMobileEntity<POSITION_TYPE, TRAJECTORY_TYPE>::revise_pause_phases_of_any_visited_poi(int start_pause_phase, int num, int id)
	{
		while (!visited_pois_info_list.at(id).pause_phases.empty()) visited_pois_info_list.at(id).pause_phases.pop_back();
		if (isPause(start_pause_phase)) {
			for (int i = visited_pois_info_list.at(id).arrive_phase; i < start_pause_phase; i++) visited_pois_info_list.at(id).pause_phases.push_back(i);
		}
		for (int i = 0; i < num; i++, start_pause_phase++) visited_pois_info_list.at(id).pause_phases.push_back(start_pause_phase);
	}

	///<summary>
	/// pause_phasesをnum個分加算する
	///</summary>
	template <typename POSITION_TYPE, typename TRAJECTORY_TYPE>
	void RevisablePauseMobileEntity<POSITION_TYPE, TRAJECTORY_TYPE>::add_pause_phases(int num, int phase)
	{
		//全部インクリメントさせて，現在の注目位置の最初に挿入
		for (int id = visited_pois_info_list_id; id < get_visited_pois_num(); id++) {
			for (auto iter = visited_pois_info_list.at(id).pause_phases.begin(); iter != visited_pois_info_list.at(id).pause_phases.end(); iter++) {
				(*iter) += num;
			}
		}
		auto it = visited_pois_info_list.at(visited_pois_info_list_id).pause_phases.begin();
		it = visited_pois_info_list.at(visited_pois_info_list_id).pause_phases.insert(it, phase);
	}

	///<summary>
	/// pause_phasesをnum個分減算する
	///</summary>
	template <typename POSITION_TYPE, typename TRAJECTORY_TYPE>
	void RevisablePauseMobileEntity<POSITION_TYPE, TRAJECTORY_TYPE>::remove_pause_phases(int num, int phase)
	{
		//全部デクリメントさせて，現在の注目位置の最初に挿入
		for (int id = visited_pois_info_list_id; id < get_visited_pois_num(); id++) {
			for (auto iter = visited_pois_info_list.at(id).pause_phases.begin(); iter != visited_pois_info_list.at(id).pause_phases.end(); iter++) {
				(*iter) -= num;
			}
		}
		auto it = visited_pois_info_list.at(visited_pois_info_list_id).pause_phases.begin();
		visited_pois_info_list.at(visited_pois_info_list_id).pause_phases.erase(it);
	}

	///<summary>
	/// 到着phaseを修正する．
	///</summary>
	template <typename POSITION_TYPE, typename TRAJECTORY_TYPE>
	void RevisablePauseMobileEntity<POSITION_TYPE, TRAJECTORY_TYPE>::revise_arrive_phase(int arrive_phase)
	{
		visited_pois_info_list.at(visited_pois_info_list_id).arrive_phase = arrive_phase;
	}

	///<summary>
	/// 任意の訪問POIの到着phaseを修正する．
	///</summary>
	template <typename POSITION_TYPE, typename TRAJECTORY_TYPE>
	void RevisablePauseMobileEntity<POSITION_TYPE, TRAJECTORY_TYPE>::revise_arrive_phase_of_any_visited_poi(int arrive_phase, int id)
	{
		visited_pois_info_list.at(id).arrive_phase = arrive_phase;
	}

	///<summary>
	/// 全訪問予定POIの到着phaseを修正する．
	///</summary>
	template <typename POSITION_TYPE, typename TRAJECTORY_TYPE>
	void RevisablePauseMobileEntity<POSITION_TYPE, TRAJECTORY_TYPE>::revise_all_arrive_phase()
	{
		for (int id = 0; id < get_visited_pois_num(); id++) {
			visited_pois_info_list.at(id).arrive_phase = visited_pois_info_list.at(id).pause_phases.front();
		}
	}


	///<summary>
	/// pause_timeを修正する．
	///</summary>
	template <typename POSITION_TYPE, typename TRAJECTORY_TYPE>
	void RevisablePauseMobileEntity<POSITION_TYPE, TRAJECTORY_TYPE>::revise_pause_time(double new_pause_time)
	{
		visited_pois_info_list.at(visited_pois_info_list_id).pause_time = new_pause_time;
	}

	///<summary>
	/// pause_timeを修正する．
	///</summary>
	template <typename POSITION_TYPE, typename TRAJECTORY_TYPE>
	void RevisablePauseMobileEntity<POSITION_TYPE, TRAJECTORY_TYPE>::revise_pause_time_using_pause_phase(double new_pause_time, int phase)
	{
		for (std::vector<VisitedPoiInfo>::iterator iter = visited_pois_info_list.begin(); iter != visited_pois_info_list.end(); iter++) {
			for (std::vector<int>::iterator iter2 = iter->pause_phases.begin(); iter2 != iter->pause_phases.end(); iter2++) {
				if (*iter2 == phase) iter->pause_time = new_pause_time;
			}
		}
		throw std::invalid_argument("Not Found");
	}

	///<summary>
	/// ある訪問POIにおけるpause_timeを修正する．
	///</summary>
	template <typename POSITION_TYPE, typename TRAJECTORY_TYPE>
	void RevisablePauseMobileEntity<POSITION_TYPE, TRAJECTORY_TYPE>::revise_pause_time_of_any_visited_poi(double new_pause_time, int id)
	{
		visited_pois_info_list.at(id).pause_time = new_pause_time;
	}


	///<summary>
	/// now_pause_timeをchange_time分増減させる．
	/// 残り時間が負になる場合があるので，各プログラムで場合分けしなければならないことに注意
	///</summary>
	template <typename POSITION_TYPE, typename TRAJECTORY_TYPE>
	void RevisablePauseMobileEntity<POSITION_TYPE, TRAJECTORY_TYPE>::add_now_pause_time(int now_phase, double change_time)
	{
		now_pause_time_list.at(now_phase) += change_time;
		/*
		if (now_pause_time_list.at(now_phase) >= 0) {
			now_pause_time_list.at(now_phase) += change_time;
		}
		//負の時は，0.0を設定し，rest_pause_time_when_departingに余り分を登録
		else {
			double rest_time = std::abs(change_time) - now_pause_time_list.at(now_phase);
			set_now_pause_time(now_phase, 0.0);
			set_rest_pause_time_when_departing(rest_time);
		}
		*/
	}
	
	///<summary>
	/// now_pause_time_listをphase_id時まで修正する．
	/// 現在停止中かつnew_puase_timeがサービス利用間隔より大きかったら，現在停止POIの修正で，
	/// そうでない場合は，今向かっているPOIの停止時間の修正
	///</summary>
	template <typename POSITION_TYPE, typename TRAJECTORY_TYPE>
	void RevisablePauseMobileEntity<POSITION_TYPE, TRAJECTORY_TYPE>::revise_now_pause_time(int phase_id, double new_pause_time)
	{
		double service_interval = trajectory->find_phase_of_time(1) - trajectory->find_phase_of_time(0);
		int init_pause_phase = get_now_speed(phase_id) == 0.0 && get_now_pause_time(phase_id) > service_interval ? get_arrive_phase_using_pause_phase(phase_id) : get_arrive_phase();
		for (int i = init_pause_phase; i <= phase_id; i++) {
			now_pause_time_list.at(i) += new_pause_time;
		}
	}

	///<summary>
	/// 任意の訪問POIのnow_pause_time_listを修正する．
	///</summary>
	template <typename POSITION_TYPE, typename TRAJECTORY_TYPE>
	void RevisablePauseMobileEntity<POSITION_TYPE, TRAJECTORY_TYPE>::revise_now_pause_time_of_any_visited_poi(double new_pause_time, int id)
	{

	}




	///<summary>
	/// 出発速度の修正．
	///</summary>
	template <typename POSITION_TYPE, typename TRAJECTORY_TYPE>
	void RevisablePauseMobileEntity<POSITION_TYPE, TRAJECTORY_TYPE>::revise_starting_speed(double speed)
	{
		visited_pois_info_list.at(visited_pois_info_list_id).starting_speed = speed;
	}

	///<summary>
	/// POI到着時の余り時間を修正する
	///</summary>
	template <typename POSITION_TYPE, typename TRAJECTORY_TYPE>
	void RevisablePauseMobileEntity<POSITION_TYPE, TRAJECTORY_TYPE>::revise_dest_rest_time(double dest_rest_time)
	{
		visited_pois_info_list.at(visited_pois_info_list_id).dest_rest_time = dest_rest_time;
	}

	///<summary>
	/// 任意のPOI到着時の余り時間を修正する
	///</summary>
	template <typename POSITION_TYPE, typename TRAJECTORY_TYPE>
	void RevisablePauseMobileEntity<POSITION_TYPE, TRAJECTORY_TYPE>::revise_dest_rest_time_of_any_visited_poi(double dest_rest_time, int id)
	{
		visited_pois_info_list.at(id).dest_rest_time = dest_rest_time;
	}


	///<summary>
	/// 現在の速度の修正．
	///</summary>
	template <typename POSITION_TYPE, typename TRAJECTORY_TYPE>
	void RevisablePauseMobileEntity<POSITION_TYPE, TRAJECTORY_TYPE>::revise_now_speed(int phase, double speed)
	{
		now_speed_list.at(phase) = speed;
	}


	template <typename POSITION_TYPE, typename TRAJECTORY_TYPE>
	std::shared_ptr<TRAJECTORY_TYPE> RevisablePauseMobileEntity<POSITION_TYPE, TRAJECTORY_TYPE>::get_trajectory()
	{
		return trajectory;
	}

	///<summary>
	/// pathを修正し，再計算を施す．
	///</summary>
	template <typename POSITION_TYPE, typename TRAJECTORY_TYPE>
	void RevisablePauseMobileEntity<POSITION_TYPE, TRAJECTORY_TYPE>::revise_path()
	{

	}

	///<summary>
	/// pathの再計算を行う．
	/// ただし，再計算不要な部分はコピーで対応
	///</summary>
	template <typename POSITION_TYPE, typename TRAJECTORY_TYPE>
	void RevisablePauseMobileEntity<POSITION_TYPE, TRAJECTORY_TYPE>::recalculation_path(const Graph::MapNodeIndicator& source, const Graph::MapNodeIndicator& destination, int phase_id)
	{
	}

}