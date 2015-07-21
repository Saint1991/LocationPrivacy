#include "stdafx.h"
#include "Coordinate.h"


///<summary>
/// コンストラクタ
///</summary>
Graph::Coordinate::Coordinate(unsigned int id, double x, double y) : _id(id),  _x(x), _y(y)
{
}


///<summary>
/// デストラクタ
///</summary>
Graph::Coordinate::~Coordinate()
{
}


unsigned int Graph::Coordinate::id() const 
{
	return _id;
}

///<summary>
/// x座標の取得
///</summary>
double Graph::Coordinate::x() const
{
	return _x;
}


///<summary>
/// y座標の取得
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

