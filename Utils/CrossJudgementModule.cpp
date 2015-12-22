#include "stdafx.h"
#include "CrossJudgementModule.h"

namespace Evaluate
{
	///<summary>
	/// コンストラクタ
	///</summary>
	template <typename MAP_TYPE, typename POSITION_TYPE, typename TRAJECTORY_TYPE, typename DUMMY_TYPE, typename USER_TYPE>
	CrossJudgementModule<MAP_TYPE, POSITION_TYPE, TRAJECTORY_TYPE, DUMMY_TYPE, USER_TYPE>::CrossJudgementModule(
		std::shared_ptr<MAP_TYPE const> map, 
		std::shared_ptr<Entity::EntityManager<POSITION_TYPE, TRAJECTORY_TYPE, DUMMY_TYPE, USER_TYPE> const> entities, 
		const std::function<bool(std::shared_ptr<MAP_TYPE const>, const Graph::MapNodeIndicator&, const Graph::MapNodeIndicator&, const Graph::MapNodeIndicator&, const Graph::MapNodeIndicator&)>& cross_rule
	)	: map(map), entities(entities), cross_rule(cross_rule)
	{
	}


	///<summary>
	/// デストラクタ
	///</summary>
	template <typename MAP_TYPE, typename POSITION_TYPE, typename TRAJECTORY_TYPE, typename DUMMY_TYPE, typename USER_TYPE>
	CrossJudgementModule<MAP_TYPE, POSITION_TYPE, TRAJECTORY_TYPE, DUMMY_TYPE, USER_TYPE>::~CrossJudgementModule()
	{
	}


	///<summary>
	/// 
	///</summary>
	template <typename MAP_TYPE, typename POSITION_TYPE, typename TRAJECTORY_TYPE, typename DUMMY_TYPE, typename USER_TYPE>
	std::vector<CrossInfo> CrossJudgementModule<MAP_TYPE, POSITION_TYPE, TRAJECTORY_TYPE, DUMMY_TYPE, USER_TYPE>::get_all_cross_info_of_entity(Entity::entity_id id) const
	{
		std::vector<CrossInfo> ret;
		std::shared_ptr<Entity::MobileEntity<POSITION_TYPE, TRAJECTORY_TYPE>> entity = entities->get_entity_by_id(id);
		std::shared_ptr<Time::TimeSlotManager const> timeslot = entities->read_timeslot();

		timeslot->for_each_time([&](time_t time, long interval, int phase) {
			if (phase == 0) return;
			
			//交差相手を格納する
			std::vector<Entity::entity_id> cross_entities;

			//指定エンティティの位置と1フェーズ前の位置
			Graph::MapNodeIndicator previous_node = entity->read_node_pos_info_of_phase(phase - 1).first;
			Graph::MapNodeIndicator current_node = entity->read_node_pos_info_of_phase(phase).first;
			for (Entity::entity_id target_id = 0; target_id <= entities->get_dummy_count(); target_id++) {
				//交差相手のエンティティ
				std::shared_ptr<Entity::MobileEntity<POSITION_TYPE, TRAJECTORY_TYPE>> target_entity = entities->get_entity_by_id(target_id);
				Graph::MapNodeIndicator target_previous_node = target_entity->read_node_pos_info_of_phase(phase - 1).first;
				Graph::MapNodeIndicator target_current_node = target_entity->read_node_pos_info_of_phase(phase).first;
			}
		});

		return ret;
	}

}

