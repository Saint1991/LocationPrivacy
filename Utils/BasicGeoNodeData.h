#ifdef UTILS_EXPORTS
#define BASIC_GEONODE_DATA_API __declspec(dllexport)
#else
#define BASIC_GEONODE_DATA_API __declspec(dllimport)
#endif

#pragma once
#include "LatLng.h"


namespace Geography
{

	///<summary>
	/// 地理的なノードがもつ基本情報(緯度・経度)
	///</summary>
	class BASIC_GEONODE_DATA_API BasicGeoNodeData
	{
	protected:
		std::unique_ptr<Geography::LatLng> position;
	public:
		BasicGeoNodeData(Geography::LatLng position);
		BasicGeoNodeData(double latitude, double longitude);
		~BasicGeoNodeData();		
		const Geography::LatLng get_position() const;
	};
}


