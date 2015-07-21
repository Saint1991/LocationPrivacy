#pragma once
#include "LatLng.h"

namespace Entity{

	///<summary>
	/// �ړ��̂�\���N���X
	/// ���[�U�C�_�~�[�̊��N���X
	///</summary>
	class MobileEntity
	{
	protected:
		std::unique_ptr<std::list<std::shared_ptr<Geography::LatLng>>> position_history;
	
	public:
		MobileEntity();
		virtual ~MobileEntity();

	};
}


