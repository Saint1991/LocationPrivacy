#include "stdafx.h"
#include "LatLng.h"


namespace Geography 
{
	///<summary>
	/// �R���X�g���N�^
	/// �ܓx��[-90, 90]�łȂ��ꍇ�C�o�x��[-180, 180]�łȂ��ꍇ��invalid_argument���X���[���܂��D
	///</summary>
	LatLng::LatLng(double latitude, double longitude) : Graph::Coordinate(longitude, latitude)
	{
		is_valid = validate(latitude, longitude);
	}

	///<summary>
	/// �f�X�g���N�^
	///</summary>
	LatLng::~LatLng()
	{
	}

	///<summary>
	/// �ܓx�o�x�̒l���`�F�b�N���܂��D
	/// �s���Ȓl�̏ꍇinvalid_argument���X���[����false��Ԃ��܂�
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
	/// �ܓx���擾���܂�
	///</summary>
	double LatLng::lat() const
	{
		return _y;
	}


	///<summary>
	/// �o�x���擾���܂�
	///</summary>
	double LatLng::lng() const
	{
		return _x;
	}


	///<summary>
	/// �s���Ȓl�ō\������Ă��邩�ǂ�����Ԃ�
	///</summary>
	bool LatLng::is_valid_point() const
	{
		return is_valid;
	}

	#pragma region Export
	
	const std::string LatLng::LATITUDE = "latitude";
	const std::string LatLng::LONGITUDE = "longitude";

	///<summary>
	/// �t�@�C���o�͌����̃n�b�V���f�[�^���쐬���܂��D
	///</summary>
	std::unordered_map<std::string, std::string> LatLng::get_export_data() const
	{
		std::unordered_map<std::string, std::string> ret = {
			{LATITUDE, std::to_string(_y)},
			{LONGITUDE, std::to_string(_x)}
		};
		return ret;
	}

	#pragma endregion Export�֌W

	///<summary>
	/// Lambert-Andoyer�̌����Ɋ�Â�2�_�Ԃ̋����v�Z [m]
	///</summary>
	double dist(const LatLng& ll1, const LatLng& ll2)
	{
		return GeoCalculation::lambert_distance(ll1, ll2);
	}


	///<summary>
	/// Lambert-Andoyer�̌����Ɋ�Â�from����to�֌������x�N�g���̕��ʊp [rad]
	/// ����0�Ƃ��Ď��v����̍��W�n
	///</summary>
	double angle(const LatLng& from, const LatLng& to)
	{
		return GeoCalculation::lambert_azimuth_angle(from, to);
	}

}

