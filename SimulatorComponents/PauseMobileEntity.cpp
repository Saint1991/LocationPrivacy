#include "stdafx.h"
#include "PauseMobileEntity.h"


namespace Entity
{

	///<summary>
	/// コンストラクタ
	///</summary>
	VisitedPoiInfo::VisitedPoiInfo()
	{
	}

	///<summary>
	/// デストラクタ
	///</summary>
	VisitedPoiInfo::~VisitedPoiInfo()
	{
	}

	///<summary>
	/// コンストラクタ
	///</summary>
	template <typename POSITION_TYPE, typename TRAJECTORY_TYPE>
	PauseMobileEntity<POSITION_TYPE, TRAJECTORY_TYPE>::PauseMobileEntity(entity_id id, std::shared_ptr<Time::TimeSlotManager const> timeslot)
		: MobileEntity<POSITION_TYPE, TRAJECTORY_TYPE>(id, timeslot),
		now_pause_time_list(std::vector<double>(timeslot->phase_count(), 0)),
		now_speed_list(std::vector<double>(timeslot->phase_count(), 0)),
		visited_pois_info_list_id(0)
	{
	}

	///<summary>
	/// デストラクタ
	///</summary>
	template <typename POSITION_TYPE, typename TRAJECTORY_TYPE>
	PauseMobileEntity<POSITION_TYPE, TRAJECTORY_TYPE>::~PauseMobileEntity()
	{
	}

	///<summary>
	/// 訪問POI情報をk管理するIDを取得する
	///</summary>
	template <typename POSITION_TYPE, typename TRAJECTORY_TYPE>
	int PauseMobileEntity<POSITION_TYPE, TRAJECTORY_TYPE>::get_visited_pois_info_list_id()
	{
		return visited_pois_info_list_id;
	}

	///<summary>
	/// 訪問POI情報を管理するIDをリセットする
	///</summary>
	template <typename POSITION_TYPE, typename TRAJECTORY_TYPE>
	void PauseMobileEntity<POSITION_TYPE, TRAJECTORY_TYPE>::clear_visited_pois_info_list_id()
	{
		visited_pois_info_list_id = 0;
	}


	///<summary>
	/// 訪問POI情報をセットする．訪問地点の登録の時はこちらを使う
	/// arrive_phaseも記録.ただし，pause_phaseは登録しない．pause_phaseはs停止時間登録の際に行う．
	/// visited_poi_infoをpush_backする前に，pause_phasesはクリアすること
	/// push_back後は，arrive_phaseを用いて，ソートを行う．
	///</summary>
	template <typename POSITION_TYPE, typename TRAJECTORY_TYPE>
	void PauseMobileEntity<POSITION_TYPE, TRAJECTORY_TYPE>::set_visited_poi_of_phase(int phase, const Graph::MapNodeIndicator& node_id, const Geography::LatLng& position)
	{
 		trajectory->set_position_of_phase(phase, node_id, position);

		//もし，開始点以外の挿入の時は，pause_phasesをクリアする．
		if (!visited_pois_info_list.empty()) {
			clear_visited_poi_info();
		}

		visited_poi_info.visited_poi = std::make_pair(node_id, position);
		visited_poi_info.arrive_phase = phase;
		visited_pois_info_list.push_back(visited_poi_info);

		//sort後，arrive_phase順にソートしておく．
		sort_pois_order_by_arrive_phase();
	}

	///<summary>
	/// 共有地点設定時の訪問POI情報をセットする
	/// MobileEntityをオーバーライドした関数
	///</summary>
	template <typename POSITION_TYPE, typename TRAJECTORY_TYPE>
	void PauseMobileEntity<POSITION_TYPE, TRAJECTORY_TYPE>::set_crossing_position_of_phase(int phase, const Graph::MapNodeIndicator& node_id, Geography::LatLng position, const std::string& venue_name)
	{
		register_as_cross_position(phase);
		set_visited_poi_of_phase(phase, node_id, position);
	}

