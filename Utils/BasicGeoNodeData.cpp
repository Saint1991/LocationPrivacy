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

	#pragma region Export

	///<summary>
	/// �t�@�C���G�N�X�|�[�g�p
	///</summary>
	std::unordered_map<std::string, std::string> BasicGeoNodeData::get_export_data() const
	{
		std::unordered_map<std::string, std::string> ret = {
			{LatLng::LATITUDE, std::to_string(position.lat())},
			{LatLng::LONGITUDE, std::to_string(position.lng())}
		};
		return ret;
	}

	#pragma endregion Export�֌W
}
