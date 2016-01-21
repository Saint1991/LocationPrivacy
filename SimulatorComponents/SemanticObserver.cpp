#include "stdafx.h"
#include "SemanticObserver.h"

namespace  Observer
{

	///<summary>
	/// コンストラクタ
	/// (現状の内容ではpreferenceには真の嗜好を渡すべき)
	///</summary>
	template <typename DUMMY_TYPE, typename USER_TYPE>
	SemanticObserver<DUMMY_TYPE, USER_TYPE>::SemanticObserver(
		std::shared_ptr<Map::BasicDbMap const> map,
		std::shared_ptr<Entity::EntityManager<Geography::LatLng, Graph::SemanticTrajectory<Geography::LatLng>, DUMMY_TYPE, USER_TYPE> const> entities,
		std::shared_ptr<User::PreferenceTree> preference,
		double move_speed,
		const std::function<bool(std::shared_ptr<Map::BasicDbMap const>, const Graph::MapNodeIndicator&, const Graph::MapNodeIndicator&, const Graph::MapNodeIndicator&, const Graph::MapNodeIndicator&, double, long)>& cross_rule
		) : BasicObserver<Graph::SemanticTrajectory<Geography::LatLng>, DUMMY_TYPE, USER_TYPE>(map, entities, move_speed, cross_rule), preference(preference), confusion_total_count_with_semantics(0), confusion_achieve_count_with_semantics(0)
	{
	}


	///<summary>
	/// デストラクタ
	///</summary>
	template <typename DUMMY_TYPE, typename USER_TYPE>
	SemanticObserver<DUMMY_TYPE, USER_TYPE>::~SemanticObserver()
	{
	}


	///<summary>
	/// 観測されたトラジェクトリから起こり得たカテゴリシーケンスを計算し、
	/// その各シーケンスと発生度数の期待値についてexecute_functionを実行する
	/// ただし同一シーケンスでも一度で実行されるとは限らない点に注意
	///</summary>
	template <typename DUMMY_TYPE, typename USER_TYPE>
	void SemanticObserver<DUMMY_TYPE, USER_TYPE>::for_each_category_sequence_of_possible_trajectory(const std::function<void(const Collection::Sequence<std::string>&, double)>& execute_function)
	{
		for_each_expected_trajectory_frequency([&](const Collection::Sequence<Graph::MapNodeIndicator>& trajectory, double expected_frequency) {
			Collection::Sequence<std::string> category_sequence;
			for (Collection::Sequence<Graph::MapNodeIndicator>::const_iterator iter = trajectory.begin(); iter != trajectory.end(); iter++) {
				if (iter->type() != Graph::NodeType::POI) throw std::invalid_argument("NodeType must be POI");
				std::shared_ptr<Map::BasicPoi const> visited_poi = map->get_static_poi(iter->id());
				category_sequence.push_back(visited_poi->category_id());
			}
			execute_function(category_sequence, expected_frequency);
		});
	}

