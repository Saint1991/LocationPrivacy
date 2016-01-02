#pragma once
#include "BasicObserver.h"
#include "SemanticTrajectory.h"
#include "PreferenceTree.h"

namespace Observer
{
	template <typename DUMMY_TYPE, typename USER_TYPE>
	class SemanticObserver : public BasicObserver<Graph::SemanticTrajectory<Geography::LatLng>, DUMMY_TYPE, USER_TYPE>
	{
	protected:
		std::shared_ptr<User::PreferenceTree const> observed_preference;

	public:
		SemanticObserver(
			std::shared_ptr<Map::BasicDbMap const> map,
			std::shared_ptr<Entity::EntityManager<Geography::LatLng, Graph::SemanticTrajectory<Geography::LatLng>, DUMMY_TYPE, USER_TYPE> const> entities,
			std::shared_ptr<User::PreferenceTree> preference,
			double move_speed,
			const std::function<bool(std::shared_ptr<Map::BasicDbMap const>, const Graph::MapNodeIndicator&, const Graph::MapNodeIndicator&, const Graph::MapNodeIndicator&, const Graph::MapNodeIndicator&, double, long)>& cross_rule = nullptr
		);
		~SemanticObserver();

		void for_each_category_sequence_of_possible_trajectory(const std::function<void(const Collection::Sequence<std::string>&, double)>& execute_function) const;
	};
}