	///<summary>
	/// 現在停止中or現在向かっているPOIを取得
	///</summary>
	template <typename POSITION_TYPE, typename TRAJECTORY_TYPE>
	std::pair<Graph::MapNodeIndicator, POSITION_TYPE> PauseMobileEntity<POSITION_TYPE, TRAJECTORY_TYPE>::get_poi()
	{
		return visited_pois_info_list.at(visited_pois_info_list_id).visited_poi;
	}

	///<summary>
	/// i番目に訪問予定のPOIを取得
	/// もしi番目の予定訪問POIがなかった場合は，最終フェーズのpositionを返り値とする．
	///</summary>
	template <typename POSITION_TYPE, typename TRAJECTORY_TYPE>
	std::pair<Graph::MapNodeIndicator, POSITION_TYPE> PauseMobileEntity<POSITION_TYPE, TRAJECTORY_TYPE>::get_any_poi(int i)
	{
		int last_phase = trajectory->phase_count() - 1;
		return i <= get_visited_pois_num() 
			? visited_pois_info_list.at(i).visited_poi : std::make_pair(trajectory->read_node_pos_info_of_phase(last_phase).first, *trajectory->read_node_pos_info_of_phase(last_phase).second);
	}

	///<summary>
	/// 次に向かうPOIの情報を取得
	/// もし次のの予定訪問POIがなかった場合は，最終フェーズのpositionを返り値とする．
	///</summary>
	template <typename POSITION_TYPE, typename TRAJECTORY_TYPE>
	std::pair<Graph::MapNodeIndicator, POSITION_TYPE> PauseMobileEntity<POSITION_TYPE, TRAJECTORY_TYPE>::get_next_poi()
	{
		int last_phase = trajectory->phase_count() - 1;
		return (visited_pois_info_list_id + 1) <= get_visited_pois_num()
			? visited_pois_info_list.at(visited_pois_info_list_id + 1).visited_poi : std::make_pair(trajectory->read_node_pos_info_of_phase(last_phase).first, *trajectory->read_node_pos_info_of_phase(last_phase).second);
	}
	

	///<summary>
	/// visited_poi_infoの情報をクリアする
	///</summary>
	template <typename POSITION_TYPE, typename TRAJECTORY_TYPE>
	void PauseMobileEntity<POSITION_TYPE, TRAJECTORY_TYPE>::clear_visited_poi_info()
	{
		while (!visited_poi_info.pause_phases.empty()) {
			visited_poi_info.pause_phases.pop_back();
		}
		visited_poi_info.arrive_phase = 0;
		visited_poi_info.dest_rest_time = 0.0;
		visited_poi_info.pause_time = 0.0;
		visited_poi_info.rest_pause_time_when_departing = 0.0;
		visited_poi_info.starting_speed = 0.0;

	}

	///<summary>
	/// visited_pois_info_listをarrive_phaseの順にソートする
	///</summary>
	template <typename POSITION_TYPE, typename TRAJECTORY_TYPE>
	void PauseMobileEntity<POSITION_TYPE, TRAJECTORY_TYPE>::sort_pois_order_by_arrive_phase()
	{
		std::sort(visited_pois_info_list.begin(), visited_pois_info_list.end(),
			[](VisitedPoiInfo& poi1, VisitedPoiInfo& poi2) {
			return poi1.arrive_phase < poi2.arrive_phase;
		});
	}

	///<summary>
	/// 予定訪問POIの数を求める
	///</summary>
	template <typename POSITION_TYPE, typename TRAJECTORY_TYPE>
	int PauseMobileEntity<POSITION_TYPE, TRAJECTORY_TYPE>::get_visited_pois_num()
	{
		return visited_pois_info_list.size();
	}

	///<summary>
	/// 次に訪問予定の訪問POIの停止フェーズを求める．
	/// 停止中の場合は，現在phaseを返し，移動中の場合は，向かっている訪問地点の予定している到着時間に値するphaseを返す
	///</summary>
	template <typename POSITION_TYPE, typename TRAJECTORY_TYPE>
	std::vector<int> PauseMobileEntity<POSITION_TYPE, TRAJECTORY_TYPE>::get_pause_phases()
	{
		return visited_pois_info_list.at(visited_pois_info_list_id).pause_phases;
	}