	///<summary>
	/// SemanticObservedStructureを作成する
	/// この構造では，エッジが移動したエンティティ数の期待値的比率を表し，
	/// ノードその移動経路集合に対し，サポート値を累積していったものを保持する．
	///</summary>
	template <typename DUMMY_TYPE, typename USER_TYPE>
	std::shared_ptr<SemanticObservedTrajectoryStructure const> SemanticObserver<DUMMY_TYPE, USER_TYPE>::create_semantic_observed_trajectory_structure() {
		
		if (semantic_observed_trajectory_structure != nullptr) return semantic_observed_trajectory_structure;
		if (structure == nullptr) create_observed_trajectory_structure();
		
		std::shared_ptr<SemanticObservedTrajectoryStructure> semantic_structure = std::make_shared<SemanticObservedTrajectoryStructure>();
		structure->for_each_possible_trajectory([&](const Collection::Sequence<Graph::MapNodeIndicator>& trajectory) {
			
			//観測されたトラジェクトリのシーケンスとサポート
			Collection::Sequence<User::category_id> sequence = map->convert_to_category_sequence(trajectory);
			double support = preference->get_support_of(sequence);

			//作成するsemantic_structureをルートからたどる
			SemanticObservedTrajectoryStructure::base_iterator iter = semantic_structure->root<SemanticObservedTrajectoryStructure::base_iterator>();
			Graph::node_id parent_node_id = iter->get_id();
			
			for (int phase = 0; phase < trajectory.size(); phase++) {
				
				int depth = phase + 1;
				Graph::MapNodeIndicator target_id = trajectory.at(phase);

				//Rootノードを取得
				ObservedTrajectoryStructure::base_iterator original_current_iter = phase == 0 ?  structure->find_node(Graph::MapNodeIndicator(-1, Graph::NodeType::INVALID), -1) : structure->find_node(trajectory.at(phase - 1), phase - 1);
				Graph::node_id original_next_node_id = structure->find_node_id(target_id, phase);
				std::shared_ptr<Graph::FlowEdge const> copy_edge = iter->get_static_edge_to(original_next_node_id);
				if (copy_edge == nullptr) throw std::exception("The edge somehow not found");
				double copy_edge_value = copy_edge->get_flow();

				//作成中のSemanticStructure中で子ノードの存在を確認する
				SemanticObservedTrajectoryStructure::base_iterator child_iter = iter.find_child_if([&target_id](std::shared_ptr<SemanticObservedTrajectoryNode const> node) {
					return node->data != nullptr && *node->data == target_id;
				});

				//対象のノードがまだない場合は作成
				if (*child_iter == nullptr) {
					//子ノードの作成
					std::shared_ptr<SemanticObservedTrajectoryNode> new_node = std::make_shared<SemanticObservedTrajectoryNode>(semantic_structure->node_count(), depth, std::make_shared<Graph::MapNodeIndicator>(target_id));
					new_node->flow_in_from(parent_node_id, support);
					iter.add_child(new_node, copy_edge_value);
					iter = semantic_structure->find_node(target_id, depth);
				}
				//既存のノードの場合はサポートを加算するだけ
				else {
					child_iter->flow_in_from(parent_node_id, support);
					iter = child_iter;
				}
			}
		});

		semantic_observed_trajectory_structure = semantic_structure;
		return semantic_structure;
	}

	///<summary>
	/// Semanticsを考慮したMTCの計算 (未実装)
	///</summary>
	template <typename DUMMY_TYPE, typename USER_TYPE>
	double SemanticObserver<DUMMY_TYPE, USER_TYPE>::calc_mtc_with_semantics(double threshold)
	{
		if (semantic_observed_trajectory_structure == nullptr) create_semantic_observed_trajectory_structure();
		std::shared_ptr<Time::TimeSlotManager const> timeslot = entities->read_timeslot();

		size_t confusion_count = 0;
		double confusion_time_sum = 0.0;
		for (int start_phase = 0; start_phase < timeslot->phase_count() - 1; start_phase++) {
			double time_to_confusion = calc_time_to_confusion_with_semantics(cross_infos, semantic_observed_trajectory_structure, start_phase, threshold);
			if (time_to_confusion != -1.0) {
				confusion_count++;
				confusion_time_sum += time_to_confusion;
				confusion_achieve_count_with_semantics++;
			}
			confusion_total_count_with_semantics++;
		}

		if (confusion_count == 0) return -1.0;

		double mtc = confusion_time_sum / confusion_count;
		return mtc;
	}


