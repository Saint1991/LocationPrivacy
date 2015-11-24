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
		
		time_t time;
		std::shared_ptr<POSITION_TYPE> position;
		
		TrajectoryState(time_t time, std::shared_ptr<POSITION_TYPE> position);
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
		std::shared_ptr<std::vector<std::shared_ptr<POSITION_TYPE>>> positions;
		std::shared_ptr<Time::TimeSlotManager> timeslot;
		std::shared_ptr<std::vector<Graph::MapNodeIndicator>> node_ids;

	public:
		Trajectory(std::unique_ptr<std::vector<time_t>> times, bool use_relative_time = true);
		Trajectory(std::unique_ptr<std::vector<std::string>> times, bool use_relative_time = true);
		Trajectory(std::shared_ptr<Time::TimeSlotManager> timeslot);
		Trajectory(std::unique_ptr<std::vector<std::string>> times, std::shared_ptr<std::vector<Graph::MapNodeIndicator>> node_ids, std::shared_ptr<std::vector<std::shared_ptr<POSITION_TYPE>>> positions, bool use_relative_time = true);
		virtual ~Trajectory();

		size_t phase_count() const;

		bool set_position_of_phase(int phase, const POSITION_TYPE& position);
		bool set_position_at(time_t time, const POSITION_TYPE& position);

		std::shared_ptr<POSITION_TYPE const> position_of_phase(int phase) const;
		std::shared_ptr<POSITION_TYPE const> position_at(time_t time) const;

		void foreach(const std::function<void(int, time_t, std::shared_ptr<POSITION_TYPE const>)>& execute_function) const;
		virtual std::list<std::shared_ptr<IO::FileExportable const>> get_export_data() const;
	};

	template class Trajectory<Geography::LatLng>;
	template class Trajectory<Graph::Coordinate>;
}