	///<summary>
	/// 全ての訪問予定POIの停止phaseを求める．
	///</summary>
	template <typename POSITION_TYPE, typename TRAJECTORY_TYPE>
	std::vector<int> PauseMobileEntity<POSITION_TYPE, TRAJECTORY_TYPE>::get_all_pause_phases()
	{
		std::vector<int> all_pause_phases;

		for (std::vector<VisitedPoiInfo>::iterator iter = visited_pois_info_list.begin(); iter != visited_pois_info_list.end(); iter++) {
			for (std::vector<int>::iterator iter2 = iter->pause_phases.begin(); iter2 != iter->pause_phases.end(); iter2++) {
				all_pause_phases.push_back(*iter2);
			}
		}

		return all_pause_phases;
	}

	///<summary>
	/// 次に訪問予定の停止POIの到着するphaseをsetする．
	///</summary>
	template <typename POSITION_TYPE, typename TRAJECTORY_TYPE>
	void PauseMobileEntity<POSITION_TYPE, TRAJECTORY_TYPE>::set_pause_phases(int pause_phase)
	{
		visited_poi_info.pause_phases.push_back(pause_phase);
		visited_pois_info_list.at(visited_pois_info_list_id).pause_phases.push_back(pause_phase);
	}

	///<summary>
	/// 次に訪問予定の停止POIの到着するphaseをsetする．
	///</summary>
	template <typename POSITION_TYPE, typename TRAJECTORY_TYPE>
	void PauseMobileEntity<POSITION_TYPE, TRAJECTORY_TYPE>::set_pause_phases_using_arrive_phase(int arrive_phase, int pause_phase) {
		for (std::vector<VisitedPoiInfo>::iterator iter = visited_pois_info_list.begin(); iter != visited_pois_info_list.end(); iter++) {
			if (iter->arrive_phase == arrive_phase) {
				iter->pause_phases.push_back(pause_phase);
				visited_poi_info.pause_phases.push_back(pause_phase);
				return;
			}
		}
		throw std::invalid_argument("Not Found");
	}

	///<summary>
	/// 次に訪問予定の停止POIの到着するphaseを求める．
	/// 停止中の場合は，現在phaseを返し，移動中の場合は，向かっている訪問地点の予定している到着時間に値するphaseを返す
	///</summary>
	template <typename POSITION_TYPE, typename TRAJECTORY_TYPE>
	int PauseMobileEntity<POSITION_TYPE, TRAJECTORY_TYPE>::get_arrive_phase()
	{
		return visited_pois_info_list.at(visited_pois_info_list_id).arrive_phase;
	}

	///<summary>
	/// 次に訪問予定の停止POIの到着するphaseをpause_phaseを利用して求める．
	/// 停止中の場合は，現在phaseを返し，移動中の場合は，向かっている訪問地点の予定している到着時間に値するphaseを返す
	///</summary>
	template <typename POSITION_TYPE, typename TRAJECTORY_TYPE>
	int PauseMobileEntity<POSITION_TYPE, TRAJECTORY_TYPE>::get_arrive_phase_using_pause_phase(int pause_phase)
	{
		for (std::vector<VisitedPoiInfo>::iterator iter = visited_pois_info_list.begin(); iter != visited_pois_info_list.end(); iter++) {
			for (std::vector<int>::iterator iter2 = iter->pause_phases.begin(); iter2 != iter->pause_phases.end(); iter2++) {
				if (*iter2 == pause_phase) return iter->arrive_phase;
			}
		}
		throw std::invalid_argument("Not Found");
	}

