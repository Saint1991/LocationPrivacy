#include "stdafx.h"
#include "LatLng.h"

///<summary>
/// �R���X�g���N�^
/// �ܓx��[-90, 90]�łȂ��ꍇ�C�o�x��[-180, 180]�łȂ��ꍇ��invalid_argument���X���[���܂��D
///</summary>
Geography::LatLng::LatLng(double latitude, double longitude)
{
	set_lat(latitude);
	set_lng(longitude);
}

///<summary>
/// �f�X�g���N�^
///</summary>
Geography::LatLng::~LatLng()
{
}


///<summary>
/// �ܓx��ݒ肵�܂��D
/// [-90, 90]�łȂ��ꍇ��invalid_argument���X���[���܂��D
///</summary>
void Geography::LatLng::set_lat(double latitude)
{
	if (latitude < -90.0 || 90.0 < latitude) {
		throw std::invalid_argument("Latitude must be within the range [-90, 90]");
	}
	this->latitude = latitude;
}


///<summary>
/// �o�x��ݒ肵�܂��D
/// [-180, 180]�łȂ��ꍇ��invalid_argument���X���[���܂��D
///</summary>
void Geography::LatLng::set_lng(double longitude)
{
	if (longitude < -180.0 || 180.0 < longitude) {
		throw std::invalid_argument("Longitude must be within the range [-180, 180]");
	}
	this->longitude = longitude;
}


///<summary>
/// �ܓx���擾���܂�
///</summary>
double Geography::LatLng::lat() const
{
	return latitude;
}


///<summary>
/// �o�x���擾���܂�
///</summary>
double Geography::LatLng::lng() const
{
	return longitude;
}