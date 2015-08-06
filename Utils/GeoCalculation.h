#ifdef UTILS_EXPORTS
#define GEOGRAPHIC_CALC_API __declspec(dllexport)
#else
#define GEOGRAPHIC_CALC_API __declspec(dllimport)
#endif

#pragma once
#include "LatLng.h"

namespace Geography 
{

	//LatLngの前方宣言
	class LatLng;

	///<summary>
	/// 地理的な計算のユーティリティクラス
	///</summary>
	class GeoCalculation
	{
	private:
		
		//地球の半径
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