	///<summary>
	/// i番目に訪問予定の停止POIの到着するphaseを求める．
	///</summary>
	template <typename POSITION_TYPE, typename TRAJECTORY_TYPE>
	int PauseMobileEntity<POSITION_TYPE, TRAJECTORY_TYPE>::get_any_arrive_phase(int i)
	{
		return visited_pois_info_list.at(i).arrive_phase;
	}


	///<summary>
	/// 訪問POIの停止時間を変更不可能な状態で求める
	///</summary>
	template <typename POSITION_TYPE, typename TRAJECTORY_TYPE>
	double PauseMobileEntity<POSITION_TYPE, TRAJECTORY_TYPE>::get_pause_time() const
	{
		return visited_pois_info_list.at(visited_pois_info_list_id).pause_time;
	}

	///<summary>
	/// 訪問POIの停止時間を変更可能な状態で求める
	///</summary>
	template <typename POSITION_TYPE, typename TRAJECTORY_TYPE>
	double PauseMobileEntity<POSITION_TYPE, TRAJECTORY_TYPE>::get_pause_time()
	{
		return visited_pois_info_list.at(visited_pois_info_list_id).pause_time;
	}


	///<summary>
	/// 訪問POIの停止時間を変更可能な状態で求める
	///</summary>
	template <typename POSITION_TYPE, typename TRAJECTORY_TYPE>
	double PauseMobileEntity<POSITION_TYPE, TRAJECTORY_TYPE>::get_pause_time_using_arrive_phase(int arrive_phase)
	{
		for (std::vector<VisitedPoiInfo>::iterator iter = visited_pois_info_list.begin(); iter != visited_pois_info_list.end(); iter++) {
			if (iter->arrive_phase == arrive_phase) {
				return iter->pause_time;
			}
		}
		throw std::invalid_argument("Not Found");
	}

	///<summary>
	/// i番目に訪問予定の停止時間を変更可能な状態で求める
	///</summary>
	template <typename POSITION_TYPE, typename TRAJECTORY_TYPE>
	double PauseMobileEntity<POSITION_TYPE, TRAJECTORY_TYPE>::get_any_poi_pause_time(int i)
	{
		return i <= get_visited_pois_num() ? visited_pois_info_list.at(i).pause_time : 0;
	}

	///<summary>
	/// 訪問POI情報に，int型の停止時間と到着phaseをsetする
	///</summary>
	template <typename POSITION_TYPE, typename TRAJECTORY_TYPE>
	void PauseMobileEntity<POSITION_TYPE, TRAJECTORY_TYPE>::set_pause_time(int phase, int pause_time)
	{
		visited_poi_info.pause_phases.push_back(phase);
		visited_poi_info.pause_time = pause_time;

		visited_pois_info_list.at(visited_pois_info_list_id).pause_phases.push_back(phase);
		visited_pois_info_list.at(visited_pois_info_list_id).pause_time = pause_time;
	}

	///<summary>
	/// 訪問POI情報に，double型の停止時間と到着phaseをsetする
	///</summary>
	template <typename POSITION_TYPE, typename TRAJECTORY_TYPE>
	void PauseMobileEntity<POSITION_TYPE, TRAJECTORY_TYPE>::set_pause_time(int phase, double pause_time)
	{
		visited_poi_info.pause_phases.push_back(phase);
		visited_poi_info.pause_time = pause_time;

		visited_pois_info_list.at(visited_pois_info_list_id).pause_phases.push_back(phase);
		visited_pois_info_list.at(visited_pois_info_list_id).pause_time = pause_time;
	}

	///<summary>
	/// 訪問POI情報に，double型の停止時間と到着phaseをsetする
	///</summary>
	template <typename POSITION_TYPE, typename TRAJECTORY_TYPE>
	void PauseMobileEntity<POSITION_TYPE, TRAJECTORY_TYPE>::set_pause_time_using_arrive_phase(int arrive_phase, double pause_time)
	{
		for (std::vector<VisitedPoiInfo>::iterator iter = visited_pois_info_list.begin(); iter != visited_pois_info_list.end(); iter++) {
			if (iter->arrive_phase == arrive_phase) {
				visited_poi_info.pause_phases.push_back(arrive_phase);
				visited_poi_info.pause_time = pause_time;

				iter->pause_phases.push_back(arrive_phase);
				iter->pause_time = pause_time;
				return;
			}
		}
		throw std::invalid_argument("Not Found");
	}

