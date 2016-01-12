#include "stdafx.h"
#include "RevisableTrajectory.h"

namespace Graph
{
	///<summary>
	/// �R���X�g���N�^
	///</summary>
	template <typename POSITION_TYPE>
	RevisableTrajectory<POSITION_TYPE>::RevisableTrajectory(std::shared_ptr<Time::TimeSlotManager const> timeslot)
		: Trajectory<POSITION_TYPE>(timeslot)
	{
	}

	///<summary>
	/// �R���X�g���N�^
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
	/// �f�X�g���N�^
	///</summary>
	template <typename POSITION_TYPE>
	RevisableTrajectory<POSITION_TYPE>::~RevisableTrajectory()
	{
	}

	///<summary>
	/// positions��ύX�s�̏�ԂŎ擾����
	///</summary>
	template <typename POSITION_TYPE>
	std::shared_ptr<std::vector<std::shared_ptr<POSITION_TYPE const>>> RevisableTrajectory<POSITION_TYPE>::read_positions() const
	{
		return positions;
	}

	///<summary>
	/// positions��ύX�̏�ԂŎ擾����
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
	/// timeslot��ύX�̏�ԂŎ擾����
	///</summary>
	template <typename POSITION_TYPE>
	std::shared_ptr<Time::TimeSlotManager> RevisableTrajectory<POSITION_TYPE>::get_timeslot()
	{
		return std::const_pointer_cast<Time::TimeSlotManager>(timeslot);
	}


	///<summary>
	/// visited_node_ids��ύX�̏�ԂŎ擾����
	///</summary>
	template <typename POSITION_TYPE>
	std::shared_ptr<std::vector<Graph::MapNodeIndicator>> RevisableTrajectory<POSITION_TYPE>::get_visited_node_ids()
	{
		return visited_node_ids;
	}

	///<summary>
	/// venue_names��ύX�̏�ԂŎ擾����
	///</summary>
	template <typename POSITION_TYPE>
	std::shared_ptr<std::vector<std::string>> RevisableTrajectory<POSITION_TYPE>::get_venue_names()
	{
		return venue_names;
	}


	///<summary>
	/// id�Ɠ����ʒu���o�^����Ă���ŏ��̃t�F�[�Y��Ԃ�
	///</summary>
	template <typename POSITION_TYPE>
	int RevisableTrajectory<POSITION_TYPE>::last_phase()
	{
		return timeslot->last_phase();
	}

	///<summary>
	/// id�Ɠ����ʒu���o�^����Ă���ŏ��̃t�F�[�Y��Ԃ�
	///</summary>
	template <typename POSITION_TYPE>
	int RevisableTrajectory<POSITION_TYPE>::search_phase_same_id(Graph::MapNodeIndicator& id)
	{
		for (auto iter = visited_node_ids->begin(); iter != visited_node_ids->end(); iter++) {
			if ((*iter).id() == id.id()) return std::distance(visited_node_ids->begin(),iter);
		}
	}


	///<summary>
	/// positions��visited_node_ids_��venu_name�ɁCinsert_num�������C��~phase��}������D
	///</summary>
	template <typename POSITION_TYPE>
	void RevisableTrajectory<POSITION_TYPE>::insert_positions_to_trajectory(int phase_id, int insert_num)
	{
		positions->insert(positions->begin()+ phase_id, insert_num, positions->at(phase_id));
		visited_node_ids->insert(visited_node_ids->begin() + phase_id, insert_num, visited_node_ids->at(phase_id));
		venue_names->insert(venue_names->begin() + phase_id, insert_num, venue_names->at(phase_id));
	}


	///<summary>
	/// positions�ɁCdelete_num�������C��~phase��}������D
	/// delete_num�������C�������s���D
	///</summary>
	template <typename POSITION_TYPE>
	void RevisableTrajectory<POSITION_TYPE>::delete_positions_to_trajectory(int phase_id, int delete_num) 
	{
		//positions->erase(positions->begin() + phase_id, delete_num, positions->at(phase_id));
		//visited_node_ids->erase(visited_node_ids->begin() + phase_id, delete_num, visited_node_ids->at(phase_id));
		//venue_names->erase(venue_names->begin() + phase_id, delete_num, venue_names->at(phase_id));
	}

	///<summary>
	/// �g���W�F�N�g���̃R�s�[���s��
	/// target_start_phase����copy_start_phase~copy_end_phase���Ccopied_trajectory���R�s�[������
	/// ���̌�Ctarget_end_phase�ȍ~��vector��A��������
	///</summary>
	template <typename POSITION_TYPE>
	void RevisableTrajectory<POSITION_TYPE>::copy_trajectory(int target_start_phase, int target_end_phase, int copy_start_phase, int copy_end_phase, RevisableTrajectory<POSITION_TYPE>& original_trajectory, RevisableTrajectory<POSITION_TYPE>& copied_trajectory)
	{		
		//��U�N���A
		//while (!positions->empty() != target_start_phase + 1) positions->pop_back();
		//while (visited_node_ids->size() != target_start_phase + 1) visited_node_ids->pop_back();
		//while (venue_names->size() != target_start_phase + 1) venue_names->pop_back();
		/*
		//�ύX����˂�����
		positions->insert(positions->end(), copied_trajectory.get_positions()->begin() + copy_start_phase, copied_trajectory.get_positions()->begin() + copy_end_phase);
		visited_node_ids->push_back(visited_node_ids->end(), copied_trajectory.get_visited_node_ids()->begin() + copy_start_phase, copied_trajectory.get_visited_node_ids()->begin() + copy_end_phase);
		venue_names->push_back(venue_names->end(), copied_trajectory.get_venue_names()->begin() + copy_start_phase, copied_trajectory.get_venue_names()->begin() + copy_end_phase);
			
		//�ύX����Ă��Ȃ�����˂�����
		positions->insert(positions->end(), original_trajectory.get_positions()->prev_positions->begin() + target_end_phase, original_trajectory.get_positions()->prev_positions->end());
		prev_visited_node_ids->insert(visited_node_ids->end(), original_trajectory.get_visited_node_ids()prev_visited_node_ids->begin() + target_end_phase, original_trajectory->get_visited_node_ids()->prev_visited_node_ids->end());
		prev_venue_names->insert(venue_names->end(), original_trajectory.get_venue_names()->begin() + target_end_phase, original_trajectory.get_venue_names()->end());
		*/
	}

	///<summary>
	/// phase_id��node-id�����߂�
	///</summary>
	template <typename POSITION_TYPE>
	Graph::MapNodeIndicator RevisableTrajectory<POSITION_TYPE>::get_visited_node_id(int phase_id)
	{
		return visited_node_ids->at(phase_id);
	}

	///<summary>
	/// �K��POI��ID���X�g��Ԃ�
	///</summary>
	template <typename POSITION_TYPE>
	std::vector<Graph::MapNodeIndicator> RevisableTrajectory<POSITION_TYPE>::read_visited_node_ids()
	{
		return *visited_node_ids;
	}

	///<summary>
	/// �K��POI��ID���X�g��Ԃ�
	///</summary>
	template <typename POSITION_TYPE>
	void RevisableTrajectory<POSITION_TYPE>::set_positions(std::shared_ptr<std::vector<std::shared_ptr<POSITION_TYPE const>>> copy_positions)
	{
		positions = copy_positions;
	}
}