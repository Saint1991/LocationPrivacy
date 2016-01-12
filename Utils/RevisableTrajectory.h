#ifdef UTILS_EXPORTS
#define REVISABLE_TRAJECTORY_API __declspec(dllexport)
#else
#define REVISABLE_TRAJECTORY_API __declspec(dllimport)
#endif

#pragma once
#include "Coordinate.h"
#include "LatLng.h"
#include "Sequence.h"
#include "Trajectory.h"

namespace Graph
{
	template <typename POSITION_TYPE = Geography::LatLng>
	class REVISABLE_TRAJECTORY_API RevisableTrajectory : public Trajectory<POSITION_TYPE>
	{
	public:
		RevisableTrajectory(std::shared_ptr<Time::TimeSlotManager const> timeslot);
		RevisableTrajectory(std::shared_ptr<Time::TimeSlotManager const> timeslot, std::shared_ptr<std::vector<Graph::MapNodeIndicator>> node_ids, std::shared_ptr<std::vector<std::shared_ptr<POSITION_TYPE const>>> positions, std::shared_ptr<std::vector<std::string>> venue_names = nullptr);

		~RevisableTrajectory();

		std::shared_ptr<std::vector<std::shared_ptr<POSITION_TYPE const>>> read_positions() const;
		std::shared_ptr<std::vector<std::shared_ptr<POSITION_TYPE>>> get_positions();
		std::shared_ptr<Time::TimeSlotManager> get_timeslot();
		std::shared_ptr<std::vector<Graph::MapNodeIndicator>> get_visited_node_ids();
		std::shared_ptr<std::vector<std::string>> get_venue_names();

		int last_phase();
		int search_phase_same_id(Graph::MapNodeIndicator& id);
		void insert_positions_to_trajectory(int phase_id, int insert_num = 1);
		void delete_positions_to_trajectory(int phase_id, int delete_num = 1);
		void copy_trajectory(int target_start_phase, int target_end_phase, int copy_start_phase, int copy_end_phase, RevisableTrajectory<POSITION_TYPE>& original_trajectory, RevisableTrajectory<POSITION_TYPE>& copied_trajectory);
		Graph::MapNodeIndicator get_visited_node_id(int phase_id);
		std::vector<Graph::MapNodeIndicator> read_visited_node_ids();

		void set_positions(std::shared_ptr<std::vector<std::shared_ptr<POSITION_TYPE const>>> positions);
	};

	template class RevisableTrajectory<Geography::LatLng>;
	template class RevisableTrajectory<Graph::Coordinate>;
}
