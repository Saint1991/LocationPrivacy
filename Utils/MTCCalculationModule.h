#pragma once
#include "stdafx.h"
#include "EntityManager.h"
#include "MobileEntity.h"
#include "GeoCalculation.h"
#include "GraphUtility.h"

namespace Evaluation {
	template <typename MAP_TYPE, typename ENTITY_MANAGER, typename DUMMY, typename REQUIREMENT, typename POSITION_TYPE = Geography::LatLng>
	class MTCCalculationModule
	{
	private:
		double entropy;
		std::vector<double> MTC;
		std::vector<double> user_probability;
		
	public:
		MTCCalculationModule(std::shared_ptr<MAP_TYPE const> map, std::shared_ptr<ENTITY_MANAGER> entities, std::shared_ptr<REQUIREMENT> requirement, std::shared_ptr<Time::TimeSlotManager> time_manager);
		~MTCCalculationModule();

		bool contains_in_moveable_range(std::shared_ptr<DUMMY>& mobile_entity1, std::shared_ptr<DUMMY>& mobile_entity2, std::shared_ptr<REQUIREMENT> requirement, int phase) const;
		bool check_angle_greater_than_30_degrees(std::shared_ptr<DUMMY>& mobile_entity1, std::shared_ptr<DUMMY>& mobile_entity2, int phase) const;
		void calculate_user_probability(int entity_id1, int entity_id2);
		double calculate_entropy(std::vector<double> user_probability) const;
		double calculate_MTC(std::shared_ptr<MAP_TYPE const> map, std::shared_ptr<ENTITY_MANAGER> entities, std::shared_ptr<REQUIREMENT> requirement, std::shared_ptr<Time::TimeSlotManager> time_manager, int start_phase, int end_phase) const;

	};
}

