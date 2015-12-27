
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
		std::shared_ptr<Entity::MobileEntity<POSITION_TYPE, TRAJECTORY_TYPE> const> entity = entities->read_entity_by_id(id);
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
				std::shared_ptr<Entity::MobileEntity<POSITION_TYPE, TRAJECTORY_TYPE> const> target_entity = entities->read_entity_by_id(target_id);
				Graph::MapNodeIndicator target_previous_node = target_entity->read_node_pos_info_of_phase(phase - 1).first;
				Graph::MapNodeIndicator target_current_node = target_entity->read_node_pos_info_of_phase(phase).first;

				//���������̔���
				if (cross_rule(map, previous_node, current_node, target_previous_node, target_current_node, move_speed, interval)) {
					cross_entities.push_back(target_id);
				}
			}
			ret.push_back(CrossInfo(phase - 1, id, cross_entities));
		});

		return ret;
	}


	///<summary>
	/// �Ώۂ̃G���e�B�e�B�̎�肤��g���W�F�N�g���̖؍\�����擾����
	/// phase��last_phase�̏ꍇ�͋��vector��Ԃ�
	///</summary>
	template <typename MAP_TYPE, typename POSITION_TYPE, typename TRAJECTORY_TYPE, typename DUMMY_TYPE, typename USER_TYPE>
	std::vector<CrossInfo> CrossJudgementModule<MAP_TYPE, POSITION_TYPE, TRAJECTORY_TYPE, DUMMY_TYPE, USER_TYPE>::get_all_cross_info_of_phase(int phase) const
	{

		std::shared_ptr<Time::TimeSlotManager const> timeslot = entities->read_timeslot();
		long interval = timeslot->time_of_phase(phase + 1) - timeslot->time_of_phase(phase);
		int last_phase = timeslot->phase_count() - 1;

		if (phase == last_phase) return std::vector<CrossInfo>();

		//�ΏۃG���e�B�e�B�̃g���W�F�N�g��
		std::vector<CrossInfo> ret(entities->get_dummy_count() + 1);
		for (Entity::entity_id id = 0; id <= entities->get_dummy_count(); id++) {
			
			std::shared_ptr<Entity::MobileEntity<POSITION_TYPE, TRAJECTORY_TYPE> const> entity = entities->read_entity_by_id(id);
			Graph::MapNodeIndicator current_node = entity->read_node_pos_info_of_phase(phase).first;
			Graph::MapNodeIndicator next_node = entity->read_node_pos_info_of_phase(phase + 1).first;

			std::vector<Entity::entity_id> cross_entities;

			for (Entity::entity_id target_id = 0; target_id <= entities->get_dummy_count(); target_id++) {
				if (id == target_id) continue;
				
				std::shared_ptr<Entity::MobileEntity<POSITION_TYPE, TRAJECTORY_TYPE> const> target_entity = entities->read_entity_by_id(target_id);
				Graph::MapNodeIndicator current_target_node = target_entity->read_node_pos_info_of_phase(phase).first;
				Graph::MapNodeIndicator next_target_node = target_entity->read_node_pos_info_of_phase(phase + 1).first;

				if (cross_rule(map, current_node, next_node, current_target_node, next_target_node, move_speed, interval)) {
					cross_entities.push_back(target_id);
				}
			}
			ret.at(id) = CrossInfo(phase, id, cross_entities);
		}
		return ret;
	}

}

