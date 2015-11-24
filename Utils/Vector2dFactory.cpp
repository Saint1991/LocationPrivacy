#include "stdafx.h"
#include "Vector2dFactory.h"


Graph::Vector2dFactory::Vector2dFactory()
{
}


Graph::Vector2dFactory::~Vector2dFactory()
{
}


///<summary>
/// x, y座標からVector2dのインスタンスを作成する．
///</summary>
Graph::Vector2d Graph::Vector2dFactory::create_by_cartesian(double x, double y)
{
	return Graph::Vector2d(x, y);
}


///<summary>
/// 極座標指定でVector2dのインスタンスを作成する．
///</summary>
Graph::Vector2d Graph::Vector2dFactory::create_by_polar(double size, double angle)
{
	double x = size * std::cos(angle);
	double y = size * std::sin(angle);
	return Graph::Vector2d(x, y);
}
