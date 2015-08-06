#ifdef UTILS_EXPORTS
#define GEOGRAPHIC_CALC_API __declspec(dllexport)
#else
#define GEOGRAPHIC_CALC_API __declspec(dllimport)
#endif

#pragma once
#include "LatLng.h"

namespace Geography 
{

	//LatLng�̑O���錾
	class LatLng;

	///<summary>
	/// �n���I�Ȍv�Z�̃��[�e�B���e�B�N���X
	///</summary>
	class GeoCalculation
	{
	private:
		
		//�n���̔��a
		static const long R = 6378137L;
		
		GeoCalculation();
		~GeoCalculation();

	public:
		GEOGRAPHIC_CALC_API static double harversine(const LatLng& ll1, const LatLng& ll2);
		GEOGRAPHIC_CALC_API static double harversine_distance(const LatLng& ll1, const LatLng& ll2);
		GEOGRAPHIC_CALC_API static double lambert_distance(const LatLng& ll1, const LatLng& ll2);
		GEOGRAPHIC_CALC_API static double lambert_azimuth_angle(const LatLng& from, const LatLng& to);

		GEOGRAPHIC_CALC_API static const std::vector<std::shared_ptr<LatLng const>> convex_hull(const std::vector<std::shared_ptr<LatLng const>>& point_list);
		GEOGRAPHIC_CALC_API static double calc_convex_hull_size(const std::vector<std::shared_ptr<LatLng const>>& point_list);
	};
}


