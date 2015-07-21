#ifdef UTILS_EXPORTS
#define COORDINATE_API __declspec(dllexport)
#else
#define COORDINATE_API __declspec(dllimport)
#endif

#pragma once
#include "Vector2d.h"

namespace Graph {
	
	class Vector2d;

	class  COORDINATE_API Coordinate
	{
	private:
		unsigned int _id;
		double _x;
		double _y;
	
	public:
		Coordinate(unsigned int id,  double x, double y);
		~Coordinate(); 
		double x() const;
		double y() const;
		unsigned int id() const;

		friend Vector2d operator+(Graph::Coordinate c1, Graph::Coordinate c2);
		friend Vector2d operator-(Graph::Coordinate c1, Graph::Coordinate c2);
	};
}


