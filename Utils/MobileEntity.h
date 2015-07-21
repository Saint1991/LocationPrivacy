#pragma once
#include "LatLng.h"

namespace Entity{

	///<summary>
	/// 移動体を表すクラス
	/// ユーザ，ダミーの基底クラス
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


