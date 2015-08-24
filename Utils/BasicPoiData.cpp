#include "stdafx.h"
#include "BasicPoiData.h"

namespace Geography
{


	///<summary>
	/// コンストラクタ
	///</summary>
	BasicPoiData::BasicPoiData(const std::string& venue_name, const LatLng& position) : BasicGeoNodeData(position), venue_name(venue_name)
	{
	}


	///<summary>
	/// コンストラクタ
	///</summary>
	BasicPoiData::BasicPoiData(const std::string& venue_name, double latitude, double longitude) : BasicGeoNodeData(latitude, longitude), venue_name(venue_name)
	{
	}

	///<summary>
	/// デストラクタ
	///</summary>
	BasicPoiData::~BasicPoiData()
	{
	}


	///<summary>
	/// POI名の取得
	///</summary>
	const std::string BasicPoiData::get_venue_name() const
	{
		return venue_name;
	}

	#pragma region Export

	const std::string BasicPoiData::VENUE_NAME = "venue_name";

	///<summary>
	/// ファイルエクスポート用
	///</summary>
	std::unordered_map<std::string, std::string> BasicPoiData::get_export_data() const
	{
		std::unordered_map<std::string, std::string> ret = {
			{VENUE_NAME, venue_name},
			{LatLng::LATITUDE, std::to_string(position.lat())},
			{LatLng::LONGITUDE, std::to_string(position.lng())}
		};
		return ret;
	}

	#pragma endregion Export関係
}


