#include "stdafx.h"
#include "Vector2d.h"


///<summary>
/// コンストラクタ
///</summary>
///<param name='x'>x</param>
///<param name='y'>y</param>
Graph::Vector2d::Vector2d(double x, double y) : x(x), y(y)
{
}


///<summary>
/// デストラクタ
///</summary>
Graph::Vector2d::~Vector2d()
{
}


///<summary>
/// ベクトル長を返します．
///</summary>
double Graph::Vector2d::length()
{
	return std::sqrt(x * x + y * y);
}


///<summary>
/// 指定した角度分(rad)ベクトルを回転させます．
///</summary>
void Graph::Vector2d::rotate(double radian)
{
	double r = length();
	double rotated_angle = std::atan2(y, x) + radian;
	x = r * std::cos(rotated_angle);
	y = r * std::sin(rotated_angle);
}

///<summary>
/// 指定した2つのベクトルの内積を返します．
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



