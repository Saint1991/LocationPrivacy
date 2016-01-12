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
	/// コンストラクタ
	///</summary>
	template <typename POSITION_TYPE>
	RevisableTrajectory<POSITION_TYPE>::RevisableTrajectory(
		std::shared_ptr<Time::TimeSlotManager const> timeslot, 
		std::shared_ptr<std::vector<Graph::MapNodeIndicator>> node_ids, 
		std::shared_ptr<std::vector<std::shared_ptr<POSITION_TYPE const>>> positions, 
		std::shared_ptr<std::vector<std::string>> venue_names = nullptr
	) : Trajectory<POSITION_TYPE>(timeslot, node_ids, positions, venue_names)
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
	std::shared_ptr<std::vector<std::shared_ptr<POSITION_TYPE const>>> RevisableTrajectory<POSITION_TYPE>::read_positions() const
	{
		return positions;
	}

	///<summary>
	/// positionsを変更可の状態で取得する
	///</summary>
	template <typename POSITION_TYPE>
	std::shared_ptr<std::vector<std::shared_ptr<POSITION_TYPE>>> RevisableTrajectory<POSITION_TYPE>::get_positions()
	{
		std::shared_ptr<std::vector<std::shared_ptr<POSITION_TYPE>>> ret = std::make_shared<std::vector<std::shared_ptr<POSITION_TYPE>>>();
		for (std::vector<std::shared_ptr<POSITION_TYPE const>>::iterator iter = positions->begin(); iter != positions->end(); iter++) {
			ret->push_back(std::const_pointer_cast<POSITION_TYPE>(*iter));
		}
		return ret;
	}

	///<summary>
	/// timeslotを変更可の状態で取得する
	///</summary>
	template <typename POSITION_TYPE>
	std::shared_ptr<Time::TimeSlotManager> RevisableTrajectory<POSITION_TYPE>::get_timeslot()
	{
		return std::const_pointer_cast<Time::TimeSlotManager>(timeslot);
	}


	///<summary>
	/// visited_node_idsを変更可の状態で取得する
	///</summary>
	template <typename POSITION_TYPE>
	std::shared_ptr<std::vector<Graph::MapNodeIndicator>> RevisableTrajectory<POSITION_TYPE>::get_visited_node_ids()
	{
		return visited_node_ids;
	}

	///<summary>
	/// venue_namesを変更可の状態で取得する
	///</summary>
	template <typename POSITION_TYPE>
	std::shared_ptr<std::vector<std::string>> RevisableTrajectory<POSITION_TYPE>::get_venue_names()
	{
		return venue_names;
	}


	///<summary>
	/// idと同じ位置が登録されている最初のフェーズを返す
	///</summary>
	template <typename POSITION_TYPE>
	int RevisableTrajectory<POSITION_TYPE>::last_phase()
	{
		return timeslot->last_phase();
	}

	///<summary>
	/// idと同じ位置が登録されている最初のフェーズを返す
	///</summary>
	template <typename POSITION_TYPE>
	int RevisableTrajectory<POSITION_TYPE>::search_phase_same_id(Graph::MapNodeIndicator& id)
	{
		for (auto iter = visited_node_ids->begin(); iter != visited_node_ids->end(); iter++) {
			if ((*iter).id() == id.id()) return std::distance(visited_node_ids->begin(),iter);
		}
	}


	///<summary>
	/// positionsとvisited_node_ids_とvenu_nameに，insert_num分だけ，停止phaseを挿入する．
	///</summary>
	template <typename POSITION_TYPE>
	void RevisableTrajectory<POSITION_TYPE>::insert_positions_to_trajectory(int phase_id, int insert_num)
	{
		positions->insert(positions->begin()+ phase_id, insert_num, positions->at(phase_id));
		visited_node_ids->insert(visited_node_ids->begin() + phase_id, insert_num, visited_node_ids->at(phase_id));
		venue_names->insert(venue_names->begin() + phase_id, insert_num, venue_names->at(phase_id));
	}


	///<summary>
	/// positionsに，delete_num分だけ，停止phaseを挿入する．
	/// delete_num分だけ，複製を行う．
	///</summary>
	template <typename POSITION_TYPE>
	void RevisableTrajectory<POSITION_TYPE>::delete_positions_to_trajectory(int phase_id, int delete_num) 
	{
		//positions->erase(positions->begin() + phase_id, delete_num, positions->at(phase_id));
		//visited_node_ids->erase(visited_node_ids->begin() + phase_id, delete_num, visited_node_ids->at(phase_id));
		//venue_names->erase(venue_names->begin() + phase_id, delete_num, venue_names->at(phase_id));
	}

	///<summary>
	/// トラジェクトリのコピーを行う
	/// target_start_phaseからcopy_start_phase~copy_end_phase分，copied_trajectoryをコピーさせる
	/// その後，target_end_phase以降とvectorを連結させる
	///</summary>
	template <typename POSITION_TYPE>
	void RevisableTrajectory<POSITION_TYPE>::copy_trajectory(int target_start_phase, int target_end_phase, int copy_start_phase, int copy_end_phase, RevisableTrajectory<POSITION_TYPE>& original_trajectory, RevisableTrajectory<POSITION_TYPE>& copied_trajectory)
	{		
		//一旦クリア
		//while (!positions->empty() != target_start_phase + 1) positions->pop_back();
		//while (visited_node_ids->size() != target_start_phase + 1) visited_node_ids->pop_back();
		//while (venue_names->size() != target_start_phase + 1) venue_names->pop_back();
		/*
		//変更分を突っ込む
		positions->insert(positions->end(), copied_trajectory.get_positions()->begin() + copy_start_phase, copied_trajectory.get_positions()->begin() + copy_end_phase);
		visited_node_ids->push_back(visited_node_ids->end(), copied_trajectory.get_visited_node_ids()->begin() + copy_start_phase, copied_trajectory.get_visited_node_ids()->begin() + copy_end_phase);
		venue_names->push_back(venue_names->end(), copied_trajectory.get_venue_names()->begin() + copy_start_phase, copied_trajectory.get_venue_names()->begin() + copy_end_phase);
			
		//変更されていない分を突っ込む
		positions->insert(positions->end(), original_trajectory.get_positions()->prev_positions->begin() + target_end_phase, original_trajectory.get_positions()->prev_positions->end());
		prev_visited_node_ids->insert(visited_node_ids->end(), original_trajectory.get_visited_node_ids()prev_visited_node_ids->begin() + target_end_phase, original_trajectory->get_visited_node_ids()->prev_visited_node_ids->end());
		prev_venue_names->insert(venue_names->end(), original_trajectory.get_venue_names()->begin() + target_end_phase, original_trajectory.get_venue_names()->end());
		*/
	}

	///<summary>
	/// phase_idのnode-idを求める
	///</summary>
	template <typename POSITION_TYPE>
	Graph::MapNodeIndicator RevisableTrajectory<POSITION_TYPE>::get_visited_node_id(int phase_id)
	{
		return visited_node_ids->at(phase_id);
	}

	///<summary>
	/// 訪問POIのIDリストを返す
	///</summary>
	template <typename POSITION_TYPE>
	std::vector<Graph::MapNodeIndicator> RevisableTrajectory<POSITION_TYPE>::read_visited_node_ids()
	{
		return *visited_node_ids;
	}

	///<summary>
	/// 訪問POIのIDリストを返す
	///</summary>
	template <typename POSITION_TYPE>
	void RevisableTrajectory<POSITION_TYPE>::set_positions(std::shared_ptr<std::vector<std::shared_ptr<POSITION_TYPE const>>> copy_positions)
	{
		positions = copy_positions;
	}
}