#ifdef SIMULATORCOMPONENTS_EXPORTS
#define SEMANTIC_OBSERVER_API __declspec(dllexport)
#else
#define SEMANTIC_OBSERVER_API __declspec(dllimport)
#endif

#pragma once
#include "BasicObserver.h"
#include "SemanticTrajectory.h"
#include "PreferenceTree.h"
#include "SemanticObservedTrajectoryStructure.h"

namespace Observer
{
	template <typename DUMMY_TYPE, typename USER_TYPE>
	class SEMANTIC_OBSERVER_API SemanticObserver : public BasicObserver<Graph::SemanticTrajectory<Geography::LatLng>, DUMMY_TYPE, USER_TYPE>
	{
	private:
		double calc_time_to_confusion_with_semantics(std::shared_ptr<std::vector<std::vector<Evaluate::CrossInfo>>> cross_infos, std::shared_ptr<SemanticObservedTrajectoryStructure> semantic_observed_trajectory_structure, int start_phase, double threshold = 1.0);

	protected:
		int confusion_total_count_with_semantics;
		int confusion_achieve_count_with_semantics;
		std::shared_ptr<User::PreferenceTree const> preference;
		std::shared_ptr<SemanticObservedTrajectoryStructure> semantic_observed_trajectory_structure;

	public:
		SemanticObserver(
			std::shared_ptr<Map::BasicDbMap const> map,
			std::shared_ptr<Entity::EntityManager<Geography::LatLng, Graph::SemanticTrajectory<Geography::LatLng>, DUMMY_TYPE, USER_TYPE> const> entities,
			std::shared_ptr<User::PreferenceTree> preference,
			double move_speed,
			const std::function<bool(std::shared_ptr<Map::BasicDbMap const>, const Graph::MapNodeIndicator&, const Graph::MapNodeIndicator&, const Graph::MapNodeIndicator&, const Graph::MapNodeIndicator&, double, long)>& cross_rule = nullptr
		);
		~SemanticObserver();

		void for_each_category_sequence_of_possible_trajectory(const std::function<void(const Collection::Sequence<std::string>&, double)>& execute_function);
		std::shared_ptr<SemanticObservedTrajectoryStructure const> create_semantic_observed_trajectory_structure();
		double calc_mtc_with_semantics(double threshold = 1.0);
		double get_confusion_achieve_ratio_with_semantics() const;
	};

	template class SemanticObserver<Entity::Dummy<Geography::LatLng>, User::BasicUser<Geography::LatLng>>;
}


