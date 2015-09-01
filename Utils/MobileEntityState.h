#pragma once
#include "BasicMobileEntityState.h"

namespace Entity
{

	///<summary>
	/// ユーザ，及びダミーの状態を記述するデータクラス
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

	//明示的特殊化
	template class MobileEntityState<Graph::Coordinate>;
	template class MobileEntityState<Geography::LatLng>;
}


