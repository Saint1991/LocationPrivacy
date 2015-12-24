#include "stdafx.h"
#include "Rectangle.h"

namespace Graph {


	///<summary>
	/// コンストラクタ
	///</summary>
	template <typename POSITION_TYPE>
	Rectangle<POSITION_TYPE>::Rectangle() : top(0.0), left(0.0), bottom(0.0), right(0.0)
	{

	}

	///<summary>
	/// コンストラクタ
	///</summary>
	template <typename POSITION_TYPE>
	Rectangle<POSITION_TYPE>::Rectangle(double top, double left, double bottom, double right)
	{
		if (right < left) std::swap(left, right);
		if (top < bottom) std::swap(bottom, top);
		this->top = top;
		this->left = left;
		this->right = right;
		this->bottom = bottom;
	}

	///<summary>
	/// デストラクタ
	///</summary>
	template <typename POSITION_TYPE>
	Rectangle<POSITION_TYPE>::~Rectangle()
	{

	}

	///<summary>
	/// 領域内にエンティティがあるかどうかを取得
	/// 緯度経度Ver．
	///</summary>
	template <typename POSITION_TYPE>
	bool Rectangle<POSITION_TYPE>::contains(const POSITION_TYPE& position) const
	{
		double x = position.x();
		double y = position.y();
		return left <= x && x <= right && bottom <= y && y <= top;
	}


	///<summary>
	/// rectの各辺をx倍に変換したものを返す
	/// 緯度経度Ver.
	///</summary>
	template <>
	Rectangle<Geography::LatLng> Rectangle<Geography::LatLng>::transform_rect_of_latlang_to_x_times(double x) const
	{
		if (x = 0.0) throw std::invalid_argument("X must not be 0.0");

		Geography::LatLng center((this->top + this->bottom) / 2, (this->left + this->right) / 2);

		double transformed_height = this->height() * x;
		double transformed_width = this->width() * x;

		double top = Geography::GeoCalculation::calc_translated_point(center, transformed_height / 2, M_PI_2 * 3).lat();;
		double left = Geography::GeoCalculation::calc_translated_point(center, transformed_width / 2, M_PI).lng();
		double bottom = Geography::GeoCalculation::calc_translated_point(center, transformed_height / 2, M_PI_2).lat();;
		double right = Geography::GeoCalculation::calc_translated_point(center, transformed_width / 2, 0).lng();
		
		return Graph::Rectangle<Geography::LatLng>(top, left, bottom, right);
	}

	///<summary>
	/// rectの各辺をx倍に変換したものを返す
	/// CoordinateVer.
	///</summary>
	template <>
	Rectangle<Coordinate> Rectangle<Coordinate>::transform_rect_of_coordinate_to_x_times(double x) const
	{
		if (x = 0.0) throw std::invalid_argument("X must not be 0.0");
		return Graph::Rectangle<Coordinate>(this->top * x, this->left * x, this->bottom * x, this->right * x);
	}


	///<summary>
	/// Coordinateの場合のWidth計算
	///</summary>
	template <>
	double Rectangle<Coordinate>::width() const
	{
		return std::abs(right - left);
	}


	///<summary>
	/// LatLngの場合のWidth[m]の計算
	///</summary>
	template <>
	double Rectangle<Geography::LatLng>::width() const
	{
		Geography::LatLng topleft(top, left);
		Geography::LatLng topright(top, right);
		return Geography::GeoCalculation::lambert_distance(topleft, topright);
	}
	

	///<summary>
	/// Coordinateの場合のHeight計算
	///</summary>
	template <>
	double Rectangle<Coordinate>::height() const
	{
		return std::abs(top - bottom);
	}


	///<summary>
	/// LatLngの場合のHeight[m]の計算
	///</summary>
	template <>
	double Rectangle<Geography::LatLng>::height() const
	{
		Geography::LatLng topleft(top, left);
		Geography::LatLng bottomleft(bottom, left);
		return Geography::GeoCalculation::lambert_distance(topleft, bottomleft);
	}


	///<summary>
	/// 面積の計算
	/// LatLngの場合は[m^2]
	///</summary>
	template <typename POSITION_TYPE>
	double Rectangle<POSITION_TYPE>::area_size() const
	{
		return width() * height();
	}
}
