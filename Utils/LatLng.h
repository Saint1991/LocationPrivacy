#ifdef UTILS_EXPORTS
#define LATLNG_API __declspec(dllexport)
#else
#define LATLNG_API __declspec(dllimport)
#endif

#pragma once
#include "FileExportable.h"
#include "GeoCalculation.h"

namespace Geography 
{

	///<summary>
	/// 緯度・経度を表すクラス
	///</summary>
	class LATLNG_API LatLng : public IO::FileExportable
	{
	protected:
		double latitude;
		double longitude;

		void set_lat(double latitude);
		void set_lng(double longitude);

	public:
		static const std::string LATITUDE;
		static const std::string LONGITUDE;

		LatLng(double latitude, double longitude);
		virtual ~LatLng();
		double lat() const;
		double lng() const;
		std::unordered_map<std::string, std::string> get_export_data() const;

		LATLNG_API friend double dist(const LatLng& ll1, const LatLng& ll2);
		LATLNG_API friend double angle(const LatLng& from, const LatLng& to);
	};
}
