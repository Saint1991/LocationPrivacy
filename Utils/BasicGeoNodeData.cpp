#include "stdafx.h"
#include "BasicGeoNodeData.h"


///<summary>
/// コンストラクタ
///</summary>
Geography::BasicGeoNodeData::BasicGeoNodeData(Geography::LatLng position) : position(std::make_unique<Geography::LatLng>(position))
{
}


///<summary>
/// コンストラクタ
///</summary>
Geography::BasicGeoNodeData::BasicGeoNodeData(double latitude, double longitude) : position(std::make_unique<Geography::LatLng>(latitude, longitude))
{

}


///<summary>
/// デストラクタ
///</summary>
Geography::BasicGeoNodeData::~BasicGeoNodeData()
{
}

///<summary>
/// 緯度・経度を取得する
///</summary>
const Geography::LatLng Geography::BasicGeoNodeData::get_position() const
{
	return *this->position;
}
