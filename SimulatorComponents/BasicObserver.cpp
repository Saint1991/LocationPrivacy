#include "stdafx.h"
#include "BasicObserver.h"

namespace Observer
{


	///<summary>
	/// �R���X�g���N�^
	///</summary>
	template <typename TRAJECTORY_TYPE, typename DUMMY_TYPE, typename USER_TYPE>
	BasicObserver<TRAJECTORY_TYPE, DUMMY_TYPE, USER_TYPE>::BasicObserver (
		std::shared_ptr<Map::BasicDbMap const> map,
		std::shared_ptr<Entity::EntityManager<Geography::LatLng, TRAJECTORY_TYPE, DUMMY_TYPE, USER_TYPE> const> entities,
		double move_speed,
		const std::function<bool(std::shared_ptr<Map::BasicDbMap const>, const Graph::MapNodeIndicator&, const Graph::MapNodeIndicator&, const Graph::MapNodeIndicator&, const Graph::MapNodeIndicator&, double, long)>& cross_rule
	) : module(std::make_unique<Evaluate::CrossJudgementModule<Map::BasicDbMap, Geography::LatLng, TRAJECTORY_TYPE, DUMMY_TYPE, USER_TYPE>>(map, entities, move_speed, cross_rule)), entities(entities), map(map)
	{
	}


	///<summary>
	/// �f�X�g���N�^
	///</summary>
	template <typename TRAJECTORY_TYPE, typename DUMMY_TYPE, typename USER_TYPE>
	BasicObserver<TRAJECTORY_TYPE, DUMMY_TYPE, USER_TYPE>::~BasicObserver()
	{
	}


	///<summary>
	/// �g���W�F�N�g���Ƃ��̌����֌W����Ɋϑ�����邷�ׂẴg���W�F�N�g�����f�����쐬����
	///</summary>
	template <typename TRAJECTORY_TYPE, typename DUMMY_TYPE, typename USER_TYPE>
	std::shared_ptr<ObservedTrajectoryStructure> BasicObserver<TRAJECTORY_TYPE, DUMMY_TYPE, USER_TYPE>::create_observed_trajectory_structure() const
	{
		std::shared_ptr<ObservedTrajectoryStructure> trajectory_structure = std::make_shared<ObservedTrajectoryStructure>();
		ObservedTrajectoryStructure::base_iterator iter = trajectory_structure->root<ObservedTrajectoryStructure::base_iterator>();

		//�܂��͌������l�����ɌX�̉ϑ��ȃg���W�F�N�g�������쐬����
		std::shared_ptr<Time::TimeSlotManager const> timeslot = entities->read_timeslot();
		timeslot->for_each_time([&](time_t time, long interval, int phase) {

			int depth = phase + 1;

			for (Entity::entity_id id = 0; id <= entities->get_dummy_count(); id++) {

				//���ڂ��Ă���G���e�B�e�B�ɉ����Đe�m�[�h���X�V
				std::shared_ptr<Entity::MobileEntity<Geography::LatLng, Graph::SemanticTrajectory<Geography::LatLng>> const> entity = entities->read_entity_by_id(id);
				if (phase >= 1) {
					Graph::MapNodeIndicator previous_node = entity->read_node_pos_info_of_phase(phase - 1).first;
					iter = trajectory_structure->find_node(previous_node, phase - 1);
				}
				else if (phase == 0) iter->count_up();

				//���̖K��n�_���G���e�B�e�B��1�Ŏq�m�[�h�Ƃ��Đڑ�����
				//�����̃m�[�h�̏ꍇ�̓G���e�B�e�B�����J�E���g�A�b�v����
				Graph::MapNodeIndicator current_node = entity->read_node_pos_info_of_phase(phase).first;
				ObservedTrajectoryStructure::base_iterator child = iter.find_child_if([&current_node](std::shared_ptr<ObservedTrajectoryNode const> node) {
					return *node->data == current_node;
				});
				if (*child == nullptr) {
					std::shared_ptr<ObservedTrajectoryNode> new_node = std::make_shared<ObservedTrajectoryNode>(trajectory_structure->node_count(), depth, std::make_shared<Graph::MapNodeIndicator>(current_node));
					iter.add_child(new_node);
				}
				else {
					child->count_up();
				}
			}
		});

		//�����������Ɏ�肤��g���W�F�N�g���̐ڑ��֌W��ǉ�����
		for (Entity::entity_id id = 0; id <= entities->get_dummy_count(); id++) {

			//���ڂ���G���e�B�e�B�Ƃ��̌������
			std::shared_ptr<Entity::MobileEntity<Geography::LatLng, Graph::SemanticTrajectory<Geography::LatLng>> const> entity = entities->read_entity_by_id(id);
			std::vector<Evaluate::CrossInfo> cross_infos = module->get_all_cross_info_of_entity(id);

			//��������̒H�����m�[�h�ւ̃p�X���Ȃ��ꍇ�͂�����쐬����
			for (std::vector<Evaluate::CrossInfo>::const_iterator cross_info = cross_infos.begin(); cross_info != cross_infos.end(); cross_info++) {

				//�������쐬����trajectory_node���擾����
				int cross_phase = cross_info->phase;
				Graph::MapNodeIndicator cross_node = entity->read_node_pos_info_of_phase(cross_phase).first;
				ObservedTrajectoryStructure::base_iterator iter = trajectory_structure->find_node(cross_node, cross_info->phase);

				//��������̃m�[�h���擾��trajectory_node�ɃG�b�W��ǉ�
				for (std::vector<Entity::entity_id>::const_iterator target_entity_id = cross_info->crossing_entities.begin(); target_entity_id != cross_info->crossing_entities.end(); target_entity_id++) {

					//�G�b�W�𒣂��̃m�[�h��ID���擾(���������cross_phase + 1�̖K��ꏊ)
					Graph::MapNodeIndicator next_node = entities->read_entity_by_id(id)->read_node_pos_info_of_phase(cross_phase + 1).first;
					Graph::node_id next_node_id = trajectory_structure->find_node_id(next_node, cross_phase + 1);

					if (!iter->is_connecting_to(next_node_id)) iter->connect_to(std::make_shared<Graph::BasicEdge>(next_node_id));
				}
			}
		}
		return trajectory_structure;
	}
}

