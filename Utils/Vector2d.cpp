#include "stdafx.h"
#include "Vector2d.h"


///<summary>
/// コンストラクタ
///</summary>
///<param name='x'>x</param>
///<param name='y'>y</param>
Graph::Vector2d::Vector2d(double x, double y) : _x(x), _y(y)
{
}


///<summary>
/// コンストラクタ
/// 位置ベクトル作成用
///</summary>
///<param name='c'>Coordinate</param>
Graph::Vector2d::Vector2d(Graph::Coordinate c) : _x(c.x()), _y(c.y())
{
}


///<summary>
/// デストラクタ
///</summary>
Graph::Vector2d::~Vector2d()
{
}


///<summary>
/// x成分値を取得します．
///</summary>
double Graph::Vector2d::x() const
{
	return _x;
}


///<summary>
/// y成分値を取得します．
///</summary>
double Graph::Vector2d::y() const
{
	return _y;
}

///<summary>
/// ベクトル長を返します．
///</summary>
double Graph::Vector2d::length() const
{
	return std::sqrt(_x * _x + _y * _y);
}


///<summary>
/// 指定した角度分(rad)ベクトルを回転させます．
///</summary>
void Graph::Vector2d::rotate(double radian)
{
	double r = length();
	double rotated_angle = std::atan2(_y, _x) + radian;
	_x = r * std::cos(rotated_angle);
	_y = r * std::sin(rotated_angle);
}

///<summary>
/// 指定した2つのベクトルの内積を返します．
///</summary>
double Graph::inner_product(const Graph::Vector2d& v1, const Graph::Vector2d& v2)
{
	return v1._x * v2._x + v1._y * v2._y;
}


///<summary>
/// 2つのベクトルの外積のz成分のスカラ値を返します．
///</summary>
double Graph::cross_product(const Graph::Vector2d& v1, const Graph::Vector2d& v2)
{
	return v1.x() * v2.y() - v1.y() * v2.x();
}

void Graph::Vector2d::operator=(Graph::Vector2d v)
{
	_x = v.x();
	_y = v.y();
}

Graph::Vector2d Graph::operator+(const Graph::Vector2d& v1, const Graph::Vector2d& v2)
{
	return Graph::Vector2d(v1._x + v2._x, v1._y + v2._y);
}

Graph::Vector2d Graph::operator-(const Graph::Vector2d& v1, const Graph::Vector2d& v2)
{
	return Graph::Vector2d(v1._x - v2._x, v1._y - v2._y);
}

bool Graph::operator==(const Graph::Vector2d& v1, const Graph::Vector2d& v2)
{
	return v1._x == v2._x && v1._y == v2._y;
}

bool Graph::operator!=(const Graph::Vector2d& v1, const Graph::Vector2d& v2)
{
	return !(v1._x == v2._x && v1._y == v2._y);
}
