#pragma once
#include "CrossJudgementModule.h"
#include "BasicDbMap.h"
#include "SemanticTrajectory.h"
#include "Dummy.h"
#include "BasicUser.h"

namespace Observer
{


	///<summary>
	/// サービス利用観測者を表すクラス
	/// ただしTrajectoryはPOI系列に限る
	///</summary>
	template <typename TRAJECTORY_TYPE = Graph::SemanticTrajectory<Geography::LatLng>, typename DUMMY_TYPE = Entity::Dummy<Geography::LatLng>, typename USER_TYPE = User::BasicUser<Geography::LatLng>>
	class BasicObserver
	{
		static_assert(std::is_base_of<Graph::Trajectory<Geography::LatLng>, TRAJECTORY_TYPE>::value, "TRAJECTORY_TYPE must be derived from Trajectory<LatLng>");
		static_assert(std::is_base_of<Entity::MobileEntity<Geography::LatLng, TRAJECTORY_TYPE>, DUMMY_TYPE>::value, "DUMMY_TYPE must be derived from MobileEntity<LatLng, TRAJECTORY_TYPE>");
		static_assert(std::is_base_of<Entity::MobileEntity<Geography::LatLng, TRAJECTORY_TYPE>, USER_TYPE>::value, "DUMMY_TYPE must be derived from MobileEntity<LatLng, TRAJECTORY_TYPE>");
	private:
		double calc_time_to_confusion(const std::vector<std::vector<Evaluate::CrossInfo>>& cross_infos, int start_phase) const;

	protected:
		std::unique_ptr<Evaluate::CrossJudgementModule<Map::BasicDbMap, Geography::LatLng, TRAJECTORY_TYPE, DUMMY_TYPE, USER_TYPE>> module;
		std::shared_ptr<Entity::EntityManager<Geography::LatLng, TRAJECTORY_TYPE, DUMMY_TYPE, USER_TYPE> const> entities;
		std::shared_ptr<Map::BasicDbMap const> map;
		std::shared_ptr<Observer::ObservedTrajectoryStructure> structure;
	public:
		BasicObserver(
			std::shared_ptr<Map::BasicDbMap const> map,
			std::shared_ptr<Entity::EntityManager<Geography::LatLng, TRAJECTORY_TYPE, DUMMY_TYPE, USER_TYPE> const> entities,
			double move_speed,
			const std::function<bool(std::shared_ptr<Map::BasicDbMap const>, const Graph::MapNodeIndicator&, const Graph::MapNodeIndicator&, const Graph::MapNodeIndicator&, const Graph::MapNodeIndicator&, double, long)>& cross_rule = nullptr
		);
		~BasicObserver();

		std::shared_ptr<Observer::ObservedTrajectoryStructure const> create_observed_trajectory_structure();
		void for_each_possible_trajectory_probability_of_entity(Entity::entity_id id, const std::function<void(const Collection::Sequence<Graph::MapNodeIndicator>, double)>& execute_function);
		void for_each_expected_trajectory_frequency(const std::function<void(const Collection::Sequence<Graph::MapNodeIndicator>, double)>& execute_function);

		double calc_ar_count(double required_anonymous_area) const;
		double calc_ar_size(double required_anonymous_area) const;

		double calc_mtc_without_semantics() const;
	};

	template class BasicObserver<Graph::SemanticTrajectory<Geography::LatLng>, Entity::Dummy<Geography::LatLng>, User::BasicUser<Geography::LatLng>>;
}


