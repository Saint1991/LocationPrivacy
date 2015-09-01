#include "stdafx.h"
#include "MobileEntityState.h"

namespace Entity
{

	///<summary>
	/// �R���X�g���N�^
	///</summary>
	template <typename POSITION_TYPE>
	MobileEntityState<POSITION_TYPE>::MobileEntityState(const POSITION_TYPE& position, const std::string& category_id) 
		: BasicMobileEntityState<POSITION_TYPE>(position)
	{
	}


	///<summary>
	/// �f�X�g���N�^
	///</summary>
	template <typename POSITION_TYPE>
	MobileEntityState<POSITION_TYPE>::~MobileEntityState()
	{
	}


	///<summary>
	/// �J�e�S��ID���擾����
	///</summary>
	template <typename POSITION_TYPE>
	const std::string MobileEntityState<POSITION_TYPE>::get_category_id() const
	{
		return category_id;
	}

	#pragma region Export
	template <typename POSITION_TYPE>
	const std::string MobileEntityState<POSITION_TYPE>::CATEGORY_ID = "Category ID";
	
	template <typename POSITION_TYPE>
	std::unordered_map<std::string, std::string> MobileEntityState<POSITION_TYPE>::get_export_data() const
	{
		std::unordered_map<std::string, std::string> ret = BasicMobileEntityState<POSITION_TYPE>::get_export_data();
		ret.insert(std::make_pair(CATEGORY_ID, category_id));
		return ret;
	}
	#pragma endregion Export�֘A
}
