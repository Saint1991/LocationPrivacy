#include "stdafx.h"
#include "RevisableTrajectory.h"

namespace Graph
{
	///<summary>
	/// コンストラクタ
	///</summary>
	template <typename POSITION_TYPE>
	RevisableTrajectory<POSITION_TYPE>::RevisableTrajectory(std::shared_ptr<Time::TimeSlotManager const> timeslot)
		: Trajectory<POSITION_TYPE>(timeslot)
	{
	}

	///<summary>
	/// デストラクタ
	///</summary>
	template <typename POSITION_TYPE>
	RevisableTrajectory<POSITION_TYPE>::~RevisableTrajectory()
	{
	}

	///<summary>
	/// positionsを変更不可の状態で取得する
	///</summary>
	template <typename POSITION_TYPE>
	std::shared_ptr<std::vector<std::shared_ptr<POSITION_TYPE>>> RevisableTrajectory<POSITION_TYPE>::read_positions() const
	{
		return positions;
	}

	///<summary>
	/// positionsを変更可の状態で取得する
	///</summary>
	template <typename POSITION_TYPE>
	std::shared_ptr<std::vector<std::shared_ptr<POSITION_TYPE>>> RevisableTrajectory<POSITION_TYPE>::get_positions()
	{
		return positions;
	}

	///<summary>
	/// positionsに，insert_num分だけ，停止phaseを挿入する．
	/// insert_num分だけ，削除を行う．
	///</summary>
	template <typename POSITION_TYPE>
	void RevisableTrajectory<POSITION_TYPE>::insert_positions_to_trajectory(int phase_id, int insert_num)
	{
		positions->insert(positions->begin()+ phase_id, insert_num, positions->at(phase_id));
		for (int i = 0; i < insert_num; i++) positions->pop_back();
	}


	///<summary>
	/// positionsに，delete_num分だけ，停止phaseを挿入する．
	/// delete_num分だけ，複製を行う．
	///</summary>
	template <typename POSITION_TYPE>
	void RevisableTrajectory<POSITION_TYPE>::delete_positions_to_trajectory(int phase_id, int delete_num) 
	{
		positions->erase(positions->begin() + phase_id, positions->begin() + phase_id + delete_num);
		//とりあえず現状は，最後の要素をコピー
		for (int i = 0; i < delete_num; i++) positions->push_back(positions->back());
	}
}