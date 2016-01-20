#pragma once
#include "MizunoMethod.h"
namespace Method
{

	///<summary>
	/// Œo˜H‚Ì¶¬•”•ª‚Ì‚İ‰ü—Ç‚µ‚½è–@
	///</summary>
	class MizunoMethodMod : public MizunoMethod
	{
	protected:
		std::shared_ptr<std::vector<Graph::MapNodeIndicator>> create_trajectory(Entity::entity_id current_dummy_id, const std::pair<int, Graph::MapNodeIndicator>& basis, const Collection::Sequence<User::category_id>& category_sequence);
		typedef std::pair<std::shared_ptr<Map::BasicPoi const>, double> poi_score_pair;

	public:
		MizunoMethodMod(
				std::shared_ptr<Map::BasicDbMap const> map,
				std::shared_ptr<User::BasicUser<Geography::LatLng>> user,
				std::shared_ptr<User::PreferenceTree> observed_preference_tree,
				std::shared_ptr<Requirement::PreferenceRequirement const> requirement,
				std::shared_ptr<Time::TimeSlotManager const> time_manager);
		~MizunoMethodMod();
	};
}


