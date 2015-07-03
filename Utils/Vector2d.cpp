#include "stdafx.h"
#include "Vector2d.h"


///<summary>
/// �R���X�g���N�^
///</summary>
///<param name='x'>x</param>
///<param name='y'>y</param>
Graph::Vector2d::Vector2d(double x, double y) : x(x), y(y)
{
}


///<summary>
/// �f�X�g���N�^
///</summary>
Graph::Vector2d::~Vector2d()
{
}


///<summary>
/// �x�N�g������Ԃ��܂��D
///</summary>
double Graph::Vector2d::length()
{
	return std::sqrt(x * x + y * y);
}


///<summary>
/// �w�肵���p�x��(rad)�x�N�g������]�����܂��D
///</summary>
void Graph::Vector2d::rotate(double radian)
{
	double r = length();
	double rotated_angle = std::atan2(y, x) + radian;
	x = r * std::cos(rotated_angle);
	y = r * std::sin(rotated_angle);
}

///<summary>
/// �w�肵��2�̃x�N�g���̓��ς�Ԃ��܂��D
///</summary>
double Graph::inner_product(Graph::Vector2d v1, Graph::Vector2d v2)
{
	return v1.x * v2.x + v1.y * v2.y;
}

void Graph::Vector2d::operator=(Graph::Vector2d v)
{
	x = v.x;
	y = v.y;
}

Graph::Vector2d operator+(Graph::Vector2d v1, Graph::Vector2d v2)
{
	return Graph::Vector2d(v1.x + v2.x, v1.y + v2.y);
}

Graph::Vector2d operator-(Graph::Vector2d v1, Graph::Vector2d v2)
{
	return Graph::Vector2d(v1.x - v2.x, v1.y - v2.y);
}

bool operator==(Graph::Vector2d v1, Graph::Vector2d v2)
{
	return v1.x == v2.x && v1.y == v2.y;
}

bool operator!=(Graph::Vector2d v1, Graph::Vector2d v2)
{
	return !(v1.x == v2.x && v1.y == v2.y);
}



