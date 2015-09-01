#pragma once
#include "BasicMobileEntityState.h"

namespace Entity
{
	template <typename POSITION_TYPE>
	class MobileEntityState : public BasicMobileEntityState<POSITION_TYPE>
	{
	protected:
		std::string category_id;

	public:
		MobileEntityState(const POSITION_TYPE& position, double user_probability, const std::string& category_id);
		virtual ~MobileEntityState();

		const std::string get_category_id() const;
	};

	//–¾Ž¦“I“ÁŽê‰»
	template class MobileEntityState<Graph::Coordinate>;
	template class MobileEntityState<Geography::LatLng>;
}


