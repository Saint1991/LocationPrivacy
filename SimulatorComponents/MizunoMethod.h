#ifdef SIMULATORCOMPONENTS_EXPORTS
#define MIZUNO_METHOD_API __declspec(dllexport)
#else
#define MIZUNO_METHOD_API __declspec(dllimport)
#endif

#pragma once
#include "LatLng.h"
#include "BasicUser.h"
#include "Dummy.h"
#include "PreferenceRequirement.h"
#include "IProposedMethod.h"
#include "SemanticTrajectory.h"
#include "Grid.h"
#include "BasicDbPreprocessedMap.h"

namespace Method
{
	class MIZUNO_METHOD_API MizunoMethod
		: public Framework::IProposedMethod<Map::BasicDbPreprocessedMap, User::BasicUser<Geography::LatLng>, Entity::Dummy<Geography::LatLng>, Requirement::PreferenceRequirement, Geography::LatLng, Graph::SemanticTrajectory<Geography::LatLng>>
	{
	private:
		//�X�R�A�����O�̃p�����[�^
		static constexpr double B = 0.8;
		//�i�s�����ɉ��킹�鐧���̊p�x��
		static constexpr double THETA = M_PI_2;
		//�o�H�����̎��s�񐔂̏��
		static constexpr int MAX_TRAJECTORY_CREATION_TRIAL = 1000;
		
		//�ŏ���~����(2��)
		static constexpr int MIN_PAUSE_TIME = 0;

		
		//�ݒ蓽���̈�
		std::unique_ptr<std::vector<double>> setting_anonymous_areas;

	protected:
		typedef std::pair<std::pair<Collection::Sequence<User::category_id>, Entity::cross_target>, double> sequence_score_set;
		typedef std::pair<std::pair<std::vector<Graph::MapNodeIndicator>, Entity::cross_target>, double> trajectory_score_set;
		std::shared_ptr<User::PreferenceTree> observed_preference_tree;
		std::shared_ptr<User::PreferenceTree> observed_preference_tree_copy;

		void initialize();
		void decide_dummy_positions();

		void set_setting_anonymous_area(const std::function<double(int, size_t, double)>& generator);

		#pragma region GroupA

		void decide_dummy_positions_of_group_a(int num_of_group_a_dummy);
		
		//�J�e�S���V�[�P���X�̌���֘A�̃��\�b�h
		std::vector<sequence_score_set> calc_sequence_score_set(Entity::entity_id current_dummy_id);
		double total_sequence_score(double score_pref, double score_cross);
		double preference_based_score(double sup_u, double sup_o);
		double cross_based_score(int n_share_t, int n_share_e, double distro, int delta);

		//�o�H����֘A�̃��\�b�h
		std::vector<trajectory_score_set> calc_trajectory_score_set(Entity::entity_id current_dummy_id, const std::vector<sequence_score_set>& sequence_scores);
		std::pair<int, Graph::MapNodeIndicator> determine_point_basis(const Collection::Sequence<User::category_id>& category_sequence, const Entity::cross_target& cross, Entity::entity_id current_dummy_id);
		#pragma endregion GroupA�̃_�~�[�����p���\�b�h
		
		#pragma region GroupB
		void decide_dummy_positions_of_group_b(int num_of_group_b_dummy);
		
		#pragma endregion GroupB�̃_�~�[�����p���\�b�h
		
		//��̓_����Ɏ��ۂɓ��B�\�Ȍo�H�𐶐�����
		std::shared_ptr<std::vector<Graph::MapNodeIndicator>> create_trajectory(Entity::entity_id current_dummy_id, const std::pair<int, Graph::MapNodeIndicator>& basis, const Collection::Sequence<User::category_id>& category_sequence);

	public:
		MizunoMethod(
			std::shared_ptr<Map::BasicDbPreprocessedMap const> map, 
			std::shared_ptr<User::BasicUser<Geography::LatLng>> user, 
			std::shared_ptr<User::PreferenceTree> observed_preference_tree,
			std::shared_ptr<Requirement::PreferenceRequirement const> requirement, 
			std::shared_ptr<Time::TimeSlotManager const> time_manager);
		~MizunoMethod();
	};
}


