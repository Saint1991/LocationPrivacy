#include "stdafx.h"
#include "BasicGeoNodeData.h"


///<summary>
/// �R���X�g���N�^
///</summary>
Geography::BasicGeoNodeData::BasicGeoNodeData(Geography::LatLng position) : position(std::make_unique<Geography::LatLng>(position))
{
}


///<summary>
/// �R���X�g���N�^
///</summary>
Geography::BasicGeoNodeData::BasicGeoNodeData(double latitude, double longitude) : position(std::make_unique<Geography::LatLng>(latitude, longitude))
{

}


///<summary>
/// �f�X�g���N�^
///</summary>
Geography::BasicGeoNodeData::~BasicGeoNodeData()
{
}

///<summary>
/// �ܓx�E�o�x���擾����
///</summary>
const Geography::LatLng Geography::BasicGeoNodeData::get_position() const
{
	return *this->position;
}
