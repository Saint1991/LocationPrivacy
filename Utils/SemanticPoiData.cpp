#include "stdafx.h"
#include "SemanticPoiData.h"

namespace Geography
{


	///<summary>
	/// �R���X�g���N�^
	///</summary>
	PoiData::PoiData(const std::string& venue_name, const std::string& category_id, const std::string& category_name, const LatLng& position)
		: BasicPoiData(venue_name, position), category_id(category_id), category_name(category_name)
	{

	}

	///<summary>
	/// �R���X�g���N�^
	///</summary>
	PoiData::PoiData(const std::string& venue_name, const std::string& category_id, const std::string& category_name, double latitude, double longitude)
		: BasicPoiData(venue_name, latitude, longitude), category_id(category_id), category_name(category_name)
	{
	}


	///<summary>
	/// �f�X�g���N�^
	///</summary>
	PoiData::~PoiData()
	{
	}


	///<summary>
	/// �J�e�S��ID�̎擾
	///</summary>
	const std::string PoiData::get_category_id() const
	{
		return category_id;
	}


	///<summary>
	/// �J�e�S�����̎擾
	///</summary>
	const std::string PoiData::get_category_name() const
	{
		return category_name;
	}


	#pragma region Export

	const std::string PoiData::CATEGORY_ID = "category_id";
	const std::string PoiData::CATEGORY_NAME = "category_name";

	///<summary>
	/// �t�@�C���G�N�X�|�[�g�p
	///</summary>
	std::unordered_map<std::string, std::string> PoiData::get_export_data() const
	{
		std::unordered_map<std::string, std::string> ret = {
			{BasicPoiData::VENUE_NAME, venue_name},
			{CATEGORY_ID, category_id},
			{CATEGORY_NAME, category_name},
			{LatLng::LATITUDE, std::to_string(position.lat())},
			{LatLng::LONGITUDE, std::to_string(position.lng())}
		};
		return ret;
	}

	#pragma endregion �t�@�C���G�N�X�|�[�g�֌W
}

