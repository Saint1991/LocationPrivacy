#include "stdafx.h"
#include "MobileEntity.h"

namespace Entity
{

	///<summary>
	/// コンストラクタ
	///</summary>
	template <typename POSITION_TYPE, typename TRAJECTORY_TYPE>
	MobileEntity<POSITION_TYPE, TRAJECTORY_TYPE>::MobileEntity(entity_id id, std::shared_ptr<Time::TimeSlotManager const> timeslot)
		: Identifiable<entity_id>(id), trajectory(std::make_shared<TRAJECTORY_TYPE>(timeslot)),
		cross_flg(std::make_shared<std::vector<bool>>(timeslot->phase_count(), false)), total_cross_count(0)
	{
	}


	///<summary>
	/// コンストラクタ
	///</summary>
	template <typename POSITION_TYPE, typename TRAJECTORY_TYPE>
	MobileEntity<POSITION_TYPE, TRAJECTORY_TYPE>::MobileEntity(entity_id id, std::shared_ptr<TRAJECTORY_TYPE> trajectory)
		: Identifiable<entity_id>(id), trajectory(trajectory)
	{

	}


	///<summary>
	/// デストラクタ
	///</summary>
	template <typename POSITION_TYPE, typename TRAJECTORY_TYPE>
	MobileEntity<POSITION_TYPE, TRAJECTORY_TYPE>::~MobileEntity()
	{
	}


	///<summary>
	/// 指定したPhaseにおける位置を設定する
	///</summary>
	template <typename POSITION_TYPE, typename TRAJECTORY_TYPE>
	void MobileEntity<POSITION_TYPE, TRAJECTORY_TYPE>::set_position_of_phase(int phase, const Graph::MapNodeIndicator& node_id, const POSITION_TYPE& position)
	{
		trajectory->set_position_of_phase(phase, node_id, position);
	}

	///<summary>
	/// 時刻tにおける位置を設定する
	///</summary>
	template <typename POSITION_TYPE, typename TRAJECTORY_TYPE>
	void MobileEntity<POSITION_TYPE, TRAJECTORY_TYPE>::set_position_at(time_t time, const Graph::MapNodeIndicator& node_id, const POSITION_TYPE& position)
	{
		trajectory->set_position_at(time, node_id, position);
	}


	///<summary>
	/// 共有地点の設定をする
	///</summary>
	template <typename POSITION_TYPE, typename TRAJECTORY_TYPE>
	void MobileEntity<POSITION_TYPE, TRAJECTORY_TYPE>::set_crossing_position_of_phase(int phase, const Graph::MapNodeIndicator& node_id, POSITION_TYPE position)
	{
		register_as_cross_position(phase);
		set_position_of_phase(phase, node_id, position);
	}

	///<summary>
	/// 共有地点の設定をする
	///</summary>
	template <typename POSITION_TYPE, typename TRAJECTORY_TYPE>
	void MobileEntity<POSITION_TYPE, TRAJECTORY_TYPE>::set_crossing_position_at(time_t time, const Graph::MapNodeIndicator& node_id, POSITION_TYPE position)
	{
		int phase = trajectory->find_phase_of_time(time);
		set_crossing_position_of_phase(phase, node_id, position);
	}


	///<summary>
	/// phaseに訪問した地点を交差地点として設定し，交差回数をインクリメントします
	///</summary>
	template <typename POSITION_TYPE, typename TRAJECTORY_TYPE>
	void MobileEntity<POSITION_TYPE, TRAJECTORY_TYPE>::register_as_cross_position(int phase)
	{
		cross_flg->at(phase) = true;
		total_cross_count++;
	}

	///<summary>
	/// 合計交差回数を取得する
	///</summary>
	template <typename POSITION_TYPE, typename TRAJECTORY_TYPE>
	int MobileEntity<POSITION_TYPE, TRAJECTORY_TYPE>::get_cross_count() const
	{
		return total_cross_count;
	}


	///<summary>
	/// 指定時刻に交差が設定済かどうかチェックする
	///</summary>
	template <typename POSITION_TYPE, typename TRAJECTORY_TYPE>
	bool MobileEntity<POSITION_TYPE, TRAJECTORY_TYPE>::is_cross_set_at_phase(int phase) const
	{
		return cross_flg->at(phase);
	}


	///<summary>
	/// 指定時刻に交差が設定済かどうかチェックする
	///</summary>
	template <typename POSITION_TYPE, typename TRAJECTORY_TYPE>
	bool MobileEntity<POSITION_TYPE, TRAJECTORY_TYPE>::is_cross_set_at(time_t time) const
	{
		int phase = trajectory->find_phase_of_time(time);
		return cross_flg->at(phase);
	}


	///<summary>
	/// 交差が設定されていないPhaseを全て取得する
	///</summary>
	template <typename POSITION_TYPE, typename TRAJECTORY_TYPE>
	std::vector<int> MobileEntity<POSITION_TYPE, TRAJECTORY_TYPE>::find_cross_not_set_phases() const
	{
		std::vector<int> ret;
		for (int phase = 0; phase < cross_flg->size(); phase++) {
			if (!cross_flg->at(phase)) ret.push_back(phase);
		}
		return ret;
	}


