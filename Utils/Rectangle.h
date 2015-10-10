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
	/// �����`�̈��\���N���X
	/// Left < Right
	/// Bottom < Top
	/// �ƂȂ�̂Œ���
	/// LatLng�̏ꍇ��Width, Height�̌v�Z�͍��ӁC��ӂɊ�Â��Čv�Z���Ă���̂ŁC
	/// �c��Rectangle�͑z�肩��O���Ă���D���̂��ߌ����Ȍv�Z�Ƃ͌����Ȃ��̂Œ���
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

