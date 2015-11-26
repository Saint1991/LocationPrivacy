#include "stdafx.h"
#include "MizunoMethod.h"

namespace Method
{

	MizunoMethod::MizunoMethod(
		std::shared_ptr<Map::BasicDbMap const> map, 
		std::shared_ptr<User::BasicUser<Geography::LatLng>> user, 
		std::shared_ptr<User::PreferenceTree> observed_preference_tree_copy,
		std::shared_ptr<Requirement::PreferenceRequirement const> requirement, 
		std::shared_ptr<Time::TimeSlotManager const> time_manager)
		: Framework::IProposedMethod<Map::BasicDbMap, User::BasicUser<Geography::LatLng>, Entity::Dummy<Geography::LatLng>, Requirement::PreferenceRequirement, Geography::LatLng, Graph::SemanticTrajectory<Geography::LatLng>>(map, user, requirement, time_manager)
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
			
			std::shared_ptr<Entity::Dummy<Geography::LatLng>> current_dummy = entities->get_dummy_by_id(current_dummy_id);
			
			//ユーザの嗜好の木に含まれる各プレフィックスについてカテゴリシーケンスとスコアの組を計算する
			std::vector<sequence_score_set> sequence_scores = calc_sequence_score_set();
			
			//スコアで降順にソート
			std::sort(sequence_scores.begin(), sequence_scores.end(), [](const sequence_score_set& left, const sequence_score_set& right) {
				return left.second < right.second;
			});


		}
	}

	///<summary>
	/// 
	///</summary>
	std::vector<std::pair<Collection::Sequence<User::category_id>, double>> MizunoMethod::calc_sequence_score_set()
	{
		std::shared_ptr<User::BasicUser<Geography::LatLng> const> user = entities->get_user();
		std::shared_ptr<User::PreferenceTree const> preference_tree = user->preference;
		std::shared_ptr<Graph::SemanticTrajectory<> const> user_trajectory  = user->read_trajectory();
		std::vector<sequence_score_set> ret;

		// 今回のトラジェクトリが嗜好の木の最大長より長い場合，プレフィックスの最大長-1以上の長さのものを対象とする．
		size_t max_depth = preference_tree->max_depth();
		size_t usable_prefix_length = min(max_depth, user_trajectory->phase_count());

		for (int prefix_length = usable_prefix_length - 1; prefix_length <= usable_prefix_length; prefix_length++) {
			preference_tree->for_each_prefix(prefix_length, [&](const Collection::Sequence<User::category_id>& prefix, double sup_u) {
				double sup_o = observed_preference_tree_copy->get_support_of(prefix);
				double score_pref = preference_based_score(sup_u, sup_o);
			});
		}

		return ret;
	}


	///<summary>
	/// Score_prefの計算式
	///</summary>
	double MizunoMethod::preference_based_score(double sup_u, double sup_o)
	{
		return B * (sup_u - sup_o);
	}


	///<summary>
	/// Score_cross
	///</summary>
	double MizunoMethod::cross_based_score()
	{
		return 1.0;
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

