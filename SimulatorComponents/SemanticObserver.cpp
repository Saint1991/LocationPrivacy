#include "stdafx.h"
#include "SemanticObserver.h"

namespace  Observer
{

	///<summary>
	/// �R���X�g���N�^
	/// (����̓��e�ł�preference�ɂ͐^�̚n�D��n���ׂ�)
	///</summary>
	template <typename DUMMY_TYPE, typename USER_TYPE>
	SemanticObserver<DUMMY_TYPE, USER_TYPE>::SemanticObserver(
		std::shared_ptr<Map::BasicDbMap const> map,
		std::shared_ptr<Entity::EntityManager<Geography::LatLng, Graph::SemanticTrajectory<Geography::LatLng>, DUMMY_TYPE, USER_TYPE> const> entities,
		std::shared_ptr<User::PreferenceTree> preference,
		double move_speed,
		const std::function<bool(std::shared_ptr<Map::BasicDbMap const>, const Graph::MapNodeIndicator&, const Graph::MapNodeIndicator&, const Graph::MapNodeIndicator&, const Graph::MapNodeIndicator&, double, long)>& cross_rule
		) : BasicObserver<Graph::SemanticTrajectory<Geography::LatLng>, DUMMY_TYPE, USER_TYPE>(map, entities, move_speed, cross_rule), preference(preference)
	{
	}


	///<summary>
	/// �f�X�g���N�^
	///</summary>
	template <typename DUMMY_TYPE, typename USER_TYPE>
	SemanticObserver<DUMMY_TYPE, USER_TYPE>::~SemanticObserver()
	{
	}


	///<summary>
	/// �ϑ����ꂽ�g���W�F�N�g������N���蓾���J�e�S���V�[�P���X���v�Z���A
	/// ���̊e�V�[�P���X�Ɣ����x���̊��Ғl�ɂ���execute_function�����s����
	/// ����������V�[�P���X�ł���x�Ŏ��s�����Ƃ͌���Ȃ��_�ɒ���
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
	/// SemanticObservedStructure���쐬����
	/// ���̍\���ł́C�G�b�W���ړ������G���e�B�e�B���̊��Ғl�I�䗦��\���C
	/// �m�[�h���̈ړ��o�H�W���ɑ΂��C�T�|�[�g�l��ݐς��Ă��������̂�ێ�����D
	///</summary>
	template <typename DUMMY_TYPE, typename USER_TYPE>
	std::shared_ptr<ObservedTrajectoryStructure const> SemanticObserver<DUMMY_TYPE, USER_TYPE>::create_semantic_observed_trajectory_structure() {
		
		if (structure == nullptr) create_observed_trajectory_structure();
		std::shared_ptr<ObservedTrajectoryStructure> semantic_structure = std::make_shared<ObservedTrajectoryStructure>(*structure);
		semantic_structure->clear_node_counts();

		//�����possible_trajectory�̃G���e�B�e�B�J�E���g���T�|�[�g�̗ݐςɒu������������
		structure->for_each_possible_trajectory([&](const Collection::Sequence<Graph::MapNodeIndicator>& trajectory) {
			Collection::Sequence<User::category_id> sequence = map->convert_to_category_sequence(trajectory);
			double support = preference->get_support_of(sequence);

			//trajectory�ɑΉ����邷�ׂẴm�[�h�̃J�E���g��support�𑫂�
			ObservedTrajectoryStructure::base_iterator iter = semantic_structure->root<ObservedTrajectoryStructure::base_iterator>();
			for (Collection::Sequence<Graph::MapNodeIndicator>::const_iterator node_indicator = trajectory.begin(); node_indicator != trajectory.end(); node_indicator++) {
				iter = iter.find_child_if([&node_indicator](std::shared_ptr<ObservedTrajectoryNode const> node) {
					return *node->data == *node_indicator;
				});
				iter->add_count(support);
			}
		});

		semantic_observed_trajectory_structure = semantic_structure;
		return semantic_structure;
	}

	///<summary>
	/// Semantics���l������MTC�̌v�Z (������)
	///</summary>
	template <typename DUMMY_TYPE, typename USER_TYPE>
	double SemanticObserver<DUMMY_TYPE, USER_TYPE>::calc_mtc_with_semantics()
	{
		std::shared_ptr<ObservedTrajectoryStructure const> semantic_structure = create_semantic_observed_trajectory_structure();
		

		return 0.0;
	}
}
