#pragma once
#include "Coordinate.h"
#include "LatLng.h"
#include "EntityManager.h"
#include "CrossInfo.h"

namespace Evaluate
{


	///<summary>
	/// 交差判定関連の処理をまとめたモジュール
	/// USER_TYPEとDUMMY_TYPEはともに同じMobileEntityから派生したクラスである必要がある
	///</summary>
	template <typename POSITION_TYPE = Geography::LatLng, typename TRAJECTORY_TYPE = Graph::SemanticTrajectory<POSITION_TYPE>, typename DUMMY_TYPE = Entity::MobileEntity<POSITION_TYPE, TRAJECTORY_TYPE>, typename USER_TYPE = Entity::MobileEntity<POSITION_TYPE, TRAJECTORY_TYPE>>
	class CrossJudgementModule
	{
	protected:
		std::shared_ptr<Entity::EntityManager<DUMMY_TYPE, USER_TYPE, POSITION_TYPE> const> entities;
	public:
		CrossJudgementModule(std::shared_ptr<Entity::EntityManager<DUMMY_TYPE, USER_TYPE, POSITION_TYPE> const> entities);
		~CrossJudgementModule();
		
		std::vector<CrossInfo> get_all_cross_info_of_entity(Entity::entity_id id) const;
	};
}


