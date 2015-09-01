#pragma once
#include "stdafx.h"
#include "Identifiable.h"
#include "Coordinate.h"
#include "LatLng.h"
#include "BasicMobileEntityState.h"

namespace Entity
{

	typedef unsigned int entity_id;

	///<summary>
	/// 移動体を表すクラス
	/// ユーザ，ダミーを表すのに用いるクラス (ここから派生して別個にUser, Dummyクラスを作ってもよい)
	///</summary>
	template <typename POSITION_TYPE, typename STATE_TYPE>
	class MobileEntity : public Identifiable<entity_id>
	{
	
	//STATE_TYPEがBasicMobileEntityStateのサブクラスかチェック
	static_assert(std::is_base_of<BasicMobileEntityState<POSITION_TYPE>, STATE_TYPE>::value, "template type STATE_TYPE must be derived from BasicMobileEntityState");
	static_assert(std::is_base_of<Graph::Coordinate, POSITION_TYPE>::value, "template type POSITION_TYPE must be derived from Coordinate");
	
	protected:
		std::shared_ptr<std::vector<std::shared_ptr<STATE_TYPE>>> past_states;
		void save_state(std::shared_ptr<STATE_TYPE> state);
	
	public:
		MobileEntity(entity_id id);
		virtual ~MobileEntity();

		
		const std::shared_ptr<POSITION_TYPE const> current_position() const;
		const std::vector<std::shared_ptr<POSITION_TYPE const>> previous_positions(unsigned int count) const;
	};
}

#include "MobileEntity.hpp"


