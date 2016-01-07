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
	/// positions�ɁCinsert_num�������C��~phase��}������D
	/// insert_num�������C�폜���s���D
	///</summary>
	template <typename POSITION_TYPE>
	void RevisableTrajectory<POSITION_TYPE>::insert_positions_to_trajectory(int phase_id, int insert_num)
	{
		for (int i = 0; i < insert_num; i++) positions->pop_back();
		positions->insert(positions->begin()+ phase_id, insert_num, positions->at(phase_id));
	}


	///<summary>
	/// positions�ɁCdelete_num�������C��~phase��}������D
	/// delete_num�������C�������s���D
	///</summary>
	template <typename POSITION_TYPE>
	void RevisableTrajectory<POSITION_TYPE>::delete_positions_to_trajectory(int phase_id, int delete_num) 
	{
		positions->erase(positions->begin() + phase_id, positions->begin() + phase_id + delete_num);
		//�Ƃ肠��������́C�Ō�̗v�f���R�s�[
		for (int i = 0; i < delete_num; i++) positions->push_back(positions->back());
	}

	///<summary>
	/// �g���W�F�N�g���̃R�s�[���s��
	///</summary>
	template <typename POSITION_TYPE>
	void RevisableTrajectory<POSITION_TYPE>::copy_trajectory(int start_phase, RevisableTrajectory<POSITION_TYPE>& copied_trajectory)
	{
		if (start_phase == (this->phase_count() - 1)) return;
		start_phase++;
		std::copy(copied_trajectory.get_positions()->begin() + start_phase, copied_trajectory.get_positions()->end(), this->positions->begin() + start_phase);
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
}