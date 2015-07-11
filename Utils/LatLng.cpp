#include "stdafx.h"
#include "LatLng.h"

///<summary>
/// コンストラクタ
/// 緯度が[-90, 90]でない場合，経度が[-180, 180]でない場合はinvalid_argumentをスローします．
///</summary>
Geography::LatLng::LatLng(double latitude, double longitude)
{
	set_lat(latitude);
	set_lng(longitude);
}

///<summary>
/// デストラクタ
///</summary>
Geography::LatLng::~LatLng()
{
}


///<summary>
/// 緯度を設定します．
/// [-90, 90]でない場合はinvalid_argumentをスローします．
///</summary>
void Geography::LatLng::set_lat(double latitude)
{
	if (latitude < -90.0 || 90.0 < latitude) {
		throw std::invalid_argument("Latitude must be within the range [-90, 90]");
	}
	this->latitude = latitude;
}


///<summary>
/// 経度を設定します．
/// [-180, 180]でない場合はinvalid_argumentをスローします．
///</summary>
void Geography::LatLng::set_lng(double longitude)
{
	if (longitude < -180.0 || 180.0 < longitude) {
		throw std::invalid_argument("Longitude must be within the range [-180, 180]");
	}
	this->longitude = longitude;
}


///<summary>
/// 緯度を取得します
///</summary>
double Geography::LatLng::lat() const
{
	return latitude;
}


///<summary>
/// 経度を取得します
///</summary>
double Geography::LatLng::lng() const
{
	return longitude;
}