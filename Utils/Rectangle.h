#ifdef UTILS_EXPORTS
#define RECTANGLE_API __declspec(dllexport)
#else
#define RECTANGLE_API __declspec(dllimport)
#endif

#pragma once
#include "Coordinate.h"
#include "LatLng.h"
#include "GeoCalculation.h"

namespace Graph 
{


	///<summary>
	/// 長方形領域を表すクラス
	/// Left < Right
	/// Bottom < Top
	/// となるので注意
	/// LatLngの場合のWidth, Heightの計算は左辺，上辺に基づいて計算しているので，
	/// 歪んだRectangleは想定から外している．そのため厳密な計算とは言えないので注意
	///</summary>
	template <typename POSITION_TYPE = Coordinate>
	class RECTANGLE_API Rectangle
	{
	static_assert(std::is_base_of<Coordinate, POSITION_TYPE>::value, "template type POSITION_TYPE must be derived from Coordinate");
	public:
		double top;
		double left;
		double bottom;
		double right;

		Rectangle();
		Rectangle(double top, double left, double bottom, double right);
		~Rectangle();
		double width() const;
		double height() const;
		double area_size() const;
		bool contains(const POSITION_TYPE& position) const;
	};

	template class Rectangle<Coordinate>;
	template class Rectangle<Geography::LatLng>;
}

