#include "stdafx.h"
#include "SemanticObserver.h"

namespace  Observer
{

	///<summary>
	/// �R���X�g���N�^
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
	void SemanticObserver<DUMMY_TYPE, USER_TYPE>::for_each_category_sequence_of_possible_trajectory(const std::function<void(const Collection::Sequence<std::string>&, double)>& execute_function) const
	{
		for_each_expected_trajectory_frequency([&](const Collection::Sequence<Graph::MapNodeIndicator>& trajectory, double expected_frequency) {
			Collection::Sequence<std::string> category_sequence;
			for (Collection::Sequence<Graph::MapNodeIndicator>::const_iterator iter = trajectory.begin(); iter != trajectory.end(); iter++) {
				if (iter->type() != Graph::NodeType::POI) throw std::invalid_argument("NodeType must be POI");
				std::shared_ptr<Map::BasicPoi const> visited_poi = map->get_static_poi(iter->id());
				category_sequence.push_back(visited_poi->category_id());
				execute_function(category_sequence, expected_frequency);
			}
		});
	}
}
