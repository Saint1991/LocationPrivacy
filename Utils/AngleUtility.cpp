#include "stdafx.h"
#include "AngleUtility.h"


Math::AngleUtility::AngleUtility()
{
}


Math::AngleUtility::~AngleUtility()
{
}

///<summary>
/// �x�P�ʂ̊p�x�����W�A���P�ʂɕϊ�����D
///</summary>
///<param name='angle_radian'>�p�x(�x)</param>
double Math::AngleUtility::convert_to_radian(double angle_degree)
{
	double radian = angle_degree * M_PI / 180.0;
	return radian;
}

///<summary>
/// radian�P�ʂ̊p�x��x�P�ʂɕϊ�����D
///</summary>
///<param name='angle_radian'>�p�x(���W�A��)</param>
double Math::AngleUtility::convert_to_degree(double angle_radian)
{
	double degree = angle_radian * 180.0 / M_PI;
	return degree;
}
