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

		//�����o�ϐ�
		std::pair<Graph::MapNodeIndicator, POSITION_TYPE> visited_poi;//�K��POI
		std::vector<int> pause_phase;//�K��POI�ɂ������~phase
		int arrive_phase;//��������phase
		double pause_time;//��~����
		double starting_speed;//�K��POI�̏o�����x
		double rest_pause_time_when_departing;//�o�����̗]���~����
		double dest_rest_time;//POI�ɓ����������̗]�莞��
	};

	template class VisitedPoiInfo<Graph::Coordinate>;
	template class VisitedPoiInfo<Geography::LatLng>;
}