#ifdef UTILS_EXPORTS
#define POI_DATA_API __declspec(dllexport)
#else
#define POI_DATA_API __declspec(dllimport)
#endif

#pragma once

namespace Geography
{

	//POI��ID (�J�e�S��ID�ł͂Ȃ�)
	typedef long poi_id;

	class POI_DATA_API PoiData
	{
	public:
		PoiData();
		~PoiData();
	};
}


