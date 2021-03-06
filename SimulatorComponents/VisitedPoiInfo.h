#ifdef SIMULATORCOMPONENTS_EXPORTS
#define VISITED_POI_INFO_API __declspec(dllexport)
#else
#define VISITED_POI_INFO_API __declspec(dllimport)
#endif

#pragma once
#include "MobileEntity.h"
#include "RevisableTrajectory.h"
#include "KatoMethodRequirement.h"
#include "BasicPoi.h"

namespace Map {
	template <typename POSITION_TYPE = Geography::LatLng>
	class VISITED_POI_INFO_API VisitedPoiInfo
	{
	public:
		VisitedPoiInfo();
		virtual ~VisitedPoiInfo();

		//メンバ変数
		std::pair<Graph::MapNodeIndicator, POSITION_TYPE> visited_poi;//訪問POI
		std::vector<int> pause_phase;//訪問POIにおける停止phase
		int arrive_phase;//到着したphase
		double pause_time;//停止時間
		double starting_speed;//訪問POIの出発速度
		double rest_pause_time_when_departing;//出発時の余り停止時間
		double dest_rest_time;//POIに到着した時の余り時間
	};

	template class VisitedPoiInfo<Graph::Coordinate>;
	template class VisitedPoiInfo<Geography::LatLng>;
}