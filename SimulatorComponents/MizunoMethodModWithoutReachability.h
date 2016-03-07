#ifdef SIMULATORCOMPONENTS_EXPORTS
#define WITHOUT_REACHABILITY_API __declspec(dllexport)
#else
#define WITHOUT_REACHABILITY_API __declspec(dllimport)
#endif

#pragma once
#include "MizunoMethodMod.h"

namespace Method
{
	class  WITHOUT_REACHABILITY_API MizunoMethodModWithoutReachability : public MizunoMethodMod
	{
	protected:
		double calc_poi_score(double ar_size, double setting_anonymous_area, int reachable_entity_count, double already_achieved_anonymous_area_size);

		//基準の点を基に実際に到達可能な経路を生成する
		std::shared_ptr<std::vector<Graph::MapNodeIndicator>> create_trajectory(Entity::entity_id current_dummy_id, const std::pair<int, Graph::MapNodeIndicator>& basis, const Collection::Sequence<User::category_id>& category_sequence);

	public:
		MizunoMethodModWithoutReachability(
			std::shared_ptr<Map::BasicDbMap const> map,
			std::shared_ptr<User::BasicUser<Geography::LatLng>> user,
			std::shared_ptr<User::PreferenceTree> observed_preference_tree,
			std::shared_ptr<Requirement::PreferenceRequirement const> requirement,
			std::shared_ptr<Time::TimeSlotManager const> time_manager);
		~MizunoMethodModWithoutReachability();

		
	};
}


