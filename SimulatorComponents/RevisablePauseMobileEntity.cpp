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
		std::invalid_argument("Not Found");
	}


	///<summary>
	/// now_pause_timをchange_time分増減させる．
	///</summary>
	template <typename POSITION_TYPE, typename TRAJECTORY_TYPE>
	void RevisablePauseMobileEntity<POSITION_TYPE, TRAJECTORY_TYPE>::add_now_pause_time(int now_phase, double change_time)
	{
		now_pause_time_list.at(now_phase) += change_time;
		if (now_pause_time_list.at(now_phase) < 0) std::invalid_argument("Now Pause Time is Negative Numer!!");
	}




	///<summary>
	/// now_pause_time_listをphase_id時まで修正する．
	///</summary>
	template <typename POSITION_TYPE, typename TRAJECTORY_TYPE>
	void RevisablePauseMobileEntity<POSITION_TYPE, TRAJECTORY_TYPE>::revise_now_pause_time(int phase_id, double new_pause_time)
	{
		//現在停止中かつnew_puase_timeがサービス利用間隔より小さかったら，現在停止POIの修正で，そうでない場合は，今向かっているPOIの停止時間の修正
		double service_interval = trajectory->find_phase_of_time(1) - trajectory->find_phase_of_time(0);
		int init_pause_phase = get_now_speed(phase_id) == 0.0 && get_now_pause_time(phase_id) < service_interval ? get_arrive_phase_using_pause_phase(phase_id) : get_arrive_phase();
		for (int i = init_pause_phase; i <= phase_id; i++) {
			now_pause_time_list.at(i) += new_pause_time;
		}
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