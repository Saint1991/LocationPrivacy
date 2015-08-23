#ifdef UTILS_EXPORTS
#define POI_API __declspec(dllexport)
#else
#define POI_API __declspec(dllimport)
#endif
#pragma once

#include "PoiData.h"
#include "BasicNode.h"

namespace Geography
{
	class POI_API Poi : Graph::BasicNode<PoiData>
	{
	public:
		Poi();
		virtual ~Poi();
	};
}


