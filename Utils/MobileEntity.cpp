#include "stdafx.h"
#include "MobileEntity.h"

namespace Entity
{

	///<summary>
	/// コンストラクタ
	///</summary>
	template <typename POSITION_TYPE>
	MobileEntity<POSITION_TYPE>::MobileEntity(entity_id id, std::shared_ptr<Time::TimeSlotManager const> timeslot)
		: Identifiable<entity_id>(id), timeslot(timeslot), positions(std::make_shared<std::vector<std::shared_ptr<POSITION_TYPE>>>(timeslot->phase_count(), nullptr)), cross_flg(std::make_shared<std::vector<bool>>(timeslot->phase_count(), false)), total_cross_count(0)
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
	/// 時刻tにおける位置を設定する
	///</summary>
	template <typename POSITION_TYPE>
	void MobileEntity<POSITION_TYPE>::set_position_at(time_t time, POSITION_TYPE position)
	{
		int phase = timeslot->find_phase_of_time(time);
		set_position_of_phase(phase, position);
	}


	///<summary>
	/// 共有地点の設定をする
	///</summary>
	template <typename POSITION_TYPE>
	void MobileEntity<POSITION_TYPE>::set_crossing_position_of_phase(int phase, POSITION_TYPE position)
	{
		register_as_cross_position(phase);
		set_position_of_phase(phase, position);
	}

	///<summary>
	/// 共有地点の設定をする
	///</summary>
	template <typename POSITION_TYPE>
	void MobileEntity<POSITION_TYPE>::set_crossing_position_at(time_t time, POSITION_TYPE position)
	{
		int phase = timeslot->find_phase_of_time(time);
		set_crossing_position_of_phase(phase, position);
	}


	///<summary>
	/// phaseに訪問した地点を交差地点として設定し，交差回数をインクリメントします
	///</summary>
	template <typename POSITION_TYPE>
	void MobileEntity<POSITION_TYPE>::register_as_cross_position(int phase)
	{
		cross_flg->at(phase) = true;
		total_cross_count++;
	}

	///<summary>
	/// 合計交差回数を取得する
	///</summary>
	template <typename POSITION_TYPE>
	int MobileEntity<POSITION_TYPE>::get_cross_count() const
	{
		return total_cross_count;
	}


	///<summary>
	/// 指定時刻に交差が設定済かどうかチェックする
	///</summary>
	template <typename POSITION_TYPE>
	bool MobileEntity<POSITION_TYPE>::is_cross_set_at_phase(int phase) const
	{
		return cross_flg->at(phase);
	}


	///<summary>
	/// 指定時刻に交差が設定済かどうかチェックする
	///</summary>
	template <typename POSITION_TYPE>
	bool MobileEntity<POSITION_TYPE>::is_cross_set_at(time_t time) const
	{
		int phase = timeslot->find_phase_of_time(time);
		return cross_flg->at(phase);
	}


	///<summary>
	/// 交差が設定されていないPhaseを全て取得する
	///</summary>
	template <typename POSITION_TYPE>
	std::vector<int> MobileEntity<POSITION_TYPE>::find_cross_not_set_phases() const
	{
		std::vector<int> ret;
		for (int phase = 0; phase < cross_flg->size(); phase++) {
			if (cross_flg->at(phase)) ret.push_back(phase);
		}
		return ret;
	}


	///<summary>
	/// 交差が設定されていない時刻を一つランダムに取得する
	/// 設定されていないphaseが存在しない場合はINVALIDを返す
	///</summary>
	template <typename POSITION_TYPE>
	int MobileEntity<POSITION_TYPE>::randomly_pick_cross_not_set_phase() const
	{
		Math::Probability generator;
		std::vector<int> not_set_phases = find_cross_not_set_phases();
		if (not_set_phases.size() == 0) return INVALID;
		return not_set_phases.at(generator.uniform_distribution(0, not_set_phases.size() - 1));
	}


	///<summary>
	/// 指定したphase直前の確定しているポイントの到達時刻と座標を取得します
	/// 決定済みの点がない場合は(INVALID, nullptr)のペアを返します．
	///</summary>
	template <typename POSITION_TYPE>
	std::pair<int, std::shared_ptr<POSITION_TYPE const>> MobileEntity<POSITION_TYPE>::find_previous_fixed_position(int phase) const
	{
		for (int target = phase - 1; 0 <= target; target--) {
			std::shared_ptr<POSITION_TYPE const> position = positions->at(target);
			if (position != nullptr) return std::make_pair(target, position);
		}
		return std::make_pair(INVALID, nullptr);
	}



	///<summary>
	/// 指定したphase直後の確定しているポイントの到達時刻と座標を取得します
	/// 決定済みの点がない場合は(INVALID, nullptr)のペアを返します．
	///</summary>
	template <typename POSITION_TYPE>
	std::pair<int, std::shared_ptr<POSITION_TYPE const>> MobileEntity<POSITION_TYPE>::find_next_fixed_position(int phase) const
	{
		for (int target = phase + 1; target < positions->size(); target++) {
			std::shared_ptr<POSITION_TYPE const> position = positions->at(target);
			if (position != nullptr) return std::make_pair(target, position);
		}
		return std::make_pair(INVALID, nullptr);
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

