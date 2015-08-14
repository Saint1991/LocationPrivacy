#ifdef UTILS_EXPORTS
#define VECTOR2DFACTORY_API __declspec(dllexport)
#else
#define VECTOR2DFACTORY_API __declspec(dllimport)
#endif

#pragma once
#include "Vector2d.h"

namespace Graph 
{
	class  VECTOR2DFACTORY_API Vector2dFactory
	{
	private:
		Vector2dFactory();
		~Vector2dFactory();
	public:
		static Vector2d create_by_cartesian(double x, double y);
		static Vector2d create_by_polar(double r, double angle);
	};
}


