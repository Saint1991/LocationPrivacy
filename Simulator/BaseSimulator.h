#ifdef SIMULATOR_EXPORTS
#define BASE_SIMULATOR_API __declspec(dllexport)
#else
#define BASE_SIMULATOR_API __declspec(dllimport)
#endif

#pragma once
#include "BasicDbMap.h"
#include "LatLng.h"
#include "BasicUser.h"
#include "Dummy.h"
#include "BasicRequirement.h"
#include "ISimulator.h"
#include "DbTrajectoryLoader.h"
#include "PreferenceTree.h"
#include "Dijkstra.h"

namespace Simulation
{

	///<summary>
	/// シミュレータ実装の一例
	/// user_idはDB上でのユーザのID
	///</summary>
	class BASE_SIMULATOR_API BaseSimulator
		: public ISimulator<Map::BasicDbMap, User::BasicUser<Geography::LatLng>, Entity::Dummy<Geography::LatLng>, Requirement::BasicRequirement, Geography::LatLng, Graph::SemanticTrajectory<Geography::LatLng>>
	{
	private:
		const int TRAJECTORY_LENGTH_THRESHOLD = 3;
		const double TRAININGSET_PROPORTION;
		const unsigned int USER_ID;
	
	protected:
		std::shared_ptr<User::PreferenceTree> user_preference_tree;
		std::shared_ptr<User::PreferenceTree> observed_preference_tree;

		void build_map(const Graph::Rectangle<Geography::LatLng>& boundary);
		void create_trajectories();
		void build_user_preference_tree();
		virtual void make_requirement_list() = 0;
		
	public:
		BaseSimulator(unsigned int user_id = 1, double trainingset_proportion = 0.3);
		virtual ~BaseSimulator();
		void prepare();
		virtual void run() = 0;
		virtual void evaluate() = 0;
		virtual void export_evaluation_result() = 0;
	};
}


