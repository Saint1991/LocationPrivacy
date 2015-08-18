#ifdef UTILS_EXPORTS
#define BASIC_PATH_DATA_API __declspec(dllexport)
#else
#define BASIC_PATH_DATA_API __declspec(dllimport)
#endif

#pragma once
#include "Coordinate.h"
#include "LatLng.h"

namespace Graph
{


	///<summary>
	/// 距離を持つデータ
	/// 使うコンストラクタによって直交座標か緯度経度かを使い分けて距離計算できる
	///</summary>
	class BASIC_PATH_DATA_API BasicPathData
	{
	protected:
		double distance;

	public:
		BasicPathData(const double distance);
		BasicPathData(const Coordinate& c1, const Coordinate& c2);
		BasicPathData(const Geography::LatLng& ll1, const Geography::LatLng& ll2);
		~BasicPathData();
		
		const double get_distance() const;
		void update_distance(double distance);
	};
}


