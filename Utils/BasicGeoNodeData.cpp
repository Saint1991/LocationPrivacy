#include "stdafx.h"
#include "BasicGeoNodeData.h"

namespace Geography
{
	///<summary>
	/// コンストラクタ
	///</summary>
	BasicGeoNodeData::BasicGeoNodeData( LatLng position) : position(position)
	{
	}


	///<summary>
	/// コンストラクタ
	///</summary>
	BasicGeoNodeData::BasicGeoNodeData(double latitude, double longitude) : position(LatLng(latitude, longitude))
	{

	}


	///<summary>
	/// デストラクタ
	///</summary>
	BasicGeoNodeData::~BasicGeoNodeData()
	{
	}

	///<summary>
	/// 緯度・経度を取得する
	///</summary>
	LatLng BasicGeoNodeData::get_position() const
	{
		return this->position;
	}

}