	///<summary>
	/// 訪問POI情報に,int型のランダムな停止時間と到着phaseをsetする
	///</summary>
	template <typename POSITION_TYPE, typename TRAJECTORY_TYPE>
	void PauseMobileEntity<POSITION_TYPE, TRAJECTORY_TYPE>::set_random_pause_time(int phase, int min, int max)
	{
		Math::Probability generator;
		int pause_time = generator.uniform_distribution(min, max);

		visited_poi_info.pause_phases.push_back(phase);
		visited_poi_info.pause_time = pause_time;

		visited_pois_info_list.at(visited_pois_info_list_id).pause_phases.push_back(phase);
		visited_pois_info_list.at(visited_pois_info_list_id).pause_time = pause_time;
	}

	///<summary>
	/// ランダムな値をdouble型で停止時間としてsetする
	/// 最初のpause_phaseもpause_phasesにpush_backすることに注意
	///</summary>
	template <typename POSITION_TYPE, typename TRAJECTORY_TYPE>
	void PauseMobileEntity<POSITION_TYPE, TRAJECTORY_TYPE>::set_random_pause_time(int phase, double min, double max)
	{
		Math::Probability generator;
		double pause_time = generator.uniform_distribution(min, max);

		visited_poi_info.pause_phases.push_back(phase);
		visited_poi_info.pause_time = pause_time;

		visited_pois_info_list.at(visited_pois_info_list_id).pause_phases.push_back(phase);
		visited_pois_info_list.at(visited_pois_info_list_id).pause_time = pause_time;
	}

	///<summary>
	/// ランダムな値をdouble型で停止時間としてsetする
	/// 最初のpause_phaseもpause_phasesにpush_backすることに注意
	///</summary>
	template <typename POSITION_TYPE, typename TRAJECTORY_TYPE>
	void PauseMobileEntity<POSITION_TYPE, TRAJECTORY_TYPE>::set_random_pause_time_using_arrive_phase(int arrive_phase, int phase, double min, double max) {

		Math::Probability generator;
		double pause_time = generator.uniform_distribution(min, max);

		for (std::vector<VisitedPoiInfo>::iterator iter = visited_pois_info_list.begin(); iter != visited_pois_info_list.end(); iter++) {
			if (iter->arrive_phase == arrive_phase) {
				visited_poi_info.pause_phases.push_back(phase);
				visited_poi_info.pause_time = pause_time;

				iter->pause_phases.push_back(phase);
				iter->pause_time = pause_time;
				return;
			}
		}
		throw std::invalid_argument("Not Found");
	}

	///<summary>
	/// 訪問POI情報にspeedをsetする
	///</summary>
	template <typename POSITION_TYPE, typename TRAJECTORY_TYPE>
	void PauseMobileEntity<POSITION_TYPE, TRAJECTORY_TYPE>::set_starting_speed_at_poi(double speed)
	{
		visited_poi_info.starting_speed = speed;
		visited_pois_info_list.at(visited_pois_info_list_id).starting_speed = speed;
	}

	///<summary>
	/// 訪問POI情報にspeedをsetする
	///</summary>
	template <typename POSITION_TYPE, typename TRAJECTORY_TYPE>
	void PauseMobileEntity<POSITION_TYPE, TRAJECTORY_TYPE>::set_starting_speed_at_poi_using_arrive_phase(int arrive_phase, double speed)
	{
		for (std::vector<VisitedPoiInfo>::iterator iter = visited_pois_info_list.begin(); iter != visited_pois_info_list.end(); iter++) {
			if (iter->arrive_phase == arrive_phase) {
				iter->starting_speed = speed;
				visited_poi_info.starting_speed = speed;
				return;
			}
		}
		throw std::invalid_argument("Not Found");
	}

