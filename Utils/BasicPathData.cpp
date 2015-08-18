#include "stdafx.h"
#include "BasicPathData.h"


namespace Graph 
{
	
	///<summary>
	/// �R���X�g���N�^
	///</summary>
	BasicPathData::BasicPathData(const double distance) : distance(distance)
	{
	}

	
	///<summary>
	/// �R���X�g���N�^
	///</summary>
	BasicPathData::BasicPathData(const Coordinate& c1, const Coordinate& c2)
	{
		distance = dist(c1, c2);
	}


	///<summary>
	/// �R���X�g���N�^
	///</summary>
	BasicPathData::BasicPathData(const Geography::LatLng& ll1, const Geography::LatLng& ll2)
	{
		distance = Geography::dist(ll1, ll2);
	}

	///<summary>
	/// �f�X�g���N�^
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
