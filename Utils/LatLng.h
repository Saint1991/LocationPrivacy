#ifdef UTILS_EXPORTS
#define LATLNG_API __declspec(dllexport)
#else
#define LATLNG_API __declspec(dllimport)
#endif

#pragma once
#include "FileExportable.h"
#include "GeoCalculation.h"
#include "Coordinate.h"
#include "AngleUtility.h"

namespace Geography 
{

	///<summary>
	/// 緯度・経度を表すクラス
	///</summary>
	class LATLNG_API LatLng : public Graph::Coordinate, public IO::FileExportable
	{
	protected:
		bool is_valid = false;
		bool validate(double latitude, double longitude);

	public:
		static constexpr char* LATITUDE = "Latitude";
		static constexpr char*  LONGITUDE = "Longitude";
		static constexpr double A = 6378137.0;
		static constexpr double B = 6356752.31414036;
		static constexpr double F = 0.0033528106811823;

		LatLng();
		LatLng(double latitude, double longitude);
		virtual ~LatLng();
		double lat() const;
		double lng() const;
		bool is_valid_point() const;
		bool translate(double distance, double azimuth_angle);
		std::unordered_map<std::string, std::string> get_export_data() const;
				
		LATLNG_API friend double dist(const LatLng& ll1, const LatLng& ll2);
		LATLNG_API friend double angle(const LatLng& from, const LatLng& to);
	
	};
}