	///<summary>
	/// 訪問POI情報にspeedをrandamにsetする
	///</summary>
	template <typename POSITION_TYPE, typename TRAJECTORY_TYPE>
	void PauseMobileEntity<POSITION_TYPE, TRAJECTORY_TYPE>::set_random_starting_speed_at_poi(double average_speed, double speed_range)
	{
		Math::Probability generator;
		double min = average_speed - 0.5* speed_range;
		double max = average_speed + 0.5* speed_range;
		double entity_speed = generator.uniform_distribution(min, max);

		visited_poi_info.starting_speed = entity_speed;
		visited_pois_info_list.at(visited_pois_info_list_id).starting_speed = entity_speed;
	}

	///<summary>
	/// 訪問POI情報にspeedをrandamにsetする
	///</summary>
	template <typename POSITION_TYPE, typename TRAJECTORY_TYPE>
	void PauseMobileEntity<POSITION_TYPE, TRAJECTORY_TYPE>::set_random_starting_speed_at_poi_using_arrive_phase(int arrive_phase, double average_speed, double speed_range)
	{
		Math::Probability generator;
		double min = average_speed - 0.5* speed_range;
		double max = average_speed + 0.5* speed_range;
		double entity_speed = generator.uniform_distribution(min, max);
		
		for (std::vector<VisitedPoiInfo>::iterator iter = visited_pois_info_list.begin(); iter != visited_pois_info_list.end(); iter++) {
			if (iter->arrive_phase == arrive_phase) {
				iter->starting_speed = entity_speed;
				return;
			}
		}
		throw std::invalid_argument("Not Found");
		
	}


	///<summary>
	/// POIを出発時の速度をvisited_pois_info_list_idを用いて取得する
	///</summary>
	template <typename POSITION_TYPE, typename TRAJECTORY_TYPE>
	double PauseMobileEntity<POSITION_TYPE, TRAJECTORY_TYPE>::get_starting_speed()
	{
		return visited_pois_info_list.at(visited_pois_info_list_id).starting_speed;
	}

	///<summary>
	/// POIを出発時の速度をarrive_phaseを用いて取得する
	///</summary>
	template <typename POSITION_TYPE, typename TRAJECTORY_TYPE>
	double PauseMobileEntity<POSITION_TYPE, TRAJECTORY_TYPE>::get_starting_speed_using_arrive_phase(int arrive_phase)
	{
		for (std::vector<VisitedPoiInfo>::iterator iter = visited_pois_info_list.begin(); iter != visited_pois_info_list.end(); iter++) {
			if (iter->arrive_phase == arrive_phase) return iter->starting_speed;
		}
		throw std::invalid_argument("Not Found");
	}

	///<summary>
	/// POIを出発時の速度をarrive_phaseを用いて取得する
	///</summary>
	template <typename POSITION_TYPE, typename TRAJECTORY_TYPE>
	double PauseMobileEntity<POSITION_TYPE, TRAJECTORY_TYPE>::get_starting_speed_using_pause_phase(int pause_phase)
	{
		for (std::vector<VisitedPoiInfo>::iterator iter = visited_pois_info_list.begin(); iter != visited_pois_info_list.end(); iter++) {
			for (std::vector<int>::iterator iter2 = iter->pause_phases.begin(); iter2 != iter->pause_phases.end(); iter2++) {
				if (*iter2 == pause_phase) return iter->starting_speed;
			}
		}
		throw std::invalid_argument("Not Found");
	}

	///<summary>
	/// POI出発時の余り時間を登録
	///</summary>
	template <typename POSITION_TYPE, typename TRAJECTORY_TYPE>
	void PauseMobileEntity<POSITION_TYPE, TRAJECTORY_TYPE>::set_rest_pause_time_when_departing(double rest_pause_time)
	{
		visited_poi_info.rest_pause_time_when_departing = rest_pause_time;
		visited_pois_info_list.at(visited_pois_info_list_id).rest_pause_time_when_departing = rest_pause_time;
	}

