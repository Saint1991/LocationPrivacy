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
		~RevisableTrajectory();

		std::shared_ptr<std::vector<std::shared_ptr<POSITION_TYPE const>>> read_positions() const;
		std::shared_ptr<std::vector<std::shared_ptr<POSITION_TYPE>>> get_positions();

		void insert_positions_to_trajectory(int phase_id, int insert_num);
		void delete_positions_to_trajectory(int phase_id, int delete_num);
		void copy_trajectory(RevisableTrajectory<POSITION_TYPE>& copied_trajectory);
	};

	template class RevisableTrajectory<Geography::LatLng>;
	template class RevisableTrajectory<Graph::Coordinate>;
}
