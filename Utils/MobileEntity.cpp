#include "stdafx.h"
#include "MobileEntity.h"

namespace Entity
{

	///<summary>
	/// コンストラクタ
	///</summary>
	template <typename POSITION_TYPE>
	MobileEntity<POSITION_TYPE>::MobileEntity(entity_id id, std::shared_ptr<Time::TimeSlotManager const> timeslot)
		: Identifiable<entity_id>(id), timeslot(timeslot), positions(std::make_shared<std::vector<std::shared_ptr<POSITION_TYPE>>>(timeslot->phase_count()))
	{
	}


	///<summary>
	/// デストラクタ
	///</summary>
	template <typename POSITION_TYPE>
	MobileEntity<POSITION_TYPE>::~MobileEntity()
	{
	}


	///<summary>
	/// 指定したPhaseにおける位置を設定する
	///</summary>
	template <typename POSITION_TYPE>
	void MobileEntity<POSITION_TYPE>::set_position_of_phase(int phase, POSITION_TYPE position)
	{
		positions->at(phase) = std::make_shared<POSITION_TYPE>(position);
	}

	///<summary>
	/// 与えられたstateを保存する
	///</summary>
	template <typename POSITION_TYPE>
	void MobileEntity<POSITION_TYPE>::set_position_at(time_t time, POSITION_TYPE position)
	{
		int phase = timeslot->find_phase_of_time(time);
		set_position_of_phase(phase, position);
	}

	///<summary>
	/// phase時の位置を指定します
	/// 存在しない場合はnullptrが返ります
	///</summary>
	template <typename POSITION_TYPE>
	const std::shared_ptr<POSITION_TYPE const> MobileEntity<POSITION_TYPE>::read_position_of_phase(int phase) const
	{
		if (positions->size() == 0) return nullptr;
		return positions->at(phase);
	}


	///<summary>
	/// 指定した時刻における位置を返す
	/// 該当するエントリがない場合はnullptrを返す
	///</summary>
	template <typename POSITION_TYPE>
	const std::shared_ptr<POSITION_TYPE const> MobileEntity<POSITION_TYPE>::read_position_at(time_t time) const
	{
		int phase = timeslot->find_phase_of_time(time);
		if (phase == INVALID) return nullptr;
		return read_position_of_phase(phase);
	}
	
}