	///<summary>
	/// POI出発時の余り時間を,arrive_phaseを用いて登録
	///</summary>
	template <typename POSITION_TYPE, typename TRAJECTORY_TYPE>
	void PauseMobileEntity<POSITION_TYPE, TRAJECTORY_TYPE>::set_rest_pause_time_when_departing_using_arrive_phase(int arrive_phase, double rest_pause_time)
	{
		for (std::vector<VisitedPoiInfo>::iterator iter = visited_pois_info_list.begin(); iter != visited_pois_info_list.end(); iter++) {
			if (iter->arrive_phase == arrive_phase) {
				visited_poi_info.rest_pause_time_when_departing = rest_pause_time;
				iter->rest_pause_time_when_departing = rest_pause_time;
				return;
			}
		}
		throw std::invalid_argument("Not Found");
	}

	///<summary>
	/// POI出発時の余り時間を,pause_phaseを用いて登録
	///</summary>
	template <typename POSITION_TYPE, typename TRAJECTORY_TYPE>
	double PauseMobileEntity<POSITION_TYPE, TRAJECTORY_TYPE>::get_rest_pause_time_when_departing_using_pause_phase(int pause_phase)
	{
		for (std::vector<VisitedPoiInfo>::iterator iter = visited_pois_info_list.begin(); iter != visited_pois_info_list.end(); iter++) {
			for (std::vector<int>::iterator iter2 = iter->pause_phases.begin(); iter2 != iter->pause_phases.end(); iter2++) {
				if (*iter2 == pause_phase) return iter->rest_pause_time_when_departing;
			}
		}
		throw std::invalid_argument("Not Found");
	}
	

	///<summary>
	/// POI到着時の余り時間を登録
	///</summary>
	template <typename POSITION_TYPE, typename TRAJECTORY_TYPE>
	void PauseMobileEntity<POSITION_TYPE, TRAJECTORY_TYPE>::set_dest_rest_time(double dest_rest_time)
	{
		visited_poi_info.dest_rest_time = dest_rest_time;
		visited_pois_info_list.at(visited_pois_info_list_id).dest_rest_time = dest_rest_time;
	}


	///<summary>
	/// POI到着時の余り時間をarrive_phaseを用いて登録
	///</summary>
	template <typename POSITION_TYPE, typename TRAJECTORY_TYPE>
	void PauseMobileEntity<POSITION_TYPE, TRAJECTORY_TYPE>::set_dest_rest_time_using_arrive_phase(int arrive_phase, double dest_rest_time)
	{
		for (std::vector<VisitedPoiInfo>::iterator iter = visited_pois_info_list.begin(); iter != visited_pois_info_list.end(); iter++) {
			if (iter->arrive_phase == arrive_phase) {
				visited_poi_info.dest_rest_time = dest_rest_time;
				iter->dest_rest_time = dest_rest_time;
				return;
			}
		}
		throw std::invalid_argument("Not Found");
	}


	///<summary>
	/// 現在phaseの残り停止時間を，変更不可能な状態で求める．
	///</summary>
	template <typename POSITION_TYPE, typename TRAJECTORY_TYPE>
	int PauseMobileEntity<POSITION_TYPE, TRAJECTORY_TYPE>::get_now_pause_time(int now_phase) const
	{
		return now_pause_time_list.at(now_phase);
	}

	///<summary>
	/// 現在phaseの残り停止時間を，変更可能な状態で求める．
	///</summary>
	template <typename POSITION_TYPE, typename TRAJECTORY_TYPE>
	int PauseMobileEntity<POSITION_TYPE, TRAJECTORY_TYPE>::get_now_pause_time(int now_phase)
	{
		return now_pause_time_list.at(now_phase);
	}

	///<summary>
	/// 現在phaseの残り停止時間を，setする．
	/// phaseにはpush_backしないので注意
	///</summary>
	template <typename POSITION_TYPE, typename TRAJECTORY_TYPE>
	void PauseMobileEntity<POSITION_TYPE, TRAJECTORY_TYPE>::set_now_pause_time(int now_phase, double time)
	{
		now_pause_time_list.at(now_phase) = time;
	}




