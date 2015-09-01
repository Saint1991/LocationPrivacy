#include "stdafx.h"
#include "BasicMobileEntityState.h"

namespace Entity 
{

	///<summary>
	/// �R���X�g���N�^
	///</summary>
	template <typename POSITION_TYPE>
	BasicMobileEntityState<POSITION_TYPE>::BasicMobileEntityState(const POSITION_TYPE& position) 
		: position(std::make_shared<POSITION_TYPE>(position))
	{
	}


	///<summary>
	/// �f�X�g���N�^
	///</summary>
	template <typename POSITION_TYPE>
	BasicMobileEntityState<POSITION_TYPE>::~BasicMobileEntityState()
	{
	}


	///<summary>
	/// �ʒu���擾����
	///</summary>
	template <typename POSITION_TYPE>
	const std::shared_ptr<POSITION_TYPE const> BasicMobileEntityState<POSITION_TYPE>::read_position() const
	{
		return position;
	}


	#pragma region Export
	template <typename POSITION_TYPE>
	std::unordered_map<std::string, std::string> BasicMobileEntityState<POSITION_TYPE>::get_export_data() const
	{
		return position->get_export_data();
	}
	#pragma endregion Export�֘A
}

