#ifdef UTILS_EXPORTS
#define TRAJECTORY_API __declspec(dllexport)
#else
#define TRAJECTORY_API __declspec(dllimport)
#endif

#pragma once
#include "TimeSlotManager.h"
#include "LatLng.h"
#include "Coordinate.h"
#include "MapNodeIndicator.h"
#include "FileExportable.h"
#include "Rectangle.h"

namespace Graph
{

	#pragma region TrajectoryState
	///<summary>
	/// トラジェクトリの1Phaseに対応するデータ
	///</summary>
	template <typename POSITION_TYPE = Geography::LatLng>
	struct TRAJECTORY_API TrajectoryState : public IO::FileExportable
	{
		static constexpr char* TIME = "time";
		static constexpr char* VENUE_NAME = "venue_name";

		time_t time;
		std::string venue_name;
		std::shared_ptr<POSITION_TYPE const> position;
		
		TrajectoryState(time_t time, std::shared_ptr<POSITION_TYPE const> position, const std::string& venue_name = "");
		std::unordered_map<std::string, std::string> get_export_data() const;
	};
	template struct TrajectoryState<Geography::LatLng>;
	template struct TrajectoryState<Graph::Coordinate>;
	#pragma endregion TrajectoryState

	///<summary>
	/// トラジェクトリを表すクラス
	/// 位置が未設定の箇所にはnullptrが格納される
	///</summary>
	template <typename POSITION_TYPE = Geography::LatLng>
	class TRAJECTORY_API Trajectory
	{
	protected:
		std::shared_ptr<std::vector<std::shared_ptr<POSITION_TYPE const>>> positions;
		std::shared_ptr<Time::TimeSlotManager const> timeslot;
		std::shared_ptr<std::vector<Graph::MapNodeIndicator>> visited_node_ids;
		std::shared_ptr<std::vector<std::string>> venue_names;

	public:
		typedef std::pair<Graph::MapNodeIndicator, std::shared_ptr<POSITION_TYPE const>> node_pos_info;

		Trajectory(std::shared_ptr<Time::TimeSlotManager const> timeslot);
		Trajectory(std::shared_ptr<Time::TimeSlotManager const> timeslot, std::shared_ptr<std::vector<Graph::MapNodeIndicator>> node_ids, std::shared_ptr<std::vector<std::shared_ptr<POSITION_TYPE const>>> positions, std::shared_ptr<std::vector<std::string>> venue_names = nullptr);
		virtual ~Trajectory();

		size_t phase_count() const;

		bool set_position_of_phase(int phase, const MapNodeIndicator& node_id, const POSITION_TYPE& position, const std::string& venue_name = "");
		bool set_position_at(time_t time, const MapNodeIndicator& node_id, const POSITION_TYPE& position, const std::string& venue_name = "");

		std::shared_ptr<POSITION_TYPE const> position_of_phase(int phase) const;
		std::shared_ptr<POSITION_TYPE const> position_at(time_t time) const;

		node_pos_info read_node_pos_info_of_phase(int phase) const;
		node_pos_info  read_node_pos_info_at(time_t time) const;
		
		std::shared_ptr<TrajectoryState<POSITION_TYPE> const> read_state_of_phase(int phase) const;

		std::shared_ptr<Time::TimeSlotManager const> read_timeslot() const;

		int find_phase_of_time(time_t time) const;

		std::vector<Graph::MapNodeIndicator> sub_trajectory(int from, int to) const;

		Rectangle<Geography::LatLng> get_trajectory_boundary() const;
		virtual void foreach(const std::function<void(int, time_t, std::shared_ptr<POSITION_TYPE const>)>& execute_function) const;
		virtual void foreach_state(const std::function<void(int, long, const Graph::MapNodeIndicator&, std::shared_ptr<TrajectoryState<POSITION_TYPE> const>)>& execute_function) const;
		virtual std::list<std::shared_ptr<IO::FileExportable const>> get_export_data() const;
	};

	template class Trajectory<Geography::LatLng>;
	template class Trajectory<Graph::Coordinate>;
}

