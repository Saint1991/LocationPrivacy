#include "stdafx.h"
#include "SemanticObserver.h"

namespace  Observer
{

	///<summary>
	/// コンストラクタ
	///</summary>
	template <typename DUMMY_TYPE, typename USER_TYPE>
	SemanticObserver<DUMMY_TYPE, USER_TYPE>::SemanticObserver(
		std::shared_ptr<Map::BasicDbMap const> map,
		std::shared_ptr<Entity::EntityManager<Geography::LatLng, Graph::SemanticTrajectory<Geography::LatLng>, DUMMY_TYPE, USER_TYPE> const> entities,
		std::shared_ptr<User::PreferenceTree> preference,
		double move_speed,
		const std::function<bool(std::shared_ptr<Map::BasicDbMap const>, const Graph::MapNodeIndicator&, const Graph::MapNodeIndicator&, const Graph::MapNodeIndicator&, const Graph::MapNodeIndicator&, double, long)>& cross_rule
		) : BasicObserver<Graph::SemanticTrajectory<Geography::LatLng>, DUMMY_TYPE, USER_TYPE>(map, entities, move_speed, cross_rule), observed_preference(preference)
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
	/// Semanticsを考慮したMTCの計算 (未実装)
	///</summary>
	template <typename DUMMY_TYPE, typename USER_TYPE>
	double SemanticObserver<DUMMY_TYPE, USER_TYPE>::calc_mtc_with_semantics() const
	{
		return 0.0;
	}
}
