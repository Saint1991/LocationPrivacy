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
	
	template <typename POSITION_TYPE> 
	class RECTANGLE_API Rectangle
	{
	private:
		POSITION_TYPE left_up;
		POSITION_TYPE right_up;
		POSITION_TYPE left_down;
		POSITION_TYPE right_down;
		
	public:
		Rectangle(POSITION_TYPE left_up, POSITION_TYPE right_up, POSITION_TYPE left_down, POSITION_TYPE right_down)
		~Rectangle();
		int get_height();
		int get_width();
		int get_area();
		void set();



	};

	template class Rectangle<Coordinate>;
	template class Rectangle<Geography::LatLng>;
}

