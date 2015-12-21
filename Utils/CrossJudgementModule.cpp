#include "stdafx.h"
#include "CrossJudgementModule.h"

namespace Evaluate
{
	///<summary>
	/// コンストラクタ
	///</summary>
	template <typename POSITION_TYPE, typename TRAJECTORY_TYPE, typename DUMMY_TYPE, typename USER_TYPE>
	CrossJudgementModule<POSITION_TYPE, TRAJECTORY_TYPE, DUMMY_TYPE, USER_TYPE>::CrossJudgementModule(std::shared_ptr<Entity::EntityManager<DUMMY_TYPE, USER_TYPE, POSITION_TYPE> const> entities) : entities(entities)
	{
	}


	///<summary>
	/// デストラクタ
	///</summary>
	template <typename POSITION_TYPE, typename TRAJECTORY_TYPE, typename DUMMY_TYPE, typename USER_TYPE>
	CrossJudgementModule<POSITION_TYPE, TRAJECTORY_TYPE, DUMMY_TYPE, USER_TYPE>::~CrossJudgementModule()
	{
	}


	///<summary>
	/// 
	///</summary>
	template <typename POSITION_TYPE, typename TRAJECTORY_TYPE, typename DUMMY_TYPE, typename USER_TYPE>
	std::vector<CrossInfo> CrossJudgementModule<POSITION_TYPE, TRAJECTORY_TYPE, DUMMY_TYPE, USER_TYPE>::get_all_cross_info_of_entity(Entity::entity_id id) const
	{
		std::vector<CrossInfo> ret;
		std::shared_ptr<Entity::MobileEntity<POSITION_TYPE, TRAJECTORY_TYPE>> entity = id == 0 ? entities->get_user() : entities->read_dummy_by_id(id);

		for (int phase = 1; phase < entity->read_trajectory()->phase_count(); phase++) {
			
			
			for (Entity::entity_id target_id = 0; entities->get_dummy_count(); target_id++) {

			}

		}

		return ret;
	}

}

