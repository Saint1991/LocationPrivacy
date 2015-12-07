#ifdef SIMULATORCOMPONENTS_EXPORTS
#define PAUSE_MOBILE_ENTITY_API __declspec(dllexport)
#else
#define PAUSE_MOBILE_ENTITY_API __declspec(dllimport)
#endif

#pragma once
#include "MobileEntity.h"
#include "KatoMethodRequirement.h"
#include "BasicPoi.h"

namespace Entity
{

	typedef unsigned int entity_id;

	///<summary>
	/// 停止時間を含んだ移動体を表すクラス
	/// ユーザ，ダミーを表すのに用いるクラス (MobileEntityから派生)
	///</summary>
	template <typename POSITION_TYPE = Geography::LatLng>
	class PAUSE_MOBILE_ENTITY_API PauseMobileEntity : public MobileEntity<POSITION_TYPE>
	{
	protected:
		/*
		struct PAUSE_MOBILE_ENTITY_API VisitedPoiInfo
		{
			std::shared_ptr<Map::BasicPoi const> visited_poi;//訪問POI
			std::pair<std::pair<Graph::MapNodeIndicator, Geography::LatLng>, double(trajectory)> next_visited_poi_info;//次に訪問予定のPOIとその経路
			std::vector<int> pause_phase;//訪問POIにおける停止phase
			double starting_speed;//訪問POIの出発速度
			double rest_pause_time_when_departing;//出発した時の余った停止時間
		};
		*/
		//std::vector<VisitedPoiInfo> visited_poi_info;

		std::vector<double> pause_time_list;
		std::vector<double> speed_list;
		
		int visited_poi_id;

		std::vector<std::shared_ptr<Map::BasicPoi const>> visited_poi_list;

	public:
		PauseMobileEntity(entity_id id, std::shared_ptr<Time::TimeSlotManager const> timeslot);
		virtual ~PauseMobileEntity();

		int get_pause_time(int phase) const;
		void set_pause_time(int phase, int pause_time);
		void set_pause_time(int phase, double pause_time);
		void set_random_pause_time(int phase, int min, int max);
		void set_random_pause_time(int phase, double min, double max);

		double get_speed(int phase) const;
		void set_speed(int phase, double speed);
		void set_random_speed(int phase, double average_speed, double speed_range);
		
		std::vector<int> find_cross_not_set_phases_of_poi() const;
		int randomly_pick_cross_not_set_phase_of_poi() const;

		std::vector<std::shared_ptr<Map::BasicPoi const>> get_visited_poi_list();
		void set_visited_poi_list(std::shared_ptr<Map::BasicPoi const> visited_poi);

		//std::vector<VisitedPoiInfo> get_visited_poi_info();
		//void set_visited_POI_of_phase(int phase, std::shared_ptr<Map::BasicPoi const>& visited_poi);
		//set_visited_POI_of_phaseはMapNodeIndicatorタイプでもいいかも
		
		//std::shared_ptr<TRAJECTORY_TYPE> get_trajectory();

	};

	//明示的特殊化
	template class PauseMobileEntity<Graph::Coordinate>;
	template class PauseMobileEntity<Geography::LatLng>;
}