#include "stdafx.h"
#include "Rectangle.h"

namespace Graph {


	///<summary>
	/// �R���X�g���N�^
	///</summary>
	template <typename POSITION_TYPE>
	Rectangle<POSITION_TYPE>::Rectangle() : top(0.0), left(0.0), bottom(0.0), right(0.0)
	{

	}

	///<summary>
	/// �R���X�g���N�^
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
	/// �f�X�g���N�^
	///</summary>
	template <typename POSITION_TYPE>
	Rectangle<POSITION_TYPE>::~Rectangle()
	{

	}

	///<summary>
	/// �̈���ɃG���e�B�e�B�����邩�ǂ������擾
	/// �ܓx�o�xVer�D
	///</summary>
	template <typename POSITION_TYPE>
	bool Rectangle<POSITION_TYPE>::contains(const POSITION_TYPE& position) const
	{
		double x = position.x();
		double y = position.y();
		return left <= x && x <= right && bottom <= y && y <= top;
	}


	///<summary>
	/// Coordinate�̏ꍇ��Width�v�Z
	///</summary>
	template <>
	double Rectangle<Coordinate>::width() const
	{
		return std::abs(right - left);
	}


	///<summary>
	/// LatLng�̏ꍇ��Width[m]�̌v�Z
	///</summary>
	template <>
	double Rectangle<Geography::LatLng>::width() const
	{
		Geography::LatLng topleft(top, left);
		Geography::LatLng topright(top, right);
		return Geography::GeoCalculation::lambert_distance(topleft, topright);
	}
	

	///<summary>
	/// Coordinate�̏ꍇ��Height�v�Z
	///</summary>
	template <>
	double Rectangle<Coordinate>::height() const
	{
		return std::abs(top - bottom);
	}


	///<summary>
	/// LatLng�̏ꍇ��Height[m]�̌v�Z
	///</summary>
	template <>
	double Rectangle<Geography::LatLng>::height() const
	{
		Geography::LatLng topleft(top, left);
		Geography::LatLng bottomleft(bottom, left);
		return Geography::GeoCalculation::lambert_distance(topleft, bottomleft);
	}


	///<summary>
	/// �ʐς̌v�Z
	/// LatLng�̏ꍇ��[m^2]
	///</summary>
	template <typename POSITION_TYPE>
	double Rectangle<POSITION_TYPE>::area_size() const
	{
		return width() * height();
	}
}
