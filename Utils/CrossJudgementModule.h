#pragma once
#include "stdafx.h"
#include "Coordinate.h"
#include "LatLng.h"
#include "EntityManager.h"
#include "CrossInfo.h"
#include "SequentialTree.h"
#include "ObservedTrajectoryStructure.h"
#include "ObservedTrajectoryNode.h"

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
		std::function<bool(std::shared_ptr<MAP_TYPE const>, const Graph::MapNodeIndicator&, const Graph::MapNodeIndicator&, const Graph::MapNodeIndicator&, const Graph::MapNodeIndicator&, double, long)> cross_rule;
		double move_speed;

		bool basic_cross_rule(std::shared_ptr<MAP_TYPE const>, const Graph::MapNodeIndicator& previous_position1, const Graph::MapNodeIndicator& current_position1, const Graph::MapNodeIndicator& previous_position2, const Graph::MapNodeIndicator& current_position2, double move_speed, long interval) const;
	
	public:
		CrossJudgementModule(
			std::shared_ptr<MAP_TYPE const> map, 
			std::shared_ptr<Entity::EntityManager<POSITION_TYPE, TRAJECTORY_TYPE, DUMMY_TYPE, USER_TYPE> const> entities,
			double move_speed,
			const std::function<bool(std::shared_ptr<MAP_TYPE const>, const Graph::MapNodeIndicator&, const Graph::MapNodeIndicator&, const Graph::MapNodeIndicator&, const Graph::MapNodeIndicator&, double, long)>& cross_rule = nullptr
		);
		~CrossJudgementModule();
		
		std::vector<CrossInfo> get_all_cross_info_of_entity(Entity::entity_id id) const;
		std::vector<CrossInfo> get_all_cross_info_of_phase(int phase) const;
	};
}

#include "CrossJudgementModule.hpp"


