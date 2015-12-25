#ifdef SIMULATORCOMPONENTS_EXPORTS
#define PAUSE_MOBILE_ENTITY_API __declspec(dllexport)
#else
#define PAUSE_MOBILE_ENTITY_API __declspec(dllimport)
#endif

#pragma once
#include "MobileEntity.h"
#include "RevisableTrajectory.h"
#include "KatoMethodRequirement.h"
#include "BasicPoi.h"

namespace Entity
{
	typedef unsigned int entity_id;

	class VisitedPoiInfo
	{
	public:
		VisitedPoiInfo();
		virtual ~VisitedPoiInfo();
		
		//const int POI_NUM = 10;

		std::pair<Graph::MapNodeIndicator, Geography::LatLng> visited_poi;//訪問POI→set_visited_poi_of_phaseで設定
		std::vector<int> pause_phases;//訪問POIにおける停止phase→set_pause_phaseで設定
		int arrive_phase;//到着したphase→set_random_pause_timeとset_pause_timeで設定
		double pause_time;//停止時間→set_random_pause_timeとset_pause_timeで設定
		double starting_speed;//訪問POIの出発速度→set_starting_speed_at_poiで設定
		double rest_pause_time_when_departing;//出発時の余り停止時間→set_rest_pause_time_when_departingで設定
		double dest_rest_time;//POIに到着した時の余り時間→set_rest_timeで設定
	};
	
	///<summary>
	/// 停止時間を含んだ移動体を表すクラス
	/// ユーザ，ダミーを表すのに用いるクラス (MobileEntityから派生)
	///</summary>
	template <typename POSITION_TYPE = Geography::LatLng, typename TRAJECTORY_TYPE = Graph::RevisableTrajectory<POSITION_TYPE>>
	class PAUSE_MOBILE_ENTITY_API PauseMobileEntity : public MobileEntity<POSITION_TYPE, TRAJECTORY_TYPE>
	{
	protected:
		std::vector<VisitedPoiInfo> visited_pois_info_list;
		VisitedPoiInfo visited_poi_info;
		int visited_pois_info_list_id;
		
		std::vector<double> now_pause_time_list;//現在地点における残り停止時間
		//int pause_flag;

		std::vector<double> now_speed_list;
		
	public:
		PauseMobileEntity(entity_id id, std::shared_ptr<Time::TimeSlotManager const> timeslot);
		virtual ~PauseMobileEntity();
		
		//訪問POI_ID(visited_pois_info_list_id)に関するsetterとgetter
		int get_visited_pois_info_list_id();
		void clear_visited_pois_info_list_id();

		//訪問POI(visited_poi)に関するsetterとgetter
		void set_visited_poi_of_phase(int phase, const Graph::MapNodeIndicator& node_id, const Geography::LatLng& position);
		void set_crossing_position_of_phase(int phase, const Graph::MapNodeIndicator& node_id, Geography::LatLng position, const std::string& venue_name = "");//MobileEntityのオーバーライド
		std::pair<Graph::MapNodeIndicator, POSITION_TYPE> get_poi();
		std::pair<Graph::MapNodeIndicator, POSITION_TYPE> get_any_poi(int i);
		std::pair<Graph::MapNodeIndicator, POSITION_TYPE> get_next_poi();
		void clear_visited_poi_info();
		void sort_pois_order_by_arrive_phase();
		int get_visited_pois_num();

		//pause_phaseに関するsetterとgetter
		std::vector<int> get_pause_phases();
		std::vector<int> get_all_pause_phases();
		void set_pause_phases(int pause_phase);
		void set_pause_phases_using_arrive_phase(int arrive_phase, int pause_phase);


		//arrive_phaseに関するgetter.ただし，setはvisitedPOI登録時に設定を行うため不要．
		int get_arrive_phase();
		int get_arrive_phase_using_pause_phase(int pause_phase);
		int get_any_arrive_phase(int i);

		//訪問POI情報の停止時間(pause_time)に関するsetterとgetter
		double get_pause_time() const;
		double get_pause_time();
		double get_pause_time_using_arrive_phase(int arrive_phase);
		double get_any_poi_pause_time(int i);
		void set_pause_time(int phase, int pause_time);
		void set_pause_time(int phase, double pause_time);
		void set_pause_time_using_arrive_phase(int arrive_phase, double pause_time);
		void set_random_pause_time(int phase, int min, int max);
		void set_random_pause_time(int phase, double min, double max);
		void set_random_pause_time_using_arrive_phase(int arrive_phase, int phase, double min, double max);

		//starting_speedに関するsetterとgetter
		void set_starting_speed_at_poi(double speed);
		void set_starting_speed_at_poi_using_arrive_phase(int arrive_phase, double speed);
		void set_random_starting_speed_at_poi(double average_speed, double speed_range);
		void set_random_starting_speed_at_poi_using_arrive_phase(int arrive_phase, double average_speed, double speed_range);
		double get_starting_speed();
		double get_starting_speed_using_arrive_phase(int arrive_phase);
		double get_starting_speed_using_pause_phase(int pause_phase);

		//現在phaseにおける残り停止時間のsetterとgetter
		void set_rest_pause_time_when_departing(double rest_pause_time);
		void set_rest_pause_time_when_departing_using_arrive_phase(int arrive_phase, double rest_pause_time);
		double get_rest_pause_time_when_departing_using_pause_phase(int pause_phase);


		//到着時と出発時の余り時間のsetterとgetter
		void set_dest_rest_time(double dest_rest_time);
		void set_dest_rest_time_using_arrive_phase(int arrive_phase, double dest_rest_time);
	

		//現在情報のrest_pause_timeに関するsetterとgetter
		int get_now_pause_time(int now_phase) const;
		int get_now_pause_time(int now_phase);
		void set_now_pause_time(int now_phase, double time);


		//現在phaseにおける速度のsetterとgetter
		double get_now_speed(int phase) const;
		void set_now_speed(int phase, double speed);
		void set_random_now_speed(int phase, double average_speed, double speed_range);
				

		//停止POIにいるかどうかのチェック用
		bool check_pause_condition(int now_phase);
		void raise_flag();
		void take_down_flag();

		void increment_visited_pois_info_list_id();

		//trajectory取得用
		std::shared_ptr<TRAJECTORY_TYPE> get_trajectory();
		
		
		std::vector<int> find_cross_not_set_phases_of_poi() const;
		int randomly_pick_cross_not_set_phase_of_poi() const;

	};

	//明示的特殊化
	template class PauseMobileEntity<Graph::Coordinate>;
	template class PauseMobileEntity<Geography::LatLng>;
}