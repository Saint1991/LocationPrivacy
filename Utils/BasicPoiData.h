#ifdef UTILS_EXPORTS
#define BASIC_POI_DATA_API __declspec(dllexport)
#else
#define BASIC_POI_DATA_API __declspec(dllimport)
#endif
#pragma once
#include "BasicGeoNodeData.h"

namespace Geography
{

	///<summary>
	/// POIが持つ基本データ
	/// POI名と地点
	///</summary>
	struct BASIC_POI_DATA_API BasicPoiData : public BasicGeoNodeData 
	{
	protected:
		std::string venue_name;

	public:

		static const std::string VENUE_NAME;

		BasicPoiData(const std::string& venue_name, const LatLng& position);
		BasicPoiData(const std::string& venue_name, double latitude, double longitude);
		virtual ~BasicPoiData();

		const std::string get_venue_name() const;
		std::unordered_map<std::string, std::string> get_export_data() const;
	};
}


