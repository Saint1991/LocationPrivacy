#include "stdafx.h"
#include "Vector2dFactory.h"


Graph::Vector2dFactory::Vector2dFactory()
{
}


Graph::Vector2dFactory::~Vector2dFactory()
{
}


///<summary>
/// x, y���W����Vector2d�̃C���X�^���X���쐬����D
///</summary>
Graph::Vector2d Graph::Vector2dFactory::create_by_cartesian(double x, double y)
{
	return Graph::Vector2d(x, y);
}


///<summary>
/// �ɍ��W�w���Vector2d�̃C���X�^���X���쐬����D
///</summary>
Graph::Vector2d Graph::Vector2dFactory::create_by_polar(double length, double angle)
{
	double x = length * std::cos(angle);
	double y = length * std::sin(angle);
	return Graph::Vector2d(x, y);
}
