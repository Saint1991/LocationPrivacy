#include "stdafx.h"
#include "SemanticTrajectory.h"

namespace Graph
{


	///<summary>
	/// コンストラクタ
	///</summary>
	template <typename POSITION_TYPE>
	SemanticTrajectory<POSITION_TYPE>::SemanticTrajectory(std::unique_ptr<std::vector<time_t>> times, bool use_relative_time)
		: Trajectory<POSITION_TYPE>(std::move(times), use_relative_time)
	{
		category_sequence = std::make_shared<Collection::Sequence<category_id>>(timeslot->phase_count());
	}


	///<summary>
	/// コンストラクタ
	///</summary>
	template <typename POSITION_TYPE>
	SemanticTrajectory<POSITION_TYPE>::SemanticTrajectory(std::unique_ptr<std::vector<std::string>> times, bool use_relative_time)
		: Trajectory<POSITION_TYPE>(std::move(times), use_relative_time)
	{
		category_sequence = std::make_shared<Collection::Sequence<category_id>>(timeslot->phase_count());
	}


	///<summary>
	/// コンストラクタ
	///</summary>
	template <typename POSITION_TYPE>
	SemanticTrajectory<POSITION_TYPE>::SemanticTrajectory(std::shared_ptr<Time::TimeSlotManager> timeslot) 
		: Trajectory<POSITION_TYPE>(timeslot), category_sequence(std::make_shared<Collection::Sequence<category_id>>(timeslot->phase_count()))
	{

	}


	///<summary>
	/// デストラクタ
	///</summary>
	template <typename POSITION_TYPE>
	SemanticTrajectory<POSITION_TYPE>::~SemanticTrajectory()
	{
	}


	///<summary>
	/// 指定したPhaseにおけるカテゴリを設定する
	///</summary>
	template <typename POSITION_TYPE>
	bool SemanticTrajectory<POSITION_TYPE>::set_category_of_phase(int phase, const std::string& category_id)
	{
		if (phase < 0 || category_sequence->size() <= phase) return false;
		category_sequence->at(phase) = category_id;
	}



	///<summary>
	/// 指定した時刻におけるカテゴリを設定する
	///</summary>
	template <typename POSITION_TYPE>
	bool SemanticTrajectory<POSITION_TYPE>::set_category_at(time_t time, const std::string& category_id)
	{
		int phase = timeslot->find_phase_of_time(time);
		return set_category_of_phase(phase, category_id);
	}


	///<summary>
	/// 指定したPhaseにおけるカテゴリIDを返す
	/// 不正なPhaseの場合は空文字列を返す
	///</summary>
	template <typename POSITION_TYPE>
	std::string SemanticTrajectory<POSITION_TYPE>::category_of_phase(int phase) const
	{
		if (phase < 0 || category_sequence->size() <= phase) return "";
		return category_sequence->at(phase);
	}


	///<summary>
	/// 指定した時刻におけるカテゴリIDを返す
	/// 不正な時刻の場合は空文字列を返す
	///</summary>
	template <typename POSITION_TYPE>
	std::string SemanticTrajectory<POSITION_TYPE>::category_at(time_t time) const
	{
		int phase = timeslot->find_phase_of_time(time);
		return category_of_phase(phase);
	}

}
