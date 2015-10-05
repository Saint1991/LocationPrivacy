#pragma once
#include "Coordinate.h"
#include "LatLng.h"
#include "MobileEntity.h"
#include "TimeSlotManager.h"

namespace Entity
{
	template <typename DUMMY, typename USER, typename POSITION_TYPE>
	class EntityManager
	{
	static_assert(std::is_base_of<MobileEntity<Geography::LatLng>, DUMMY>::value || std::is_base_of<MobileEntity<Graph::Coordinate>, DUMMY>::value, "DUMMY must be derived from MobileEntity");
	static_assert(std::is_base_of<MobileEntity<Geography::LatLng>, USER>::value || std::is_base_of<MobileEntity<Graph::Coordinate>, USER>::value, "USER must be derived from MobileEntity");
	
	protected:
		std::shared_ptr<std::vector<std::shared_ptr<DUMMY>>> dummies;
		std::shared_ptr<USER> user;
		std::shared_ptr<Time::TimeSlotManager const> timeslot;

	public:
		EntityManager(std::shared_ptr<USER> user, int num_of_dummy, std::shared_ptr<Time::TimeSlotManager> timeslot);
		virtual ~EntityManager();

		entity_id create_dummy();
		void set_crossing_point(entity_id id1, entity_id id2, POSITION_TYPE position1, POSITION_TYPE position2, int phase);
		void set_point(entity_id id, POSITION_TYPE position);
		std::shared_ptr<USER const> get_user() const;
		std::shared_ptr<DUMMY> get_dummy_by_id(entity_id id);
		std::shared_ptr<DUMMY const> read_dummy_by_id(entity_id id) const;

	};
}


