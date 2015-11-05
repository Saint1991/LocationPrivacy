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
	protected:
		std::shared_ptr<User::PreferenceTree> user_preference_tree;
		std::shared_ptr<User::PreferenceTree> observed_preference_tree;
		double testset_proportion;
		int first_trajectory_sequence;
		int user_id;

		void build_map(const Graph::Rectangle<Geography::LatLng>& boundary);
		void create_trajectories();
		void build_user_preference_tree();
		virtual void make_requirement_list() = 0;
		
	public:
		BaseSimulator(int user_id, double testset_proportion);
		virtual ~BaseSimulator();
		void prepare();
		virtual void run();
		virtual void evaluate() = 0;
		virtual void export_evaluation_result(const std::string& export_base_path) = 0;
	};
}


