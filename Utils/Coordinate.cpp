#include "stdafx.h"
#include "Coordinate.h"


///<summary>
/// �R���X�g���N�^
///</summary>
Graph::Coordinate::Coordinate(unsigned int id, double x, double y) : _id(id),  _x(x), _y(y)
{
}


///<summary>
/// �f�X�g���N�^
///</summary>
Graph::Coordinate::~Coordinate()
{
}


unsigned int Graph::Coordinate::id() const 
{
	return _id;
}

///<summary>
/// x���W�̎擾
///</summary>
double Graph::Coordinate::x() const
{
	return _x;
}


///<summary>
/// y���W�̎擾
///</summary>
double Graph::Coordinate::y() const
{
	return _y;
}


Graph::Vector2d Graph::operator+(Graph::Coordinate c1, Graph::Coordinate c2)
{
	return Graph::Vector2d(c1._x + c2._x, c1._y + c2._y);
}

Graph::Vector2d Graph::operator-(Graph::Coordinate c1, Graph::Coordinate c2)
{
	return Graph::Vector2d(c1._x - c2._x, c1._y - c2._y);
}

