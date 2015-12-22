#ifdef SIMULATOR_EXPORTS
#define PAIS_SIMULATOR_API __declspec(dllexport)
#else
#define PAIS_SIMULATOR_API __declspec(dllimport)
#endif

#pragma once
#include "BaseSimulator.h"
#include "MizunoMethod.h"
#include "MatsunoMethod.h"
#include "PreferenceRequirement.h"
#include "UserTrajectoryConverter.h"

namespace Simulation 
{

	///<summary>
	/// PAIS2016向けに作成したシミュレータ By Mizuno
	///</summary>
	class PAIS_SIMULATOR_API PaisSimulator : public BaseSimulator
	{
	private:
		static constexpr unsigned int USER_ID = 557U;
		static constexpr char* DB_NAME = "map_tokyo_category_top_level";
		
	protected:
		void make_requirement_list();
		void each_trajectory_end_callback(std::shared_ptr<Entity::EntityManager<Geography::LatLng, Graph::SemanticTrajectory<Geography::LatLng>, Entity::Dummy<>, User::BasicUser<>>> entities, std::shared_ptr<Requirement::PreferenceRequirement const> requirement, std::shared_ptr<Time::Timer> timer);
	
	public:
		PaisSimulator();
		~PaisSimulator();

		void run();
		void evaluate();
		void export_evaluation_result();
	};
}


