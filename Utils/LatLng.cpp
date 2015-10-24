#include "stdafx.h"
#include "LatLng.h"


namespace Geography 
{

	///<summary>
	/// コンストラクタ
	/// 緯度，経度ともに0で初期化
	///</summary>
	LatLng::LatLng() : Graph::Coordinate()
	{

	}

	///<summary>
	/// コンストラクタ
	/// 緯度が[-90, 90]でない場合，経度が[-180, 180]でない場合はinvalid_argumentをスローします．
	///</summary>
	LatLng::LatLng(double latitude, double longitude) : Graph::Coordinate(longitude, latitude)
	{
		is_valid = validate(latitude, longitude);
	}

	///<summary>
	/// デストラクタ
	///</summary>
	LatLng::~LatLng()
	{
	}

	///<summary>
	/// 緯度経度の値をチェックします．
	/// 不正な値の場合invalid_argumentをスローしてfalseを返します
	///</summary>
	bool LatLng::validate(double latitude, double longitude)
	{
		if (latitude < -90.0 || 90.0 < latitude) {
			throw std::invalid_argument("Latitude must be within the range [-90, 90]");
			return false;
		}
		if (longitude < -180.0 || 180.0 < longitude) {
			throw std::invalid_argument("Longitude must be within the range [-180, 180]");
			return false;
		}
		return true;
	}


	///<summary>
	/// 緯度を取得します
	///</summary>
	double LatLng::lat() const
	{
		return _y;
	}


	///<summary>
	/// 経度を取得します
	///</summary>
	double LatLng::lng() const
	{
		return _x;
	}


	///<summary>
	/// 不正な値で構成されているかどうかを返す
	///</summary>
	bool LatLng::is_valid_point() const
	{
		return is_valid;
	}

	const double LatLng::A = 6378137.0;
	const double LatLng::B = 6356752.31414036;
	const double LatLng::F = 0.0033528106811823;

	///<summary>
	/// 距離と方位角(rad)を指定して点を移動する
	/// 方位角は東から時計周りの座標系
	/// 計算はおそらくLambert法ではない，下記で利用しているものを登用した．
	/// http://www.mapli.net/direct/
	/// 計算結果が不正な値の場合はfalseを返し，移動は行わない
	///</summary>
	bool LatLng::translate(double distance, double azimuth_angle)
	{

		LatLng translated = Geography::GeoCalculation::calc_translated_point(*this, distance, azimuth_angle);

		double lat = translated.lat();
		double lng = translated.lng();

		if (std::isfinite(lat) && std::isfinite(lng) && validate(lat, lng)) {
			_y = lat;
			_x = lng;
			return true;
		}
		return false;
	}

	#pragma region Export
	
	const std::string LatLng::LATITUDE = "latitude";
	const std::string LatLng::LONGITUDE = "longitude";

	///<summary>
	/// ファイル出力向けのハッシュデータを作成します．
	///</summary>
	std::unordered_map<std::string, std::string> LatLng::get_export_data() const
	{
		std::unordered_map<std::string, std::string> ret = {
			{LATITUDE, std::to_string(_y)},
			{LONGITUDE, std::to_string(_x)}
		};
		return ret;
	}

	#pragma endregion Export関係

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