	///<summary>
	/// MTC2の確率更新の中身
	///</summary>
	template <typename DUMMY_TYPE, typename USER_TYPE>
	double SemanticObserver<DUMMY_TYPE, USER_TYPE>::calc_time_to_confusion_with_semantics(std::shared_ptr<std::vector<std::vector<Evaluate::CrossInfo>>> cross_infos, std::shared_ptr<SemanticObservedTrajectoryStructure> semantic_structure, int start_phase, double threshold)
	{
		std::shared_ptr<Time::TimeSlotManager const> timeslot = entities->read_timeslot();

		//各エンティティのユーザ確率
		std::vector<double> probability_vector(entities->get_dummy_count() + 1, 0.0);
		probability_vector.at(0) = 1.0;

		for (int phase = start_phase; phase < timeslot->phase_count() - 1; phase++) {

			std::vector<double> probability_vector_copy;
			std::copy(probability_vector.begin(), probability_vector.end(), std::back_inserter(probability_vector_copy));

			double check_sum = std::accumulate(probability_vector.begin(), probability_vector.end(), 0.0);
			if (check_sum < 0.999999 || 1.0000001 < check_sum) throw std::invalid_argument("The sum of Probability is not 1.0");

			std::vector<Evaluate::CrossInfo> cross_info_of_phase = cross_infos->at(phase);
			for (std::vector<Evaluate::CrossInfo>::const_iterator crosses = cross_info_of_phase.begin(); crosses != cross_info_of_phase.end(); crosses++) {

				//着目するエンティティ
				Entity::entity_id target_id = crosses->id;
				Graph::MapNodeIndicator current_node_id = entities->read_entity_by_id(target_id)->read_node_pos_info_of_phase(phase).first;
				Graph::MapNodeIndicator next_visit_node = entities->read_entity_by_id(target_id)->read_node_pos_info_of_phase(phase + 1).first;

				SemanticObservedTrajectoryStructure::base_iterator current_state = semantic_structure->find_node(current_node_id, phase);
				double current_state_support = current_state->get_count();

				//各交差相手のエンティティに確率を配分
				for (std::vector<Entity::entity_id>::const_iterator cross_target = crosses->crossing_entities.begin(); cross_target != crosses->crossing_entities.end(); cross_target++) {
					Graph::MapNodeIndicator cross_target_next_visit_node = entities->read_entity_by_id(*cross_target)->read_node_pos_info_of_phase(phase + 1).first;
					
					//同一ノードに訪問する場合は配分しない
					if (cross_target_next_visit_node == next_visit_node) continue;
					
					//次に移動した(と思われた場所)
					SemanticObservedTrajectoryStructure::base_iterator next_fake_state = semantic_structure->find_node(cross_target_next_visit_node, phase + 1);
					double next_state_support = next_fake_state->get_count();
					
					//配分する確率値の計算
					double transition_probability = next_state_support / current_state_support;
					if (transition_probability > 1.0) {
						std::cout << "Transition - Before: " << transition_probability << std::endl;
					}

					std::shared_ptr<Graph::FlowEdge> flow_edge = current_state->get_edge_to(next_fake_state->get_id());
					if (flow_edge == nullptr) {
						transition_probability *= 0.0;
					}
					else {
						double flow = flow_edge->get_flow();
						transition_probability *= flow;
					}

					//確率の配分値
					double probability_change = probability_vector_copy.at(target_id) * transition_probability;
					std::cout << "Transition: " << transition_probability << std::endl;
					probability_vector.at(target_id) -= probability_change;
					if (probability_vector.at(target_id) < 0) {
						for (std::vector<double>::const_iterator prob = probability_vector.begin(); prob != probability_vector.end(); prob++) {
							std::cout << *prob << "\t";
						}
						std::cout << std::endl;
					}
					probability_vector.at(*cross_target) += probability_change;
				}
			}

			double entropy = Math::Probability::calc_entropy(probability_vector);
			if (entropy >= threshold) {
				double time_to_confusion = timeslot->time_of_phase(phase + 1) - timeslot->time_of_phase(start_phase);
				return time_to_confusion;
			}
		}

		return -1.0;
	}


	template <typename DUMMY_TYPE, typename USER_TYPE>
	double SemanticObserver<DUMMY_TYPE, USER_TYPE>::get_confusion_achieve_ratio_with_semantics() const
	{
		if (confusion_total_count_with_semantics == 0) return 0;
		return (double)confusion_achieve_count_with_semantics / (double)confusion_total_count_with_semantics;
	}

	
}
