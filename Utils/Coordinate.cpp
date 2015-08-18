#include "stdafx.h"
#include "Coordinate.h"


namespace Graph
{
	///<summary>
	/// �R���X�g���N�^
	///</summary>
	Coordinate::Coordinate(double x, double y, unsigned int id) : Identifiable(id), _x(x), _y(y)
	{
	}


	///<summary>
	/// �f�X�g���N�^
	///</summary>
	Coordinate::~Coordinate()
	{
	}

	///<summary>
	/// x���W�̎擾
	///</summary>
	double Coordinate::x() const
	{
		return _x;
	}


	///<summary>
	/// y���W�̎擾
	///</summary>
	double Coordinate::y() const
	{
		return _y;
	}


	Vector2d operator+(const Coordinate& c1, const Coordinate& c2)
	{
		return Graph::Vector2d(c1._x + c2._x, c1._y + c2._y);
	}

	Vector2d operator-(const Coordinate& c1, const Coordinate& c2)
	{
		return Graph::Vector2d(c1._x - c2._x, c1._y - c2._y);
	}

	///<summary>
	/// 2�_�Ԃ̋������v�Z����
	///</summary>
	double dist(const Coordinate& c1, const Coordinate& c2)
	{
		return (c2 - c1).length();
	}
}
