#ifdef UTILS_EXPORTS
#define POI_DATA_API __declspec(dllexport)
#else
#define POI_DATA_API __declspec(dllimport)
#endif

#pragma once
#include "BasicGeoNodeData.h"

namespace Geography
{

	//POI‚ÌID (ƒJƒeƒSƒŠID‚Å‚Í‚È‚¢)
	typedef long poi_id;

	struct POI_DATA_API PoiData : public BasicGeoNodeData
	{
	protected:
		std::string category_id;
		std::string category_name;
		std::string venue_name;
	
	public:
		PoiData(const std::string& venue_name, const std::string& category_id, const std::string& category_name, const LatLng& position);
		PoiData(const std::string& venue_name, const std::string& category_id, const std::string& category_name, double latitude, double longitude);
		virtual ~PoiData();
		
		const std::string get_category_id() const;
		const std::string get_category_name() const;
		const std::string get_venue_name() const;
	};
}


