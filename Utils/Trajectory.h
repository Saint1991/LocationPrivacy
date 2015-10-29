#pragma once
#include "TimeSlotManager.h"
#include "LatLng.h"
#include "Coordinate.h"

namespace Graph
{

	///<summary>
	/// トラジェクトリを表すクラス
	/// 位置が未設定の箇所にはnullptrが格納される
	///</summary>
	template <typename POSITION_TYPE = Geography::LatLng>
	class Trajectory
	{
	protected:
		std::shared_ptr<std::vector<std::shared_ptr<POSITION_TYPE>>> positions;
		std::shared_ptr<Time::TimeSlotManager> timeslot;
	
	public:
		Trajectory(std::unique_ptr<std::vector<time_t>> times, bool use_relative_time = true);
		Trajectory(std::unique_ptr<std::vector<std::string>> times, bool use_relative_time = true);
		Trajectory(std::shared_ptr<Time::TimeSlotManager> timeslot);
		virtual ~Trajectory();

		bool set_position_of_phase(int phase, const POSITION_TYPE& position);
		bool set_position_at(time_t time, const POSITION_TYPE& position);

		std::shared_ptr<POSITION_TYPE const> position_of_phase(int phase) const;
		std::shared_ptr<POSITION_TYPE const> position_at(time_t time) const;

		void foreach(const std::function<void(int, time_t, std::shared_ptr<POSITION_TYPE const>)>& execute_function) const;
	};

	template class Trajectory<Geography::LatLng>;
	template class Trajectory<Graph::Coordinate>;
}

