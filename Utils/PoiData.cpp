#include "stdafx.h"
#include "PoiData.h"

namespace Geography
{


	///<summary>
	/// コンストラクタ
	///</summary>
	PoiData::PoiData(const std::string& venue_name, const std::string& category_id, const std::string& category_name, const LatLng& position)
		: BasicGeoNodeData(position), venue_name(venue_name), category_id(category_id), category_name(category_name)
	{

	}

	///<summary>
	/// コンストラクタ
	///</summary>
	PoiData::PoiData(const std::string& venue_name, const std::string& category_id, const std::string& category_name, double latitude, double longitude)
		: BasicGeoNodeData(latitude, longitude), category_id(category_id), category_name(category_name), venue_name(venue_name)
	{
	}


	///<summary>
	/// デストラクタ
	///</summary>
	PoiData::~PoiData()
	{
	}


	///<summary>
	/// カテゴリIDの取得
	///</summary>
	const std::string PoiData::get_category_id() const
	{
		return category_id;
	}


	///<summary>
	/// カテゴリ名の取得
	///</summary>
	const std::string PoiData::get_category_name() const
	{
		return category_name;
	}


	///<summary>
	/// POI名の取得
	///</summary>
	const std::string PoiData::get_venue_name() const
	{
		return venue_name;
	}
}

