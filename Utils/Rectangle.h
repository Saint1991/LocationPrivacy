#ifdef UTILS_EXPORTS
#define RECTANGLE_API __declspec(dllexport)
#else
#define RECTANGLE_API __declspec(dllimport)
#endif

#pragma once
#include "Coordinate.h"
#include "LatLng.h"

namespace Graph 
{
	
	class RECTANGLE_API Rectangle
	{
	public:
		double top;
		double left;
		double bottom;
		double right;

		Rectangle(double top, double left, double bottom, double right);
		~Rectangle();
		double get_height();
		double get_width();
		double get_area();
		
	};

}

