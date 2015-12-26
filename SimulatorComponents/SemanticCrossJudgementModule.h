#pragma once
#include "CrossJudgementModule.h"
#include "ObservedTrajectoryStructure.h"
#include "BasicDbMap.h"
#include "LatLng.h"
#include "SemanticTrajectory.h"
#include "MobileEntity.h"
#include "Dummy.h"
#include "BasicUser.h"
#include "EntityManager.h"

namespace Observer
{


	///<summary>
	/// Œğ·”»’èŠÖ˜A‚Ìˆ—‚ğ‚Ü‚Æ‚ß‚½ƒ‚ƒWƒ…[ƒ‹
	///</summary>
	class SemanticCrossJudgementModule 
		: public Evaluate::CrossJudgementModule<Map::BasicDbMap, Geography::LatLng, Graph::SemanticTrajectory<Geography::LatLng>, Entity::Dummy<Geography::LatLng>, User::BasicUser<Geography::LatLng>>
	{
	public:
		SemanticCrossJudgementModule(
			std::shared_ptr<Map::BasicDbMap const> map,
			std::shared_ptr<Entity::EntityManager<Geography::LatLng, Graph::SemanticTrajectory<Geography::LatLng>, Entity::Dummy<Geography::LatLng>, User::BasicUser<Geography::LatLng>> const> entities,
			double move_speed,
			const std::function<bool(std::shared_ptr<Map::BasicDbMap const>, const Graph::MapNodeIndicator&, const Graph::MapNodeIndicator&, const Graph::MapNodeIndicator&, const Graph::MapNodeIndicator&, double, long)>& cross_rule = nullptr 
		);
		~SemanticCrossJudgementModule();

		std::shared_ptr<ObservedTrajectoryStructure> create_observed_trajectory_structure() const;
	};
}


