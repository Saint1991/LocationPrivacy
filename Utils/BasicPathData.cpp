#include "stdafx.h"
#include "BasicPathData.h"


namespace Graph 
{
	
	///<summary>
	/// コンストラクタ
	///</summary>
	BasicPathData::BasicPathData(const double distance) : distance(distance)
	{
	}

	
	///<summary>
	/// コンストラクタ
	///</summary>
	BasicPathData::BasicPathData(const Coordinate& c1, const Coordinate& c2)
	{
		distance = dist(c1, c2);
	}


	///<summary>
	/// コンストラクタ
	///</summary>
	BasicPathData::BasicPathData(const Geography::LatLng& ll1, const Geography::LatLng& ll2)
	{
		distance = Geography::dist(ll1, ll2);
	}

	///<summary>
	/// デストラクタ
	///</summary>
	BasicPathData::~BasicPathData()
	{
	}

	const double Graph::BasicPathData::get_distance() const
	{
		return distance;
	}

	void Graph::BasicPathData::update_distance(double distance)
	{
		this->distance = distance;
	}
}