	///<summary>
	/// 交差が設定されていない時刻を一つランダムに取得する
	/// 設定されていないphaseが存在しない場合はINVALIDを返す
	///</summary>
	template <typename POSITION_TYPE, typename TRAJECTORY_TYPE>
	int MobileEntity<POSITION_TYPE, TRAJECTORY_TYPE>::randomly_pick_cross_not_set_phase() const
	{
		Math::Probability generator;
		std::vector<int> not_set_phases = find_cross_not_set_phases();
		if (not_set_phases.size() == 0) return INVALID;
		return not_set_phases.at(generator.uniform_distribution(0, not_set_phases.size() - 1));
	}


	///<summary>
	/// 指定したphase直前の確定しているポイントの到達時刻と座標を取得します
	/// 決定済みの点がない場合はPhaseがINVALIDの値を返します
	///</summary>
	template <typename POSITION_TYPE, typename TRAJECTORY_TYPE>
	std::pair<int, std::pair<Graph::MapNodeIndicator, std::shared_ptr<POSITION_TYPE const>>> MobileEntity<POSITION_TYPE, TRAJECTORY_TYPE>::find_previous_fixed_position(int phase) const
	{
		for (int target = phase - 1; 0 <= target; target--) {
			node_pos_info info = trajectory->read_node_pos_info_of_phase(target);
			Graph::MapNodeIndicator node_id = info.first;
			std::shared_ptr<POSITION_TYPE const> position = info.second;
			if (position != nullptr) return std::make_pair(target, std::make_pair(node_id, position));
		}
		return std::make_pair(INVALID, std::make_pair(Graph::MapNodeIndicator(INVALID, Graph::NodeType::INVALID), nullptr));
	}



	///<summary>
	/// 指定したphase直後の確定しているポイントの到達時刻と座標を取得します
	/// 決定済みの点がない場合はPhaseがINVALIDの値を返します
	///</summary>
	template <typename POSITION_TYPE, typename TRAJECTORY_TYPE>
	std::pair<int, std::pair<Graph::MapNodeIndicator, std::shared_ptr<POSITION_TYPE const>>> MobileEntity<POSITION_TYPE, TRAJECTORY_TYPE>::find_next_fixed_position(int phase) const
	{
		for (int target = phase + 1; target <trajectory->phase_count(); target++) {
			node_pos_info info = trajectory->read_node_pos_info_of_phase(target);
			Graph::MapNodeIndicator node_id = info.first;
			std::shared_ptr<POSITION_TYPE const> position = info.second;
			if (position != nullptr) return std::make_pair(target, std::make_pair(node_id, position));
		}
		return std::make_pair(INVALID, std::make_pair(Graph::MapNodeIndicator(INVALID, Graph::NodeType::INVALID), nullptr));
	}

	///<summary>
	/// phase時の位置を指定します
	/// 存在しない場合はnullptrが返ります
	///</summary>
	template <typename POSITION_TYPE, typename TRAJECTORY_TYPE>
	std::shared_ptr<POSITION_TYPE const> MobileEntity<POSITION_TYPE, TRAJECTORY_TYPE>::read_position_of_phase(int phase) const
	{
		return trajectory->position_of_phase(phase);
	}


	///<summary>
	/// 指定した時刻における位置を返す
	/// 該当するエントリがない場合はnullptrを返す
	///</summary>
	template <typename POSITION_TYPE, typename TRAJECTORY_TYPE>
	std::shared_ptr<POSITION_TYPE const> MobileEntity<POSITION_TYPE, TRAJECTORY_TYPE>::read_position_at(time_t time) const
	{
		return trajectory->position_at(time);
	}


	///<summary>
	/// 指定したPhaseに存在したノードのIDと位置を取得します
	///</summary>
	template <typename POSITION_TYPE, typename TRAJECTORY_TYPE>
	std::pair<Graph::MapNodeIndicator, std::shared_ptr<POSITION_TYPE const>> MobileEntity<POSITION_TYPE, TRAJECTORY_TYPE>::read_node_pos_info_of_phase(int phase) const
	{
		return trajectory->read_node_pos_info_of_phase(phase);
	}

	///<summary>
	/// 指定したPhaseに存在したノードのIDと位置を取得します
	///</summary>
	template <typename POSITION_TYPE, typename TRAJECTORY_TYPE>
	std::pair<Graph::MapNodeIndicator, std::shared_ptr<POSITION_TYPE const>> MobileEntity<POSITION_TYPE, TRAJECTORY_TYPE>::read_node_pos_info_at(time_t time) const
	{
		return trajectory->read_node_pos_info_at(time);
	}


	///<summary>
	/// トラジェクトリデータを取得する
	///</summary>
	template <typename POSITION_TYPE, typename TRAJECTORY_TYPE>
	std::shared_ptr<TRAJECTORY_TYPE const> MobileEntity<POSITION_TYPE, TRAJECTORY_TYPE>::read_trajectory() const
	{
		return trajectory;
	}
	
}

