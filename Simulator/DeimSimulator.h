#ifdef SIMULATOR_EXPORTS
#define DEIM_SIMULATOR_API __declspec(dllexport)
#else
#define DEIM_SIMULATOR_API __declspec(dllimport)
#endif

#pragma once
#include "BaseSimulator.h"
#include "MizunoMethod.h"
#include "MatsunoMethod.h"
#include "PreferenceRequirement.h"
#include "UserTrajectoryConverter.h"
#include "SemanticObserver.h"

namespace Simulation 
{

	///<summary>
	/// DEIM2016向けに作成したシミュレータ By Mizuno
	///</summary>
	class DEIM_SIMULATOR_API DeimSimulator : public BaseSimulator
	{
	private:
		static constexpr unsigned int USER_ID = 557U;
		static constexpr char* DB_NAME = "map_tokyo_category_top_level";
		int current_trajectory_id;
		std::vector<double> similarity_vector_proposed;

		
	protected:
		void make_requirement_list();
		void each_trajectory_end_callback(std::shared_ptr<Entity::EntityManager<Geography::LatLng, Graph::SemanticTrajectory<Geography::LatLng>, Entity::Dummy<>, User::BasicUser<>>> entities, std::shared_ptr<Requirement::PreferenceRequirement const> requirement, std::shared_ptr<Time::Timer> timer);
	
	public:
		DeimSimulator();
		~DeimSimulator();

		void run();
		void evaluate();
		void export_evaluation_result();
	};
}


