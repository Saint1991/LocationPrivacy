#include "stdafx.h"
#include "MizunoMethodSameSequence.h"

namespace Method
{

	///<summary>
	/// コンストラクタ
	///</summary>
	MizunoMethodSameSequence::MizunoMethodSameSequence(
		std::shared_ptr<Map::BasicDbMap const> map,
		std::shared_ptr<User::BasicUser<Geography::LatLng>> user,
		std::shared_ptr<User::PreferenceTree> observed_preference_tree,
		std::shared_ptr<Requirement::PreferenceRequirement const> requirement,
		std::shared_ptr<Time::TimeSlotManager const> time_manager) : MizunoMethodModWithoutReachability(map, user, observed_preference_tree, requirement, time_manager)
	{
	}


	MizunoMethodSameSequence::~MizunoMethodSameSequence()
	{
	}


	///<summary>
	/// 常にユーザと同じシーケンスを選択し，交差の相手及び時刻はランダム
	///</summary>
	std::vector<std::pair<std::pair<Collection::Sequence<User::category_id>, Entity::cross_target>, double>> MizunoMethodSameSequence::calc_sequence_score_set(Entity::entity_id current_dummy_id)
	{
		std::shared_ptr<User::BasicUser<Geography::LatLng> const> user = entities->get_user();
		std::shared_ptr<Graph::SemanticTrajectory<Geography::LatLng> const> user_trajectory = user->read_trajectory();
		Collection::Sequence<User::category_id> category_sequence_of_user = user_trajectory->get_category_sequence();

		std::vector<sequence_score_set> ret;

		Math::Probability prob;
		Entity::entity_id cross_target = prob.uniform_distribution(0, current_dummy_id - 1);
		int cross_phase = prob.uniform_distribution(0, user_trajectory->phase_count() - 2);

		Entity::cross_target cross = std::make_pair(cross_phase, cross_target);
		ret.push_back(std::make_pair(std::make_pair(category_sequence_of_user, cross), 1.0));
		return ret;
	}
	
}


