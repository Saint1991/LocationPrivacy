#ifdef SIMULATORCOMPONENTS_EXPORTS
#define MIZUNOMETHODSAMESEQUENCE_API __declspec(dllexport)
#else
#define MIZUNOMETHODSAMESEQUENCE_API __declspec(dllimport)
#endif

#pragma once
#include "MizunoMethodModWithoutReachability.h"

namespace Method
{

	///<summary>
	/// 毎回ユーザと同じシーケンスにするVer
	///</summary>
	class  MIZUNOMETHODSAMESEQUENCE_API MizunoMethodSameSequence : public MizunoMethodModWithoutReachability
	{
	protected:
		std::vector<sequence_score_set> calc_sequence_score_set(Entity::entity_id current_dummy_id);
	
	public:
		MizunoMethodSameSequence(
			std::shared_ptr<Map::BasicDbMap const> map,
			std::shared_ptr<User::BasicUser<Geography::LatLng>> user,
			std::shared_ptr<User::PreferenceTree> observed_preference_tree,
			std::shared_ptr<Requirement::PreferenceRequirement const> requirement,
			std::shared_ptr<Time::TimeSlotManager const> time_manager);
		~MizunoMethodSameSequence();

		
	};
}


