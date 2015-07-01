#include "stdafx.h"
#include "AngleUtility.h"


Math::AngleUtility::AngleUtility()
{
}


Math::AngleUtility::~AngleUtility()
{
}

///<summary>
/// 度単位の角度をラジアン単位に変換する．
///</summary>
///<param name='angle_radian'>角度(度)</param>
double Math::AngleUtility::convert_to_radian(double angle_degree)
{
	double radian = angle_degree * M_PI / 180.0;
	return radian;
}

///<summary>
/// radian単位の角度を度単位に変換する．
///</summary>
///<param name='angle_radian'>角度(ラジアン)</param>
double Math::AngleUtility::convert_to_degree(double angle_radian)
{
	double degree = angle_radian * 180.0 / M_PI;
	return degree;
}
