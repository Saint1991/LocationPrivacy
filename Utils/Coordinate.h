#ifdef UTILS_EXPORTS
#define COORDINATE_API __declspec(dllexport)
#else
#define COORDINATE_API __declspec(dllimport)
#endif

#pragma once
#include "Vector2d.h"
#include "Identifiable.h"

namespace Graph 
{
	
	class Vector2d;


	///<summary>
	/// ユークリッド空間の点を表すクラス
	///</summary>
	class  COORDINATE_API Coordinate : public Identifiable
	{
	private:
		double _x;
		double _y;
	
	public:
		Coordinate(double x, double y, unsigned int id = 0);
		~Coordinate(); 

		double x() const;
		double y() const;

		friend Vector2d operator+(const Coordinate& c1, const Coordinate& c2);
		friend Vector2d operator-(const Coordinate& c1, const Coordinate& c2);
		friend double dist(const Coordinate& c1, const Coordinate& c2);
	};
}


