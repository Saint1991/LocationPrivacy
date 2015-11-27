#include "stdafx.h"
#include "MizunoMethod.h"

namespace Method
{


	///<summary>
	/// コンストラクタ
	///</summary>
	MizunoMethod::MizunoMethod(
		std::shared_ptr<Map::BasicDbMap const> map, 
		std::shared_ptr<User::BasicUser<Geography::LatLng>> user, 
		std::shared_ptr<User::PreferenceTree> observed_preference_tree,
		std::shared_ptr<Requirement::PreferenceRequirement const> requirement, 
		std::shared_ptr<Time::TimeSlotManager const> time_manager)
		: Framework::IProposedMethod<Map::BasicDbMap, User::BasicUser<Geography::LatLng>, Entity::Dummy<Geography::LatLng>, Requirement::PreferenceRequirement, Geography::LatLng, Graph::SemanticTrajectory<Geography::LatLng>>(map, user, requirement, time_manager),
		observed_preference_tree(observed_preference_tree), observed_preference_tree_copy(std::make_shared<User::PreferenceTree>(*observed_preference_tree))
	{
	}

	MizunoMethod::~MizunoMethod()
	{
	}


	///<summary>
	/// 初期化
	/// 特にやることはない
	///</summary>
	void MizunoMethod::initialize()
	{

	}


	///<summary>
	/// 提案手法の内容
	///</summary>
	void MizunoMethod::decide_dummy_positions()
	{
		std::shared_ptr<User::BasicUser<>> user = entities->get_user();
		int num_of_group_a_dummy = requirement->required_preference_conservation * requirement->dummy_num;
		int num_of_group_b_dummy = requirement->dummy_num - num_of_group_a_dummy;

		decide_dummy_positions_of_group_a(num_of_group_a_dummy);
		decide_dummy_positions_of_group_b(num_of_group_b_dummy);
	}

	#pragma region GroupA
	
	///<summary>
	/// グループAのダミーの生成
	///</summary>
	void MizunoMethod::decide_dummy_positions_of_group_a(int num_of_group_a_dummy)
	{
		for (int current_dummy_id = 1; current_dummy_id <= num_of_group_a_dummy; current_dummy_id++) {
			
			//ユーザの嗜好の木に含まれる各プレフィックスについてカテゴリシーケンスとスコアの組を計算する
			std::vector<sequence_score_set> sequence_scores = calc_sequence_score_set(current_dummy_id);

			//実際の経路の生成
			std::vector<trajectory_score_set> trajectory_scores = calc_trajectory_score_set(current_dummy_id, sequence_scores);
			
			//経路が生成できなかった場合は例外を投げて終了 (暫定的処置)
			if (trajectory_scores.size() == 0) throw Framework::TrajectoryNotFoundException("Cannot found valid trajectory following preference tree");
			std::sort(trajectory_scores.begin(), trajectory_scores.end(), [](const trajectory_score_set& left, const trajectory_score_set& right) {
				return left.second > right.second;
			});

			//スコアが最大のものの中からランダムで1つ選出し，経路を確定させる
			std::vector<std::pair<std::vector<Graph::MapNodeIndicator>, Entity::cross_target>> candidates;
			double max_score = trajectory_scores.begin()->second;
			for (std::vector<trajectory_score_set>::const_iterator iter = trajectory_scores.begin(); iter != trajectory_scores.end(); iter++) {
				if (iter->second == max_score) candidates.push_back(iter->first);
			}

			//経路の選択
			Math::Probability generator;
			int pick_index = generator.uniform_distribution(0, candidates.size() - 1);
			std::pair<std::vector<Graph::MapNodeIndicator>, Entity::cross_target> trajectory = candidates.at(pick_index);

			//現在決定中のエンティティの経路を確定させる
			std::shared_ptr<Entity::Dummy<Geography::LatLng>> current_dummy = entities->get_dummy_by_id(current_dummy_id);
			std::vector<Graph::MapNodeIndicator> route = trajectory.first;
			int cross_phase = trajectory.second.first;
			for (int phase = 0; phase < route.size(); phase++) {
				Graph::MapNodeIndicator node_id = route.at(phase);
				std::shared_ptr<Map::BasicPoi const> visit_poi = map->get_static_poi(node_id.id());
				Geography::LatLng position(visit_poi->lat(), visit_poi->lng());
				phase == cross_phase ? current_dummy->set_crossing_position_of_phase(phase, node_id, position) : current_dummy->set_position_of_phase(phase, node_id, position);
				current_dummy->set_category_of_phase(phase, visit_poi->category_id());
				std::cout << visit_poi->category_name() << "\t";
			}
			std::cout << std::endl;

			//observed_preference_tree_copyに決定トラジェクトリの結果を反映する
			observed_preference_tree_copy->add_sequence_counter(current_dummy->read_trajectory()->get_category_sequence());
		}
	}

