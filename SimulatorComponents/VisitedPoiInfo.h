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

		//ƒƒ“ƒo•Ï”
		std::pair<Graph::MapNodeIndicator, POSITION_TYPE> visited_poi;//–K–âPOI
		std::vector<int> pause_phase;//–K–âPOI‚É‚¨‚¯‚é’â~phase
		int arrive_phase;//“’…‚µ‚½phase
		double pause_time;//’â~ŠÔ
		double starting_speed;//–K–âPOI‚Ìo”­‘¬“x
		double rest_pause_time_when_departing;//o”­‚Ì—]‚è’â~ŠÔ
		double dest_rest_time;//POI‚É“’…‚µ‚½‚Ì—]‚èŠÔ
	};

	template class VisitedPoiInfo<Graph::Coordinate>;
	template class VisitedPoiInfo<Geography::LatLng>;
}