	///<summary>
	/// 現在の移動速度を求める
	///</summary>
	template <typename POSITION_TYPE, typename TRAJECTORY_TYPE>
	double PauseMobileEntity<POSITION_TYPE, TRAJECTORY_TYPE>::get_now_speed(int phase) const
	{
		return now_speed_list.at(phase);
	}

	///<summary>
	/// 特定の値の移動速度を現在速度にsetする
	///</summary>
	template <typename POSITION_TYPE, typename TRAJECTORY_TYPE>
	void PauseMobileEntity<POSITION_TYPE, TRAJECTORY_TYPE>::set_now_speed(int phase, double speed)
	{
		now_speed_list.at(phase) = speed;
	}


	///<summary>
	/// ランダムな値の移動速度をsetする
	///</summary>
	template <typename POSITION_TYPE, typename TRAJECTORY_TYPE>
	void PauseMobileEntity<POSITION_TYPE, TRAJECTORY_TYPE>::set_random_now_speed(int phase, double average_speed, double speed_range)
	{
		Math::Probability generator;
		double min = average_speed - 0.5* speed_range;
		double max = average_speed + 0.5* speed_range;
		double entity_speed = generator.uniform_distribution(min, max);

		now_speed_list.at(phase) = entity_speed;

	}


	///<summary>
	/// 停止しているかどうか確認する
	/// 現在速度が0の場合停止と判断
	///</summary>
	template <typename POSITION_TYPE, typename TRAJECTORY_TYPE>
	bool PauseMobileEntity<POSITION_TYPE, TRAJECTORY_TYPE>::check_pause_condition(int now_phase)
	{
		return get_now_speed(now_phase) == 0.0 ? true : false;
	}

	///<summary>
	/// visited_pois_info_list_idをインクリメントする
	///</summary>
	template <typename POSITION_TYPE, typename TRAJECTORY_TYPE>
	void PauseMobileEntity<POSITION_TYPE, TRAJECTORY_TYPE>::increment_visited_pois_info_list_id()
	{
		visited_pois_info_list_id++;
	}


	///<summary>
	/// トラジェクトリデータを変更可能な状態で取得する
	///</summary>
	template <typename POSITION_TYPE, typename TRAJECTORY_TYPE>
	std::shared_ptr<TRAJECTORY_TYPE> PauseMobileEntity<POSITION_TYPE, TRAJECTORY_TYPE>::get_trajectory()
	{
		return trajectory;
	}


	///<summary>
	/// 交差が設定されていないPhaseを全て取得する
	///</summary>
	template <typename POSITION_TYPE, typename TRAJECTORY_TYPE>
	std::vector<int> PauseMobileEntity<POSITION_TYPE, TRAJECTORY_TYPE>::find_cross_not_set_phases_of_poi() const
	{
		std::vector<int> ret;
		for (int phase = 0; phase < cross_flg->size(); phase++) {
			node_pos_info info = trajectory->read_node_pos_info_of_phase(phase);
			if (!cross_flg->at(phase) && info.first.type() == Graph::NodeType::POI) ret.push_back(phase);
		}
		return ret;
	}

	///<summary>
	/// 交差が設定されていない時刻を一つランダムに取得する
	/// 設定されていないphaseが存在しない場合はINVALIDを返す
	///</summary>
	template <typename POSITION_TYPE, typename TRAJECTORY_TYPE>
	int PauseMobileEntity<POSITION_TYPE, TRAJECTORY_TYPE>::randomly_pick_cross_not_set_phase_of_poi() const
	{
		Math::Probability generator;
		std::vector<int> not_set_phases = find_cross_not_set_phases_of_poi();
		if (not_set_phases.size() == 0) return INVALID;
		return not_set_phases.at(generator.uniform_distribution(0, not_set_phases.size() - 1));
	}

}
