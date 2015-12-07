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
	/// �ѓc����w�����Ƙ_���̒�Ď�@�̎���
	///</summary>
	class HayashidaBachelorMethod :
		public KatoMasterMethod
	{
	protected:
		std::shared_ptr<Entity::PauseMobileEntity<Geography::LatLng>> predicted_user;

		void make_predicted_user();
		bool check_user_plan();


		void initialize();
		void adjust_dummy_positions();
		void evaluate();
		void export_results();
		void terminate();
		
	public:
		HayashidaBachelorMethod(std::shared_ptr<Map::BasicDbMap const> map, std::shared_ptr<Entity::PauseMobileEntity<Geography::LatLng>> user, std::shared_ptr<Requirement::KatoMethodRequirement const> requirement, std::shared_ptr<Time::TimeSlotManager> time_manager);
		~HayashidaBachelorMethod();

		void run();
	};

}