	///<summary>
	/// シーケンスと共有地点の候補の組<S, SP>に対するスコアの計算
	///</summary>
	std::vector<std::pair<std::pair<Collection::Sequence<User::category_id>, Entity::cross_target>, double>> MizunoMethod::calc_sequence_score_set(Entity::entity_id current_dummy_id)
	{
		std::shared_ptr<User::BasicUser<Geography::LatLng> const> user = entities->get_user();
		std::shared_ptr<User::PreferenceTree const> preference_tree = user->preference;
		std::shared_ptr<Graph::SemanticTrajectory<> const> user_trajectory  = user->read_trajectory();
		std::vector<sequence_score_set> ret;

		// 今回のトラジェクトリが嗜好の木の最大長より長い場合，プレフィックスの最大長の長さのものを対象とする．
		size_t max_depth = preference_tree->max_depth();
		size_t usable_prefix_length = min(max_depth, user_trajectory->phase_count());

		for (int prefix_length = usable_prefix_length - 0; prefix_length <= usable_prefix_length; prefix_length++) {
			
			//真の嗜好の木に含まれるすべてのPrefixについて
			preference_tree->for_each_prefix(prefix_length, [&](const Collection::Sequence<User::category_id>& prefix, double sup_u) {
				
				//Score_{pref}の計算
				double sup_o = observed_preference_tree->get_support_of(prefix);
				double sup_o_cp = observed_preference_tree_copy->get_support_of(prefix);
				double score_pref = preference_based_score(sup_u, sup_o_cp);

				//Score_{cross}の計算
				for (int target_entity_id = 0; target_entity_id < current_dummy_id; target_entity_id++) {
					
					std::shared_ptr<Entity::Dummy<>> target_entity = target_entity_id == 0 ? entities->get_user() : entities->get_dummy_by_id(target_entity_id);
					std::shared_ptr<Graph::SemanticTrajectory<> const> trajectory = target_entity->read_trajectory();
					Collection::Sequence<Entity::category_id> target_sequence = trajectory->get_category_sequence();
					
					for (int target_phase = 0; target_phase < time_manager->phase_count(); target_phase++) {
						
						
						double score_cross = 0.0;
						int delta = target_sequence.subsequence(0, target_phase) == prefix.subsequence(0, target_phase) ? 1 : 0;
						Entity::cross_target cross = Entity::NOTHING; 
						if (delta != 0) {

							//交差の時刻と相手
							cross = std::make_pair(target_phase, target_entity_id);
							
							//n_share(t)の計算
							int n_share = entities->get_total_cross_count_of_phase(target_phase);

							//交差が設定されているエンティティのサポートの合計値の計算
							double sup_sum = 0.0;
							std::vector<Entity::entity_id> cross_entities = entities->get_entities_cross_with(target_entity_id);
							for (std::vector<Entity::entity_id>::const_iterator target_entity_id = cross_entities.begin(); target_entity_id != cross_entities.end(); target_entity_id++) {
								Collection::Sequence<Entity::category_id> target_sequence = entities->read_dummy_by_id(*target_entity_id)->read_trajectory()->get_category_sequence(0, prefix_length - 1);
								sup_sum += observed_preference_tree->get_support_of(target_sequence);
							}
							// Score_Crossの計算
							score_cross = cross_based_score(delta, n_share, sup_o, sup_sum);
						}
						
						//負のものの対応は保留で一旦正のもののみ採用
						double score = total_sequence_score(score_pref, score_cross);
						if (score > 0.0) ret.push_back(std::make_pair(std::make_pair(prefix, cross), score));
					}
				}
			});
		}

		return ret;
	}


	///<summary>
	/// シーケンスに対するスコア
	///</summary>
	double MizunoMethod::total_sequence_score(double score_pref, double score_cross)
	{
		return B * score_pref + (1 - B) * score_cross;
	}

	///<summary>
	/// Score_prefの計算式
	///</summary>
	double MizunoMethod::preference_based_score(double sup_u, double sup_o)
	{
		return sup_u - sup_o;
	}


	///<summary>
	/// Score_crossの計算式
	///</summary>
	double MizunoMethod::cross_based_score(int delta, int n_share, double sup_o, double sup_sum)
	{
		return delta == 0 ? 0.0 : (1.0 / (1.0 + n_share)) * sup_o / (1.0 + sup_sum);
	}


	
	///<summary>
	/// 計算したカテゴリシーケンスに基づいて実際の移動経路を生成する
	///</summary>
	std::vector<std::pair<std::pair<std::vector<Graph::MapNodeIndicator>, Entity::cross_target>, double>> MizunoMethod::calc_trajectory_score_set(Entity::entity_id current_dummy_id, const std::vector<sequence_score_set>& sequence_scores)
	{
		std::vector<trajectory_score_set> ret;

		//実際の経路の生成
		for (std::vector<sequence_score_set>::const_iterator iter = sequence_scores.begin(); iter != sequence_scores.end(); iter++) {
			double score = iter->second;
			Collection::Sequence<User::category_id> category_sequence = iter->first.first;
			Entity::cross_target cross = iter->first.second;

			//共有地点が設定できなかった場合
			//グリッド領域を生成し，エンティティが最小の地点から一点決定する
			if (cross == Entity::NOTHING) {

			}


			
		}

		return ret;
	}



	#pragma endregion GroupAのダミー生成
	
	///<summary>
	/// グループBのダミーの生成
	///</summary>
	void MizunoMethod::decide_dummy_positions_of_group_b(int num_of_group_b_dummy)
	{
		for (int current_dummy_id = requirement->dummy_num - num_of_group_b_dummy + 1; current_dummy_id <= requirement->dummy_num;  current_dummy_id++) {

		}
	}
}

