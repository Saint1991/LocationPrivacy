#ifdef SIMULATORCOMPONENTS_EXPORTS
#define MIZUNO_METHOD_API __declspec(dllexport)
#else
#define MIZUNO_METHOD_API __declspec(dllimport)
#endif

#pragma once
#include "BasicDbMap.h"
#include "LatLng.h"
#include "BasicUser.h"
#include "Dummy.h"
#include "PreferenceRequirement.h"
#include "IProposedMethod.h"
#include "SemanticTrajectory.h"
#include "Grid.h"

namespace Method
{
	class MIZUNO_METHOD_API MizunoMethod
		: public Framework::IProposedMethod<Map::BasicDbMap, User::BasicUser<Geography::LatLng>, Entity::Dummy<Geography::LatLng>, Requirement::PreferenceRequirement, Geography::LatLng, Graph::SemanticTrajectory<Geography::LatLng>>
	{
	private:
		static constexpr double B = 0.5;

	protected:
		typedef std::pair<std::pair<Collection::Sequence<User::category_id>, Entity::cross_target>, double> sequence_score_set;
		typedef std::pair<std::pair<std::vector<Graph::MapNodeIndicator>, Entity::cross_target>, double> trajectory_score_set;
		std::shared_ptr<User::PreferenceTree> observed_preference_tree;
		std::shared_ptr<User::PreferenceTree> observed_preference_tree_copy;

		void initialize();
		void decide_dummy_positions();

		#pragma region GroupA
		void decide_dummy_positions_of_group_a(int num_of_group_a_dummy);
		
		//カテゴリシーケンスの決定関連のメソッド
		std::vector<sequence_score_set> calc_sequence_score_set(Entity::entity_id current_dummy_id);
		double total_sequence_score(double score_pref, double score_cross);
		double preference_based_score(double sup_u, double sup_o);
		double cross_based_score(int delta, int n_share, double sup_o, double sup_sum);

		//経路決定関連のメソッド
		std::vector<trajectory_score_set> calc_trajectory_score_set(Entity::entity_id current_dummy_id, const std::vector<sequence_score_set>& sequence_scores);

		#pragma region GroupAのダミー生成用メソッド
		
		#pragma region GroupB
		void decide_dummy_positions_of_group_b(int num_of_group_b_dummy);
		
		#pragma endregion GroupBのダミー生成用メソッド
	
	public:
		MizunoMethod(
			std::shared_ptr<Map::BasicDbMap const> map, 
			std::shared_ptr<User::BasicUser<Geography::LatLng>> user, 
			std::shared_ptr<User::PreferenceTree> observed_preference_tree,
			std::shared_ptr<Requirement::PreferenceRequirement const> requirement, 
			std::shared_ptr<Time::TimeSlotManager const> time_manager);
		~MizunoMethod();
	};
}


