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

	protected:
		std::unique_ptr<Evaluate::CrossJudgementModule<Map::BasicDbMap, Geography::LatLng, TRAJECTORY_TYPE, DUMMY_TYPE, USER_TYPE>> module;
		std::shared_ptr<Entity::EntityManager<Geography::LatLng, TRAJECTORY_TYPE, DUMMY_TYPE, USER_TYPE> const> entities;
		std::shared_ptr<Map::BasicDbMap const> map;
	public:
		BasicObserver(
			std::shared_ptr<Map::BasicDbMap const> map,
			std::shared_ptr<Entity::EntityManager<Geography::LatLng, TRAJECTORY_TYPE, DUMMY_TYPE, USER_TYPE> const> entities,
			double move_speed,
			const std::function<bool(std::shared_ptr<Map::BasicDbMap const>, const Graph::MapNodeIndicator&, const Graph::MapNodeIndicator&, const Graph::MapNodeIndicator&, const Graph::MapNodeIndicator&, double, long)>& cross_rule = nullptr
		);
		~BasicObserver();

		std::shared_ptr<Observer::ObservedTrajectoryStructure> create_observed_trajectory_structure() const;
	};

	template class BasicObserver<Graph::SemanticTrajectory<Geography::LatLng>, Entity::Dummy<Geography::LatLng>, User::BasicUser<Geography::LatLng>>;
}


