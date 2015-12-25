#ifdef SIMULATORCOMPONENTS_EXPORTS
#define HAYASHIDA_BACHELOR_METHOD_API __declspec(dllexport)
#else
#define HAYASHIDA_BACHELOR_METHOD_API __declspec(dllimport)
#endif


#pragma once
#include "IProposedMethod.h"
#include "BasicDbMap.h"
#include "LatLng.h"
#include "BasicUser.h"
#include "Dummy.h"
#include "BasicRequirement.h"
#include "Rectangle.h"
#include "PauseMobileEntity.h"
#include "KatoMethodRequirement.h"
#include "Probability.h"
#include "GeoCalculation.h"
#include "KatoMasterMethod.h"

namespace Method {

	///<summary>
	/// 林田さん学部卒業論文の提案手法の実装
	///</summary>
	class HAYASHIDA_BACHELOR_METHOD_API HayashidaBachelorMethod : public KatoMasterMethod
	{
	protected:
		std::shared_ptr<Entity::PauseMobileEntity<Geography::LatLng>> predicted_user;

		void make_predicted_user();
		bool check_user_going_to_sheduled_POI();
		void re_predicted_user_trajectory();

		void re_setting_of_user_cross();
		std::vector<std::pair<std::pair<entity_id, int>, double>> distance_between_user_and_dummies_at_pause_phases(std::vector<int> all_pause_phases);
		void re_traveling_salesman_problem();

		//void initialize();
		void adjust_dummy_positions();
		//void evaluate();
		//void export_results();
		//void terminate();
		
	public:
		HayashidaBachelorMethod(std::shared_ptr<Map::HayashidaDbMap const> map, std::shared_ptr<Entity::DifferentMovementUser<Geography::LatLng>> user, std::shared_ptr<Requirement::KatoMethodRequirement const> requirement, std::shared_ptr<Time::TimeSlotManager> time_manager);
		~HayashidaBachelorMethod();

		void run();
	};

}
