#pragma once
#include "BasicMobileEntityState.h"

namespace Entity
{

	///<summary>
	/// ���[�U�C�y�у_�~�[�̏�Ԃ��L�q����f�[�^�N���X
	///</summary>
	template <typename POSITION_TYPE>
	class MobileEntityState : public BasicMobileEntityState<POSITION_TYPE>
	{
	protected:
		std::string category_id;

	public:
		static const std::string CATEGORY_ID;

		MobileEntityState(const POSITION_TYPE& position, const std::string& category_id);
		virtual ~MobileEntityState();

		const std::string get_category_id() const;

		std::unordered_map<std::string, std::string> get_export_data() const;
	};

	//�����I���ꉻ
	template class MobileEntityState<Graph::Coordinate>;
	template class MobileEntityState<Geography::LatLng>;
}


