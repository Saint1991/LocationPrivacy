#ifdef UTILS_EXPORTS
#define MOBILE_ENTITY_API __declspec(dllexport)
#else
#define MOBILE_ENTITY_API __declspec(dllimport)
#endif

#pragma once
#include "stdafx.h"
#include "Identifiable.h"
#include "Coordinate.h"
#include "LatLng.h"
#include "TimeSlotManager.h"
#include "Probability.h"
#include "MapNodeIndicator.h"
#include "Trajectory.h"
#include "SemanticTrajectory.h"

namespace Entity
{

	typedef unsigned int entity_id;

	///<summary>
	/// 移動体を表すクラス
	/// ユーザ，ダミーを表すのに用いるクラス (ここから派生して別個にUser, Dummyクラスを作ってもよい)
	///</summary>
	template <typename POSITION_TYPE, typename TRAJECTORY_TYPE = Graph::SemanticTrajectory<POSITION_TYPE>>
	class MobileEntity : public Identifiable<entity_id>
	{
	protected:
		std::shared_ptr<TRAJECTORY_TYPE> trajectory;
		std::shared_ptr<std::vector<bool>> cross_flg;
		int total_cross_count;
		
	public:
		typedef std::pair<Graph::MapNodeIndicator, std::shared_ptr<POSITION_TYPE const>> node_pos_info;

		MobileEntity(entity_id id, std::shared_ptr<Time::TimeSlotManager const> timeslot);
		MobileEntity(entity_id id, std::shared_ptr<TRAJECTORY_TYPE> trajectory);
		virtual ~MobileEntity();

		void set_position_of_phase(int phase, const Graph::MapNodeIndicator& node_id, const POSITION_TYPE& position, const std::string& venue_name = "");
		void set_position_at(time_t time, const Graph::MapNodeIndicator& node_id, const POSITION_TYPE& position, const std::string& venue_name = "");

		void set_crossing_position_of_phase(int phase, const Graph::MapNodeIndicator& node_id, POSITION_TYPE position, const std::string& venue_name = "");
		void set_crossing_position_at(time_t time, const Graph::MapNodeIndicator& node_id, POSITION_TYPE position, const std::string& venue_name = "");

		void register_as_cross_position(int phase);
		int get_cross_count() const;
		bool is_cross_set_at_phase(int phase) const;
		bool is_cross_set_at(time_t time) const;
		std::vector<int> find_cross_not_set_phases() const;
		int randomly_pick_cross_not_set_phase() const;
		
		std::pair<int, node_pos_info> find_previous_fixed_position(int phase) const;
		std::pair<int, node_pos_info> find_next_fixed_position(int phase) const;

		std::shared_ptr<POSITION_TYPE const> read_position_of_phase(int phase) const;
		std::shared_ptr<POSITION_TYPE const> read_position_at(time_t time) const;

		node_pos_info read_node_pos_info_of_phase(int phase) const;
		node_pos_info read_node_pos_info_at(time_t time) const;

		void clear_cross_info();
		
		std::shared_ptr<TRAJECTORY_TYPE const> read_trajectory() const;
	};
}

#include "MobileEntity.hpp"

