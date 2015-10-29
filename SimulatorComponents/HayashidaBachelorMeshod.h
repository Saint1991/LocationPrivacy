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

namespace Method {

	///<summary>
	/// 林田さん学部卒業論文の提案手法の実装
	///</summary>
	class HayashidaBachelorMeshod :
		public Framework::IProposedMethod<Map::BasicDbMap, Entity::PauseMobileEntity<Geography::LatLng>, Entity::PauseMobileEntity<Geography::LatLng>, Requirement::KatoMethodRequirement>
	{
	protected:
		std::shared_ptr<Entity::PauseMobileEntity<Geography::LatLng>> predict_user;

		void initialize();
		void decide_dummy_positions();
		void revise_dummy_positions();
		void evaluate();
		void export_results();
		void terminate();
		
	public:
		HayashidaBachelorMeshod(std::shared_ptr<Map::BasicDbMap const> map, std::shared_ptr<Entity::PauseMobileEntity<Geography::LatLng>> user, std::shared_ptr<Requirement::KatoMethodRequirement const> requirement, std::shared_ptr<Time::TimeSlotManager> time_manager);
		~HayashidaBachelorMeshod();

		void run();
	};

}
