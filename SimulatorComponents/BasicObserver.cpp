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
	) : module(std::make_unique<Evaluate::CrossJudgementModule<Map::BasicDbMap, Geography::LatLng, TRAJECTORY_TYPE, DUMMY_TYPE, USER_TYPE>>(map, entities, move_speed, cross_rule)), entities(entities), map(map), structure(nullptr), cross_infos(nullptr), confusion_total_count(0), cunfusion_achive_count(0)
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
	/// �S������CrossInfo���v�Z����
	///</summary>
	template<typename TRAJECTORY_TYPE, typename DUMMY_TYPE, typename USER_TYPE>
	std::shared_ptr<std::vector<std::vector<Evaluate::CrossInfo>>> BasicObserver<TRAJECTORY_TYPE, DUMMY_TYPE, USER_TYPE>::calc_cross_info()
	{
		cross_infos = std::make_shared<std::vector<std::vector<Evaluate::CrossInfo>>>();
		std::shared_ptr<Time::TimeSlotManager const> timeslot = entities->read_timeslot();
		for (int phase = 0; phase < timeslot->phase_count() - 1; phase++) {
			std::cout << "Calculating Cross Info of phase " << std::to_string(phase) << std::endl;
			cross_infos->push_back(module->get_all_cross_info_of_phase(phase));
		}
		return cross_infos;
	}


	///<summary>
	/// �w�肵���G���e�B�e�B��CrossInfo��O��Phase���珇�Ɏ擾����
	///</summary>
	template <typename TRAJECTORY_TYPE, typename DUMMY_TYPE, typename USER_TYPE>
	std::vector<Evaluate::CrossInfo> BasicObserver<TRAJECTORY_TYPE, DUMMY_TYPE, USER_TYPE>::get_cross_info_of_entity(Entity::entity_id id)
	{
		if (cross_infos == nullptr) calc_cross_info();
		
		std::vector<Evaluate::CrossInfo> ret;
		for (std::vector<std::vector<Evaluate::CrossInfo>>::const_iterator iter = cross_infos->begin(); iter != cross_infos->end(); iter++) {
			ret.push_back(iter->at(id));
		}
		return ret;
	}

	///<summary>
	/// �g���W�F�N�g���Ƃ��̌����֌W����Ɋϑ�����邷�ׂẴg���W�F�N�g�����f�����쐬����
	///</summary>
	template <typename TRAJECTORY_TYPE, typename DUMMY_TYPE, typename USER_TYPE>
	std::shared_ptr<ObservedTrajectoryStructure const> BasicObserver<TRAJECTORY_TYPE, DUMMY_TYPE, USER_TYPE>::create_observed_trajectory_structure()
	{
		if (structure != nullptr) return structure;

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
				ObservedTrajectoryStructure::base_iterator child = trajectory_structure->find_node(current_node, phase);
				if (*child == nullptr) {
					std::shared_ptr<ObservedTrajectoryNode> new_node = std::make_shared<ObservedTrajectoryNode>(trajectory_structure->node_count(), depth, std::make_shared<Graph::MapNodeIndicator>(current_node));
					new_node->count_up();
					iter.add_child(new_node, 1.0);
				}
				else {
					iter->connect_to(std::make_shared<Graph::FlowEdge>(child->get_id(), 1.0));
					child->count_up();
				}
			}
		});

		if (cross_infos == nullptr) calc_cross_info();

		//���������ȉ��͌����ɂ͊Ԉ���Ă���C��ԑJ�ڂ̑g�ݍ��킹�������K�v������
		//�����������Ɏ�肤��g���W�F�N�g���̐ڑ��֌W��ǉ�����
		for (Entity::entity_id id = 0; id <= entities->get_dummy_count(); id++) {

			//���ڂ���G���e�B�e�B�Ƃ��̌������
			std::shared_ptr<Entity::MobileEntity<Geography::LatLng, Graph::SemanticTrajectory<Geography::LatLng>> const> entity = entities->read_entity_by_id(id);
			std::vector<Evaluate::CrossInfo> cross_infos_of_entity = get_cross_info_of_entity(id);
			
			//��������̒H�����m�[�h�ւ̃p�X���Ȃ��ꍇ�͂�����쐬����
			for (std::vector<Evaluate::CrossInfo>::const_iterator cross_info = cross_infos_of_entity.begin(); cross_info != cross_infos_of_entity.end(); cross_info++) {

				//�������쐬����trajectory_node���擾����
				int cross_phase = cross_info->phase;
				Graph::MapNodeIndicator cross_node = entity->read_node_pos_info_of_phase(cross_phase).first;
				ObservedTrajectoryStructure::base_iterator iter = trajectory_structure->find_node(cross_node, cross_info->phase);

				Graph::node_id next_node_id = trajectory_structure->find_node_id(entity->read_node_pos_info_of_phase(cross_phase + 1).first, cross_phase + 1);
				if (next_node_id == -1) throw std::invalid_argument("Node not found.");
				
				//��������̃m�[�h���ǂ�POI�ɂ����ړ����邩���v�Z����
				std::unordered_map<Graph::node_id, size_t> next_visit_map = { {next_node_id, 1U} };
				std::vector<Entity::entity_id> crossing_entities = cross_info->crossing_entities;
				for (std::vector<Entity::entity_id>::const_iterator target_entity_id = crossing_entities.begin(); target_entity_id != crossing_entities.end(); target_entity_id++) {
					next_node_id = trajectory_structure->find_node_id(entities->read_entity_by_id(*target_entity_id)->read_node_pos_info_of_phase(cross_phase + 1).first, cross_phase + 1);
					if (next_node_id == -1) throw std::invalid_argument("Node not found.");
					
					if (next_visit_map.find(next_node_id) == next_visit_map.end()) next_visit_map.insert({ next_node_id, 1U });
					else next_visit_map.at(next_node_id) += 1;
				}

				double visit_sum = 0.0;
				std::for_each(next_visit_map.begin(), next_visit_map.end(), [&visit_sum](const std::pair<Graph::node_id, size_t>& element) {
					visit_sum +=element.second;
				});

				//��̏������ɃG�b�W�𒣂�
				for (std::unordered_map<Graph::node_id, size_t>::const_iterator visit_count_iter = next_visit_map.begin(); visit_count_iter != next_visit_map.end(); visit_count_iter++) {
					Graph::node_id to = visit_count_iter->first;
					double probability = (double)visit_count_iter->second / visit_sum;
					iter->is_connecting_to(to) ? iter->get_edge_to(to)->set_flow(probability) : iter->flow_out_to(to, probability);
				}
			}
		}

		trajectory_structure->normalize_all_edges();
		structure = trajectory_structure;
		return trajectory_structure;
	}


	///<summary>
	/// ID��id�̃G���e�B�e�B���ړ��������g���W�F�N�g���Ƃ��̊m���̑g�ɂ���execute_function�����s����
	///</summary>
	template <typename TRAJECTORY_TYPE, typename DUMMY_TYPE, typename USER_TYPE>
	void BasicObserver<TRAJECTORY_TYPE, DUMMY_TYPE, USER_TYPE>::for_each_possible_trajectory_probability_of_entity(Entity::entity_id id, const std::function<void(const Collection::Sequence<Graph::MapNodeIndicator>, double)>& execute_function)
	{
		if (structure == nullptr) create_observed_trajectory_structure();
		std::shared_ptr<Entity::MobileEntity<Geography::LatLng, TRAJECTORY_TYPE> const> entity = entities->read_entity_by_id(id);
		Graph::MapNodeIndicator first_node = entity->read_node_pos_info_of_phase(0).first;
		std::shared_ptr<ObservedTrajectoryStructure const> sub = structure->sub_structure(first_node, 0);
		sub->for_each_possible_trajectory([&execute_function, &sub](const Collection::Sequence<Graph::MapNodeIndicator>& trajectory) {
			double probability = sub->calc_probability_of_trajectory(trajectory);
			execute_function(trajectory, probability);
		});
	}


	///<summary>
	/// �ϑ����ꂽ��񂩂��肤�邷�ׂẴg���W�F�N�g���ɂ���execute_function�����s���邻�̌o�H��H�����G���e�B�e�B�� 
	///</summary>
	template <typename TRAJECTORY_TYPE, typename DUMMY_TYPE, typename USER_TYPE>
	void BasicObserver<TRAJECTORY_TYPE, DUMMY_TYPE, USER_TYPE>::for_each_expected_trajectory_frequency(const std::function<void(const Collection::Sequence<Graph::MapNodeIndicator>, double)>& execute_function)
	{
		if (structure == nullptr) create_observed_trajectory_structure();
		std::unordered_map<Graph::MapNodeIndicator, size_t> first_node_entity_count_info = structure->get_entity_count_info_of_phase(0);
		structure->for_each_possible_trajectory([&](const Collection::Sequence<Graph::MapNodeIndicator>& trajectory) {
			double probability = structure->calc_probability_of_trajectory(trajectory);
			size_t entity_count = first_node_entity_count_info.at(trajectory.at(0));
			execute_function(trajectory, probability * entity_count);
		});
	}



	///<summary>
	/// �e�G���e�B�e�B�̌����񐔂��擾����
	/// �C���f�b�N�X���G���e�B�e�BID�ɑΉ�
	///</summary>
	template <typename TRAJECTORY_TYPE, typename DUMMY_TYPE, typename USER_TYPE>
	std::vector<int> BasicObserver<TRAJECTORY_TYPE, DUMMY_TYPE, USER_TYPE>::get_cross_count_of_each_entity()
	{
		std::vector<int> cross_counts(entities->get_dummy_count() + 1);
		for (int i = 0; i <= entities->get_dummy_count(); i++) {
			std::vector<Evaluate::CrossInfo> info = get_cross_info_of_entity(i);
			int count = 0;
			for (std::vector<Evaluate::CrossInfo>::const_iterator iter = info.begin(); iter != info.end(); iter++) {
				if (iter->crossing_entities.size() > 0) count++;
			}
			cross_counts.at(i) = count;
		}
		return cross_counts;
	}

	///<summary>
	/// AR-Count���v�Z����
	///</summary>
	template <typename TRAJECTORY_TYPE, typename DUMMY_TYPE, typename USER_TYPE>
	double BasicObserver<TRAJECTORY_TYPE, DUMMY_TYPE, USER_TYPE>::calc_ar_count(double required_anonymous_area) const
	{
		int achieve_count = 0;
		std::shared_ptr<Time::TimeSlotManager const> timeslot = entities->read_timeslot();
		timeslot->for_each_time([&](time_t time, long interval, int phase) {
			double convex_hull_size = entities->calc_convex_hull_size_of_fixed_entities_of_phase(phase);
			if (convex_hull_size >= required_anonymous_area) achieve_count++;
		});
		return (double)achieve_count / timeslot->phase_count();
	}


	///<summary>
	/// ARSize���v�Z����
	///</summary>
	template <typename TRAJECTORY_TYPE, typename DUMMY_TYPE, typename USER_TYPE>
	double BasicObserver<TRAJECTORY_TYPE, DUMMY_TYPE, USER_TYPE>::calc_ar_size(double required_anonymous_area) const
	{
		double size_sum = 0.0;
		std::shared_ptr<Time::TimeSlotManager const> timeslot = entities->read_timeslot();
		timeslot->for_each_time([&](time_t time, long interval, int phase) {
			double convex_hull_size = entities->calc_convex_hull_size_of_fixed_entities_of_phase(phase);
			size_sum += convex_hull_size;
			std::cout << std::to_string(convex_hull_size) << " m^2" << std::endl;
		});
		return (double)size_sum / timeslot->phase_count() / required_anonymous_area;
	}


	///<summary>
	/// POI�̈Ӗ������l���Ȃ��ꍇ��MTC�̌v�Z (�Ō�܂ŒB���ł��Ȃ��ꍇ�̈������ǂ����邩�͗v����)
	/// �Ƃ肠�����A�Ō�܂ŒB���ł��Ȃ������Ƃ���͍l�����Ȃ��d�l�Ŏ���
	/// ���̃g���W�F�N�g������1����B���ł��Ȃ������ꍇ��-1��Ԃ�
	///</summary>
	template <typename TRAJECTORY_TYPE, typename DUMMY_TYPE, typename USER_TYPE>
	double BasicObserver<TRAJECTORY_TYPE, DUMMY_TYPE, USER_TYPE>::calc_mtc_without_semantics(double threshold)
	{
		if (cross_infos == nullptr) calc_cross_info();
		std::shared_ptr<Time::TimeSlotManager const> timeslot = entities->read_timeslot();

		size_t confusion_count = 0;
		double confusion_time_sum = 0.0;
		for (int start_phase = 0; start_phase < timeslot->phase_count() - 1; start_phase++) {
			double time_to_confusion = calc_time_to_confusion(cross_infos, start_phase, threshold);
			if (time_to_confusion != -1.0) {
				confusion_count++;
				confusion_time_sum += time_to_confusion;
				cunfusion_achive_count++;
			} 
			confusion_total_count++;
		}

		if (confusion_count == 0) return -1.0;

		double mtc = confusion_time_sum / confusion_count;
		return mtc;
	}


	///<summary>
	/// ���[�U���������ꂽ������start_phase�Ƃ����Ƃ���time_to_confusion���v�Z����
	///</summary>
	template <typename TRAJECTORY_TYPE, typename DUMMY_TYPE, typename USER_TYPE>
	double BasicObserver<TRAJECTORY_TYPE, DUMMY_TYPE, USER_TYPE>::calc_time_to_confusion(std::shared_ptr<std::vector<std::vector<Evaluate::CrossInfo>>> cross_infos, int start_phase, double threshold)
	{
		std::shared_ptr<Time::TimeSlotManager const> timeslot = entities->read_timeslot();
		
		//�e�G���e�B�e�B�̃��[�U�m��
		std::vector<double> probability_vector(entities->get_dummy_count() + 1, 0.0);
		probability_vector.at(0) = 1.0;

		//start_phase����̊e�t�F�[�Y�ɂ��Ċe�G���e�B�e�B�̃��[�U�m�����X�V���Ă���
		for (int phase = start_phase; phase < timeslot->phase_count() - 1; phase++) {

			//�����probability_vector����U�R�s�[
			std::vector<double> probability_vector_copy;
			std::copy(probability_vector.begin(), probability_vector.end(), std::back_inserter(probability_vector_copy));

			std::vector<Evaluate::CrossInfo> cross_info_of_phase = cross_infos->at(phase);
			for (std::vector<Evaluate::CrossInfo>::const_iterator crosses = cross_info_of_phase.begin(); crosses != cross_info_of_phase.end(); crosses++) {
				
				//���ڂ���G���e�B�e�B
				Entity::entity_id target_id = crosses->id;
				Graph::MapNodeIndicator next_visit_node = entities->read_entity_by_id(target_id)->read_node_pos_info_of_phase(phase + 1).first;
				
				//�z������m���l
				double probability_change = probability_vector_copy.at(target_id) / (crosses->crossing_entities.size() + 1);

				//��������̃m�[�h�����g�ƈႤ�m�[�h�Ɉړ����邩�`�F�b�N���C����ɉ����Ď��g�̊m����z��
				for (std::vector<Entity::entity_id>::const_iterator cross_target = crosses->crossing_entities.begin(); cross_target != crosses->crossing_entities.end(); cross_target++) {
					Graph::MapNodeIndicator cross_target_next_visit_node = entities->read_entity_by_id(*cross_target)->read_node_pos_info_of_phase(phase + 1).first;
					if (cross_target_next_visit_node != next_visit_node) {
						probability_vector.at(target_id) -= probability_change;
						probability_vector.at(*cross_target) += probability_change;
					}
				}
			}

			//�G���g���s�[���v�Z���Cthreshold�𒴂����ꍇ�͌o�ߎ�����time_to_confusion�Ƃ��ĕԂ�
			double entropy = Math::Probability::calc_entropy(probability_vector);
			if (entropy >= threshold) {
				double time_to_confusion = timeslot->time_of_phase(phase + 1) - timeslot->time_of_phase(start_phase);
				return time_to_confusion;
			}
		}

		return -1.0;
	}

	template <typename TRAJECTORY_TYPE, typename DUMMY_TYPE, typename USER_TYPE>
	double BasicObserver<TRAJECTORY_TYPE, DUMMY_TYPE, USER_TYPE>::get_confusion_achive_ratio_without_semantics() const
	{
		if (confusion_total_count == 0) return 0.0;
		return (double)cunfusion_achive_count / (double)confusion_total_count;
	}
}

