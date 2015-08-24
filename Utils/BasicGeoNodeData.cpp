#include "stdafx.h"
#include "BasicGeoNodeData.h"

namespace Geography
{
	///<summary>
	/// �R���X�g���N�^
	///</summary>
	BasicGeoNodeData::BasicGeoNodeData( LatLng position) : position(position)
	{
	}


	///<summary>
	/// �R���X�g���N�^
	///</summary>
	BasicGeoNodeData::BasicGeoNodeData(double latitude, double longitude) : position(LatLng(latitude, longitude))
	{

	}


	///<summary>
	/// �f�X�g���N�^
	///</summary>
	BasicGeoNodeData::~BasicGeoNodeData()
	{
	}

	///<summary>
	/// �ܓx�E�o�x���擾����
	///</summary>
	LatLng BasicGeoNodeData::get_position() const
	{
		return this->position;
	}

}
