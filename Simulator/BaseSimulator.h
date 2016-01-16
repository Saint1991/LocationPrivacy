#ifdef SIMULATOR_EXPORTS
#define BASE_SIMULATOR_API __declspec(dllexport)
#else
#define BASE_SIMULATOR_API __declspec(dllimport)
#endif

#pragma once
#include "LatLng.h"
#include "BasicUser.h"
#include "Dummy.h"
#include "PreferenceRequirement.h"
#include "ISimulator.h"
#include "DbTrajectoryLoader.h"
#include "PreferenceTree.h"
#include "Dijkstra.h"
#include "TimeUtility.h"
#include "UserTrajectoryConverter.h"
#include "BasicDbPreprocessedMap.h"

namespace Simulation
{

	///<summary>
	/// シミュレータ実装の一例
	/// user_idはDB上でのユーザのID
	///</summary>
	class BASE_SIMULATOR_API BaseSimulator
		: public ISimulator<Map::BasicDbPreprocessedMap, User::BasicUser<Geography::LatLng>, Entity::Dummy<Geography::LatLng>, Requirement::PreferenceRequirement, Geography::LatLng, Graph::SemanticTrajectory<Geography::LatLng>>
	{
	private:
		//トラジェクトリ加工用 (最低この間隔がない場合間隔を広げる)
		static constexpr int MIN_SERVICE_INTERVAL = 0;
		const std::string DB_NAME;
		const unsigned int USER_ID;
	
	protected:
		static constexpr double AVERAGE_SPEED = 4.0;
		const int TRAJECTORY_LENGTH_THRESHOLD = 4;
		std::shared_ptr<User::PreferenceTree> user_preference_tree;
		std::shared_ptr<User::PreferenceTree> observed_preference_tree;

		void build_map(const Graph::Rectangle<Geography::LatLng>& boundary = { 35.9, 139.4, 35.5, 141.0 });
		void create_trajectories();
		void build_user_preference_tree();
		virtual void make_requirement_list() = 0;
		
	public:
		BaseSimulator(unsigned int user_id = 1, const std::string& db_name = "map_tokyo_category_top_level");
		virtual ~BaseSimulator();
		void prepare();
		virtual void run() = 0;
		virtual void evaluate() = 0;
		virtual void export_evaluation_result() = 0;
	};
}


