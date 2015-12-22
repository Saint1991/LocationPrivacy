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
	template <typename MAP_TYPE, typename POSITION_TYPE = Geography::LatLng, typename TRAJECTORY_TYPE = Graph::SemanticTrajectory<POSITION_TYPE>, typename DUMMY_TYPE = Entity::MobileEntity<POSITION_TYPE, TRAJECTORY_TYPE>, typename USER_TYPE = Entity::MobileEntity<POSITION_TYPE, TRAJECTORY_TYPE>>
	class CrossJudgementModule
	{
	protected:
		std::shared_ptr<Entity::EntityManager<POSITION_TYPE, TRAJECTORY_TYPE, DUMMY_TYPE, USER_TYPE> const> entities;
		std::shared_ptr<MAP_TYPE const> map;
		std::function<bool(std::shared_ptr<MAP_TYPE const>, const Graph::MapNodeIndicator& previous_position1, const Graph::MapNodeIndicator& current_position1, const Graph::MapNodeIndicator& previous_position2, const Graph::MapNodeIndicator& current_position2)> cross_rule;
	
	public:
		CrossJudgementModule(
			std::shared_ptr<MAP_TYPE const> map, 
			std::shared_ptr<Entity::EntityManager<POSITION_TYPE, TRAJECTORY_TYPE, DUMMY_TYPE, USER_TYPE> const> entities,
			const std::function<bool(std::shared_ptr<MAP_TYPE const>, const Graph::MapNodeIndicator&, const Graph::MapNodeIndicator&, const Graph::MapNodeIndicator&, const Graph::MapNodeIndicator&)>& cross_rule = nullptr
		);
		~CrossJudgementModule();
		
		std::vector<CrossInfo> get_all_cross_info_of_entity(Entity::entity_id id) const;
	};
}


