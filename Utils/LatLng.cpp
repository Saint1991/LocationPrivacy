#include "stdafx.h"
#include "LatLng.h"


namespace Geography 
{
	///<summary>
	/// コンストラクタ
	/// 緯度が[-90, 90]でない場合，経度が[-180, 180]でない場合はinvalid_argumentをスローします．
	///</summary>
	LatLng::LatLng(double latitude, double longitude)
	{
		set_lat(latitude);
		set_lng(longitude);
	}

	///<summary>
	/// デストラクタ
	///</summary>
	LatLng::~LatLng()
	{
	}


	///<summary>
	/// 緯度を設定します．
	/// [-90, 90]でない場合はinvalid_argumentをスローします．
	///</summary>
	void LatLng::set_lat(double latitude)
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
	void LatLng::set_lng(double longitude)
	{
		if (longitude < -180.0 || 180.0 < longitude) {
			throw std::invalid_argument("Longitude must be within the range [-180, 180]");
		}
		this->longitude = longitude;
	}


	///<summary>
	/// 緯度を取得します
	///</summary>
	double LatLng::lat() const
	{
		return latitude;
	}


	///<summary>
	/// 経度を取得します
	///</summary>
	double LatLng::lng() const
	{
		return longitude;
	}


	///<summary>
	/// ファイル出力向けのハッシュデータを作成します．
	///</summary>
	std::unordered_map<std::string, std::string> LatLng::get_export_data() const
	{
		std::unordered_map<std::string, std::string> ret_data;
		ret_data.insert(std::make_pair("latitude", std::to_string(latitude)));
		ret_data.insert(std::make_pair("longitude", std::to_string(longitude)));
		return ret_data;
	}


	///<summary>
	/// Lambert-Andoyerの公式に基づく2点間の距離計算 [m]
	///</summary>
	double dist(const LatLng& ll1, const LatLng& ll2)
	{
		return GeoCalculation::lambert_distance(ll1, ll2);
	}


	///<summary>
	/// Lambert-Andoyerの公式に基づくfromからtoへ向かうベクトルの方位角 [rad]
	/// 東を0として時計周りの座標系
	///</summary>
	double angle(const LatLng& from, const LatLng& to)
	{
		return GeoCalculation::lambert_azimuth_angle(from, to);
	}

}

