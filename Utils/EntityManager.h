#pragma once
#include "stdafx.h"
#include "Coordinate.h"
#include "LatLng.h"
#include "GraphUtility.h"
#include "GeoCalculation.h"
#include "MobileEntity.h"
#include "TimeSlotManager.h"
#include "Rectangle.h"

namespace Entity
{

	typedef std::pair<int, entity_id> cross_target;
	constexpr cross_target NOTHING = std::pair<int, entity_id>(-1, -1);

	///<summary>
	/// ダミーとユーザを管理するクラス
	/// 交差回数カウントのため，ユーザもconstにしていないが，基本positionsには操作は加えないこと
	/// POSITION_TYPEはCoordinateもしくはLatLngの派生クラス
	///</summary>
	template <typename DUMMY, typename USER, typename POSITION_TYPE = Geography::LatLng>
	class EntityManager
	{
	
	static_assert(std::is_base_of<Geography::LatLng, POSITION_TYPE>::value || std::is_same<Graph::Coordinate, POSITION_TYPE>::value, "POSITION_TYPE must be Coordinate or the class derived from LatLng");
	static_assert(std::is_base_of<MobileEntity<Geography::LatLng>, DUMMY>::value, "DUMMY must be derived from MobileEntity");
	static_assert(std::is_base_of<MobileEntity<Geography::LatLng>, USER>::value, "USER must be derived from MobileEntity");
	
	protected:
		std::shared_ptr<std::vector<std::shared_ptr<DUMMY>>> dummies;
		std::shared_ptr<USER> user;
		std::shared_ptr<Time::TimeSlotManager const> timeslot;

	public:
		EntityManager(std::shared_ptr<USER> user, int num_of_dummy, std::shared_ptr<Time::TimeSlotManager const> timeslot);
		virtual ~EntityManager();

		entity_id create_dummy();
		std::shared_ptr<USER> get_user();
		std::shared_ptr<DUMMY> get_dummy_by_id(entity_id id);
		std::shared_ptr<DUMMY> find_dummy_if(const std::function<bool(std::shared_ptr<DUMMY const>)>& compare);
		std::vector<std::shared_ptr<DUMMY>> find_all_dummies_if(const std::function<bool(std::shared_ptr<DUMMY const>)>& compare);
		std::shared_ptr<DUMMY const> read_dummy_by_id(entity_id id) const;
		std::list<std::pair<entity_id, int>> get_entity_id_list_order_by_cross_count() const;
		std::vector<entity_id> get_entities_cross_with(entity_id id) const;
		size_t get_dummy_count() const;
		size_t get_total_cross_count_of_phase(int phase) const;
		
		void for_each_dummy(const std::function<void(entity_id, std::shared_ptr<DUMMY>)>& execute_function);
		void for_each_dummy(const std::function<void(entity_id, std::shared_ptr<DUMMY const>)>& execute_function) const;

		std::vector<std::shared_ptr<POSITION_TYPE const>> get_all_fixed_positions_of_phase(int phase) const;
		void positions_for_each_phase(const std::function<void(int, time_t, const std::vector<std::shared_ptr<POSITION_TYPE const>>&)>& execute_function) const;
		double calc_convex_hull_size_of_fixed_entities_of_phase(int phase) const;

		std::shared_ptr<POSITION_TYPE const> get_average_position_of_phase(int phase) const;
		std::shared_ptr<POSITION_TYPE const> get_average_position_at(time_t time) const;
		int get_entity_count_within_boundary(int phase, const Graph::Rectangle<POSITION_TYPE>& boundary) const;
		int get_entity_count_within_boundary(int phase, double top, double left, double bottom, double right) const;
		int get_all_entities_total_crossing_count() const;
	};
}

#include "EntityManager.hpp"

