#include "stdafx.h"
#include "BasicObserver.h"

namespace Observer
{


	///<summary>
	/// コンストラクタ
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
	/// デストラクタ
	///</summary>
	template <typename TRAJECTORY_TYPE, typename DUMMY_TYPE, typename USER_TYPE>
	BasicObserver<TRAJECTORY_TYPE, DUMMY_TYPE, USER_TYPE>::~BasicObserver()
	{
	}


	///<summary>
	/// 全時刻のCrossInfoを計算する
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
	/// 指定したエンティティのCrossInfoを前のPhaseから順に取得する
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
	/// トラジェクトリとその交差関係を基に観測されるすべてのトラジェクトリモデルを作成する
	///</summary>
	template <typename TRAJECTORY_TYPE, typename DUMMY_TYPE, typename USER_TYPE>
	std::shared_ptr<ObservedTrajectoryStructure const> BasicObserver<TRAJECTORY_TYPE, DUMMY_TYPE, USER_TYPE>::create_observed_trajectory_structure()
	{
		if (structure != nullptr) return structure;

		std::shared_ptr<ObservedTrajectoryStructure> trajectory_structure = std::make_shared<ObservedTrajectoryStructure>();
		ObservedTrajectoryStructure::base_iterator iter = trajectory_structure->root<ObservedTrajectoryStructure::base_iterator>();

		//まずは交差を考えずに個々の可観測なトラジェクトリ情報を作成する
		std::shared_ptr<Time::TimeSlotManager const> timeslot = entities->read_timeslot();
		timeslot->for_each_time([&](time_t time, long interval, int phase) {

			int depth = phase + 1;

			for (Entity::entity_id id = 0; id <= entities->get_dummy_count(); id++) {

				//着目しているエンティティに応じて親ノードを更新
				std::shared_ptr<Entity::MobileEntity<Geography::LatLng, Graph::SemanticTrajectory<Geography::LatLng>> const> entity = entities->read_entity_by_id(id);
				if (phase >= 1) {
					Graph::MapNodeIndicator previous_node = entity->read_node_pos_info_of_phase(phase - 1).first;
					iter = trajectory_structure->find_node(previous_node, phase - 1);
				}
				else if (phase == 0) iter->count_up();

				//次の訪問地点をエンティティ数1で子ノードとして接続する
				//既存のノードの場合はエンティティ数をカウントアップする
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

		//多分ここ以下は厳密には間違っている，状態遷移の組み合わせを解く必要がある
		//交差情報を元に取りうるトラジェクトリの接続関係を追加する
		for (Entity::entity_id id = 0; id <= entities->get_dummy_count(); id++) {

			//着目するエンティティとその交差情報
			std::shared_ptr<Entity::MobileEntity<Geography::LatLng, Graph::SemanticTrajectory<Geography::LatLng>> const> entity = entities->read_entity_by_id(id);
			std::vector<Evaluate::CrossInfo> cross_infos_of_entity = get_cross_info_of_entity(id);
			
			//交差相手の辿ったノードへのパスがない場合はそれを作成する
			for (std::vector<Evaluate::CrossInfo>::const_iterator cross_info = cross_infos_of_entity.begin(); cross_info != cross_infos_of_entity.end(); cross_info++) {

				//交差を作成するtrajectory_nodeを取得する
				int cross_phase = cross_info->phase;
				Graph::MapNodeIndicator cross_node = entity->read_node_pos_info_of_phase(cross_phase).first;
				ObservedTrajectoryStructure::base_iterator iter = trajectory_structure->find_node(cross_node, cross_info->phase);

				Graph::node_id next_node_id = trajectory_structure->find_node_id(entity->read_node_pos_info_of_phase(cross_phase + 1).first, cross_phase + 1);
				if (next_node_id == -1) throw std::invalid_argument("Node not found.");
				
				//交差相手のノードがどのPOIにいくつ移動するかを計算する
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

				//上の情報を元にエッジを張る
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
	/// IDがidのエンティティが移動し得たトラジェクトリとその確率の組についてexecute_functionを実行する
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
	/// 観測された情報から取りうるすべてのトラジェクトリについてexecute_functionを実行するその経路を辿ったエンティティ数 
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
	/// 各エンティティの交差回数を取得する
	/// インデックスがエンティティIDに対応
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
	/// AR-Countを計算する
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
	/// ARSizeを計算する
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
	/// POIの意味情報を考えない場合のMTCの計算 (最後まで達成できない場合の扱いをどうするかは要検討)
	/// とりあえず、最後まで達成できなかったところは考慮しない仕様で実装
	/// そのトラジェクトリ中で1回も達成できなかった場合は-1を返す
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
	/// ユーザが発見された時刻をstart_phaseとしたときのtime_to_confusionを計算する
	///</summary>
	template <typename TRAJECTORY_TYPE, typename DUMMY_TYPE, typename USER_TYPE>
	double BasicObserver<TRAJECTORY_TYPE, DUMMY_TYPE, USER_TYPE>::calc_time_to_confusion(std::shared_ptr<std::vector<std::vector<Evaluate::CrossInfo>>> cross_infos, int start_phase, double threshold)
	{
		std::shared_ptr<Time::TimeSlotManager const> timeslot = entities->read_timeslot();
		
		//各エンティティのユーザ確立
		std::vector<double> probability_vector(entities->get_dummy_count() + 1, 0.0);
		probability_vector.at(0) = 1.0;

		//start_phaseからの各フェーズについて各エンティティのユーザ確立を更新していく
		for (int phase = start_phase; phase < timeslot->phase_count() - 1; phase++) {

			//現状のprobability_vectorを一旦コピー
			std::vector<double> probability_vector_copy;
			std::copy(probability_vector.begin(), probability_vector.end(), std::back_inserter(probability_vector_copy));

			std::vector<Evaluate::CrossInfo> cross_info_of_phase = cross_infos->at(phase);
			for (std::vector<Evaluate::CrossInfo>::const_iterator crosses = cross_info_of_phase.begin(); crosses != cross_info_of_phase.end(); crosses++) {
				
				//着目するエンティティ
				Entity::entity_id target_id = crosses->id;
				Graph::MapNodeIndicator next_visit_node = entities->read_entity_by_id(target_id)->read_node_pos_info_of_phase(phase + 1).first;
				
				//配分する確率値
				double probability_change = probability_vector_copy.at(target_id) / (crosses->crossing_entities.size() + 1);

				//交差相手のノードが自身と違うノードに移動するかチェックし，それに応じて自身の確率を配分
				for (std::vector<Entity::entity_id>::const_iterator cross_target = crosses->crossing_entities.begin(); cross_target != crosses->crossing_entities.end(); cross_target++) {
					Graph::MapNodeIndicator cross_target_next_visit_node = entities->read_entity_by_id(*cross_target)->read_node_pos_info_of_phase(phase + 1).first;
					if (cross_target_next_visit_node != next_visit_node) {
						probability_vector.at(target_id) -= probability_change;
						probability_vector.at(*cross_target) += probability_change;
					}
				}
			}

			//エントロピーを計算し，thresholdを超えた場合は経過時刻をtime_to_confusionとして返す
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

