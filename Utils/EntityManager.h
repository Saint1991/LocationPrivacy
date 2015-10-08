#pragma once
#include "stdafx.h"
#include "Coordinate.h"
#include "LatLng.h"
#include "MobileEntity.h"
#include "TimeSlotManager.h"
#include "Rectangle.h"

namespace Entity
{


	///<summary>
	/// ダミーとユーザを管理するクラス
	///</summary>
	template <typename DUMMY, typename USER, typename POSITION_TYPE>
	class EntityManager
	{
	static_assert(std::is_base_of<MobileEntity<Geography::LatLng>, DUMMY>::value || std::is_base_of<MobileEntity<Graph::Coordinate>, DUMMY>::value, "DUMMY must be derived from MobileEntity");
	static_assert(std::is_base_of<MobileEntity<Geography::LatLng>, USER>::value || std::is_base_of<MobileEntity<Graph::Coordinate>, USER>::value, "USER must be derived from MobileEntity");
	
	protected:
		std::shared_ptr<std::vector<std::shared_ptr<DUMMY>>> dummies;
		std::shared_ptr<USER const> user;
		std::shared_ptr<Time::TimeSlotManager const> timeslot;

	public:
		EntityManager(std::shared_ptr<USER> user, int num_of_dummy, std::shared_ptr<Time::TimeSlotManager> timeslot);
		virtual ~EntityManager();

		entity_id create_dummy();
		void set_crossing_point_of_phase(entity_id id1, entity_id id2, POSITION_TYPE position1, POSITION_TYPE position2, int phase);
		void set_crossing_point_at(entity_id id1, entity_id id2, POSITION_TYPE position1, POSITION_TYPE position2, time_t time);
		void set_point_of_phase(entity_id id, POSITION_TYPE position, int phase);
		void set_point_at(entity_id id, POSITION_TYPE position, time_t time);
		std::shared_ptr<USER const> get_user() const;
		std::shared_ptr<DUMMY> get_dummy_by_id(entity_id id);
		std::shared_ptr<DUMMY> find_dummy_if(const std::function<bool(std::shared_ptr<DUMMY const>)>& compare);
		std::shared_ptr<DUMMY const> read_dummy_by_id(entity_id id) const;
		entity_id get_min_cross_entity_id() const;

		int test(int phase, double top, double left, double bottom, double right);
		int test(int phase, Graph::Rectangle rect);

	};
}

#include "EntityManager.hpp"

