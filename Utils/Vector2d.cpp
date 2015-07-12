#include "stdafx.h"
#include "Vector2d.h"


///<summary>
/// �R���X�g���N�^
///</summary>
///<param name='x'>x</param>
///<param name='y'>y</param>
Graph::Vector2d::Vector2d(double x, double y) : _x(x), _y(y)
{
}


///<summary>
/// �f�X�g���N�^
///</summary>
Graph::Vector2d::~Vector2d()
{
}


///<summary>
/// x�����l���擾���܂��D
///</summary>
double Graph::Vector2d::x() const
{
	return _x;
}


///<summary>
/// y�����l���擾���܂��D
///</summary>
double Graph::Vector2d::y() const
{
	return _y;
}

///<summary>
/// �x�N�g������Ԃ��܂��D
///</summary>
double Graph::Vector2d::length() const
{
	return std::sqrt(_x * _x + _y * _y);
}


///<summary>
/// �w�肵���p�x��(rad)�x�N�g������]�����܂��D
///</summary>
void Graph::Vector2d::rotate(double radian)
{
	double r = length();
	double rotated_angle = std::atan2(_y, _x) + radian;
	_x = r * std::cos(rotated_angle);
	_y = r * std::sin(rotated_angle);
}

///<summary>
/// �w�肵��2�̃x�N�g���̓��ς�Ԃ��܂��D
///</summary>
double Graph::inner_product(Graph::Vector2d v1, Graph::Vector2d v2)
{
	return v1._x * v2._x + v1._y * v2._y;
}

void Graph::Vector2d::operator=(Graph::Vector2d v)
{
	_x = v.x();
	_y = v.y();
}

Graph::Vector2d Graph::operator+(Graph::Vector2d v1, Graph::Vector2d v2)
{
	return Graph::Vector2d(v1._x + v2._x, v1._y + v2._y);
}

Graph::Vector2d Graph::operator-(Graph::Vector2d v1, Graph::Vector2d v2)
{
	return Graph::Vector2d(v1._x - v2._x, v1._y - v2._y);
}

bool Graph::operator==(Graph::Vector2d v1, Graph::Vector2d v2)
{
	return v1._x == v2._x && v1._y == v2._y;
}

bool Graph::operator!=(Graph::Vector2d v1, Graph::Vector2d v2)
{
	return !(v1._x == v2._x && v1._y == v2._y);
}



