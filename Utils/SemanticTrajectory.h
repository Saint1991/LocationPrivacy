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
		static constexpr char* CATEGORY_NAME = "CategoryName";

		category_id category;
		std::string category_name;
		
		SemanticTrajectoryState(time_t time, const category_id& category, std::shared_ptr<POSITION_TYPE const> position, const std::string& venue_name = "", const std::string& category_name = "");
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
		std::shared_ptr<std::vector<std::string>> category_names;
	
	public:
		SemanticTrajectory(std::shared_ptr<Time::TimeSlotManager const> timeslot);
		SemanticTrajectory(
			std::shared_ptr<Time::TimeSlotManager const> timeslot,
			std::shared_ptr<std::vector<Graph::MapNodeIndicator>> node_ids,
			std::shared_ptr<std::vector<std::shared_ptr<POSITION_TYPE const>>> positions,
			std::shared_ptr<Collection::Sequence<category_id>> category_sequence,
			std::shared_ptr<std::vector<std::string>> venue_names = nullptr,
			std::shared_ptr<std::vector<std::string>> category_names = nullptr);
		~SemanticTrajectory();

		bool set_category_of_phase(int phase, const std::string& category_id, const std::string& category_name = "");
		bool set_category_at(time_t time, const std::string& category_id, const std::string& category_name = "");

		category_id category_of_phase(int phase) const;
		category_id category_at(time_t time) const;

		std::shared_ptr<Graph::SemanticTrajectoryState<POSITION_TYPE> const> read_semantic_state_of_phase(int phase) const;

		virtual void foreach(const std::function<void(int, time_t, std::shared_ptr<POSITION_TYPE const>, const category_id&)>& execute_function) const;
		virtual void foreach_state(const std::function<void(int, long, const Graph::MapNodeIndicator&, std::shared_ptr<SemanticTrajectoryState<POSITION_TYPE> const>)>& execute_function) const;
		Collection::Sequence<category_id> get_category_sequence(int phase_from = 0, int phase_end = -1) const;
		std::list<std::shared_ptr<IO::FileExportable const>> get_export_data() const;
	};

	template class SemanticTrajectory<Geography::LatLng>;
	template class SemanticTrajectory<Graph::Coordinate>;
}


