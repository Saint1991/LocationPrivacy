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


///<summary>
/// �t�@�C���o�͌����̃n�b�V���f�[�^���쐬���܂��D
///</summary>
std::unordered_map<std::string, std::string> Geography::LatLng::get_export_data() const
{
	std::unordered_map<std::string, std::string> ret_data;
	ret_data.insert(std::make_pair("latitude", std::to_string(latitude)));
	ret_data.insert(std::make_pair("longitude", std::to_string(longitude)));
	return ret_data;
}


///<summary>
/// Lambert-Andoyer�̌����Ɋ�Â�2�_�Ԃ̋����v�Z [m]
///</summary>
double Geography::distance(const Geography::LatLng& ll1, const Geography::LatLng& ll2)
{
	return Geography::GeoCalculation::lambert_distance(ll1, ll2);
}


///<summary>
/// Lambert-Andoyer�̌����Ɋ�Â�from����to�֌������x�N�g���̕��ʊp [rad]
/// ����0�Ƃ��Ď��v����̍��W�n
///</summary>
double Geography::azimuth_angle(const Geography::LatLng& from, const Geography::LatLng& to)
{
	return Geography::GeoCalculation::lambert_azimuth_angle(from, to);
}
