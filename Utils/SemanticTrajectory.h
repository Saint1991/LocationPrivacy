#ifdef UTILS_EXPORTS
#define SEMANTIC_TRAJECTORY_API __declspec(dllexport)
#else
#define SEMANTIC_TRAJECTORY_API __declspec(dllimport)
#endif

#pragma once
#include "Coordinate.h"
#include "LatLng.h"
#include "Sequence.h"
#include "Trajectory.h"

namespace Graph
{

	typedef std::string category_id;

	#pragma region SemanticTrajectoryState
	template <typename POSITION_TYPE = Geography::LatLng>
	struct SEMANTIC_TRAJECTORY_API SemanticTrajectoryState : TrajectoryState<POSITION_TYPE>
	{
		static constexpr char* CATEGORY = "Category";
		category_id category;
		
		SemanticTrajectoryState(time_t time, const category_id& category, std::shared_ptr<POSITION_TYPE> positioin);
		std::unordered_map<std::string, std::string> get_export_data() const;
	};
	template struct SemanticTrajectoryState<Geography::LatLng>;
	template struct SemanticTrajectoryState<Graph::Coordinate>;
	#pragma endregion SemanticTrajectoryState


	template <typename POSITION_TYPE = Geography::LatLng>
	class  SEMANTIC_TRAJECTORY_API SemanticTrajectory : public Trajectory<POSITION_TYPE>
	{
	protected:
		std::shared_ptr<Collection::Sequence<category_id>> category_sequence;
	
	public:
		SemanticTrajectory(std::unique_ptr<std::vector<time_t>> times, bool use_relative_time = true);
		SemanticTrajectory(std::unique_ptr<std::vector<std::string>> times, bool use_relative_time = true);
		SemanticTrajectory(std::shared_ptr<Time::TimeSlotManager> timeslot);
		SemanticTrajectory(std::unique_ptr<std::vector<std::string>> times, std::shared_ptr<std::vector<MapNodeIndicator>>node_ids, std::shared_ptr<std::vector<std::shared_ptr<POSITION_TYPE>>> positions, std::shared_ptr<Collection::Sequence<category_id>>category_sequence, bool use_relative_time = true);
		~SemanticTrajectory();

		bool set_category_of_phase(int phase, const std::string& category_id);
		bool set_category_at(time_t time, const std::string& category_id);

		category_id category_of_phase(int phase) const;
		category_id category_at(time_t time) const;

		Collection::Sequence<category_id> get_category_sequence(int phase_from, int phase_end) const;
		std::list<std::shared_ptr<IO::FileExportable const>> get_export_data() const;
	};

	template class SemanticTrajectory<Geography::LatLng>;
	template class SemanticTrajectory<Graph::Coordinate>;
}


