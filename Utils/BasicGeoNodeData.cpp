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

	#pragma region Export

	///<summary>
	/// ファイルエクスポート用
	///</summary>
	std::unordered_map<std::string, std::string> BasicGeoNodeData::get_export_data() const
	{
		std::unordered_map<std::string, std::string> ret = {
			{LatLng::LATITUDE, std::to_string(position.lat())},
			{LatLng::LONGITUDE, std::to_string(position.lng())}
		};
		return ret;
	}

	#pragma endregion Export関係
}
