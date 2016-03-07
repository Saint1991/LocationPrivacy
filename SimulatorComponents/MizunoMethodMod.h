#ifdef SIMULATORCOMPONENTS_EXPORTS
#define MIZUNOMETHODMOD_API __declspec(dllexport)
#else
#define MIZUNOMETHODMOD_API __declspec(dllimport)
#endif
#pragma once
#include "MizunoMethod.h"
namespace Method
{

	///<summary>
	/// åoòHÇÃê∂ê¨ïîï™ÇÃÇ›â¸ó«ÇµÇΩéËñ@
	///</summary>
	class MIZUNOMETHODMOD_API MizunoMethodMod : public MizunoMethod
	{
	protected:
		virtual std::shared_ptr<std::vector<Graph::MapNodeIndicator>> create_trajectory(Entity::entity_id current_dummy_id, const std::pair<int, Graph::MapNodeIndicator>& basis, const Collection::Sequence<User::category_id>& category_sequence);
		typedef std::pair<std::shared_ptr<Map::BasicPoi const>, double> poi_score_pair;
		typedef std::tuple<std::shared_ptr<Map::BasicPoi const>, double, int> poi_score_tuple;

		void initialize();
		virtual double calc_poi_score(double ar_size, double setting_anonymous_area, int reachable_entity_count, double already_achieved_anonymous_area_size);
	
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


