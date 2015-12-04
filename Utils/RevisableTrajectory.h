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
	};

	template class RevisableTrajectory<Geography::LatLng>;
	template class RevisableTrajectory<Graph::Coordinate>;
}
