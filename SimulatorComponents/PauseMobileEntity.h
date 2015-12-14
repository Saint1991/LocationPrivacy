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
		
		std::pair<Graph::MapNodeIndicator, Geography::LatLng> visited_poi;//訪問POI
		std::vector<int> pause_phase;//訪問POIにおける停止phase
		int arrive_phase;//到着したphase
		double pause_time;//停止時間
		double starting_speed;//訪問POIの出発速度
		double rest_pause_time_when_departing;//出発時の余り停止時間
		double dest_rest_time;//POIに到着した時の余り時間
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
		int visited_pois_info_list_id;

		std::vector<double> rest_pause_time_list;//現在地点における残り停止時間
		int pause_flag;

		std::vector<double> speed_list;
		
	public:
		PauseMobileEntity(entity_id id, std::shared_ptr<Time::TimeSlotManager const> timeslot);
		virtual ~PauseMobileEntity();
		
		//訪問POI情報の停止時間に関するsetterとgetter
		int get_pause_time() const;
		int get_pause_time();
		void set_pause_time(int phase, int pause_time);
		void set_pause_time(int phase, double pause_time);
		void set_random_pause_time(int phase, int min, int max);
		void set_random_pause_time(int phase, double min, double max);
		int get_pause_phase(int phase);
				

		//現在phaseにおける残り停止時間のsetterとgetter
		int get_rest_pause_time(int now_phase) const;
		int get_rest_pause_time(int now_phase);
		void set_rest_pause_time(int now_phase, double time);
		
		//現在phaseにおける速度のsetterとgetter
		double get_speed(int phase) const;
		void set_speed(int phase, double speed);
		void set_random_speed(int phase, double average_speed, double speed_range);
		void set_starting_speed_at_poi(double speed);
		double get_starting_speed(int visited_poi_info_id);


		//到着時と出発時の余り時間のsetterとgetter
		void set_dest_rest_time(double dest_rest_time);
		void set_rest_pause_time_when_departing(double rest_pause_time);
		
		//訪問POIに関するsetterとgetter
		void set_visited_poi_of_phase(int phase, const Graph::MapNodeIndicator& node_id, const Geography::LatLng& position);
		void set_crossing_position_of_phase(int phase, const Graph::MapNodeIndicator& node_id, Geography::LatLng position, const std::string& venue_name = "");//MobileEntityのオーバーライド
		std::pair<Graph::MapNodeIndicator, Geography::LatLng> get_poi();
		VisitedPoiInfo get_next_poi_info();

		//停止POIにいるかどうかのチェック用
		bool check_pause_flag();
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