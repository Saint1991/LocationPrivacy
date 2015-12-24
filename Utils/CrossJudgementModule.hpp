
namespace Evaluate
{

	///<summary>
	/// �R���X�g���N�^
	///</summary>
	template <typename MAP_TYPE, typename POSITION_TYPE, typename TRAJECTORY_TYPE, typename DUMMY_TYPE, typename USER_TYPE>
	CrossJudgementModule<MAP_TYPE, POSITION_TYPE, TRAJECTORY_TYPE, DUMMY_TYPE, USER_TYPE>::CrossJudgementModule(
		std::shared_ptr<MAP_TYPE const> map,
		std::shared_ptr<Entity::EntityManager<POSITION_TYPE, TRAJECTORY_TYPE, DUMMY_TYPE, USER_TYPE> const> entities,
		double move_speed,
		const std::function<bool(std::shared_ptr<MAP_TYPE const>, const Graph::MapNodeIndicator&, const Graph::MapNodeIndicator&, const Graph::MapNodeIndicator&, const Graph::MapNodeIndicator&, double, long)>& cross_rule
		) : map(map), entities(entities), move_speed(move_speed), cross_rule(cross_rule)
	{
	}


	///<summary>
	/// �f�X�g���N�^
	///</summary>
	template <typename MAP_TYPE, typename POSITION_TYPE, typename TRAJECTORY_TYPE, typename DUMMY_TYPE, typename USER_TYPE>
	CrossJudgementModule<MAP_TYPE, POSITION_TYPE, TRAJECTORY_TYPE, DUMMY_TYPE, USER_TYPE>::~CrossJudgementModule()
	{
	}

	///<summary>
	/// ��{�̌������胋�[��
	///</summary>
	template <typename MAP_TYPE, typename POSITION_TYPE, typename TRAJECTORY_TYPE, typename DUMMY_TYPE, typename USER_TYPE>
	bool CrossJudgementModule<MAP_TYPE, POSITION_TYPE, TRAJECTORY_TYPE, DUMMY_TYPE, USER_TYPE>::basic_cross_rule(std::shared_ptr<MAP_TYPE const>, const Graph::MapNodeIndicator& previous_position1, const Graph::MapNodeIndicator& current_position1, const Graph::MapNodeIndicator& previous_position2, const Graph::MapNodeIndicator& current_position2, double move_speed, long interval) const
	{
		return map->is_reachable(previous_position1, current_position2, move_speed, (double)interval) && map->is_reachable(previous_position2, current_position1, move_speed, (double)interval);
	}

	///<summary>
	/// �w�肵���G���e�B�e�B�̌�������S�Ď擾����
	///</summary>
	template <typename MAP_TYPE, typename POSITION_TYPE, typename TRAJECTORY_TYPE, typename DUMMY_TYPE, typename USER_TYPE>
	std::vector<CrossInfo> CrossJudgementModule<MAP_TYPE, POSITION_TYPE, TRAJECTORY_TYPE, DUMMY_TYPE, USER_TYPE>::get_all_cross_info_of_entity(Entity::entity_id id) const
	{
		std::vector<CrossInfo> ret;
		std::shared_ptr<Entity::MobileEntity<POSITION_TYPE, TRAJECTORY_TYPE>> entity = entities->get_entity_by_id(id);
		std::shared_ptr<Time::TimeSlotManager const> timeslot = entities->read_timeslot();

		timeslot->for_each_time([&](time_t time, long interval, int phase) {
			if (phase == 0) return;

			//����������i�[����
			std::vector<Entity::entity_id> cross_entities;

			//�w��G���e�B�e�B�̈ʒu��1�t�F�[�Y�O�̈ʒu
			Graph::MapNodeIndicator previous_node = entity->read_node_pos_info_of_phase(phase - 1).first;
			Graph::MapNodeIndicator current_node = entity->read_node_pos_info_of_phase(phase).first;
			for (Entity::entity_id target_id = 0; target_id <= entities->get_dummy_count(); target_id++) {
				//��������̃G���e�B�e�B
				std::shared_ptr<Entity::MobileEntity<POSITION_TYPE, TRAJECTORY_TYPE>> target_entity = entities->get_entity_by_id(target_id);
				Graph::MapNodeIndicator target_previous_node = target_entity->read_node_pos_info_of_phase(phase - 1).first;
				Graph::MapNodeIndicator target_current_node = target_entity->read_node_pos_info_of_phase(phase).first;

				//���������̔���
				if (cross_rule(map, previous_node, current_node, target_previous_node, target_current_node, move_speed, interval)) {
					cross_entities.push_back(target_id);
				}
			}
			ret.push_back(CrossInfo(phase, cross_entities));
		});

		return ret;
	}

}

