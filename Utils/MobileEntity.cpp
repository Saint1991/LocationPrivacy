#include "stdafx.h"
#include "MobileEntity.h"

namespace Entity
{

	///<summary>
	/// �R���X�g���N�^
	///</summary>
	template <typename POSITION_TYPE, typename TRAJECTORY_TYPE>
	MobileEntity<POSITION_TYPE, TRAJECTORY_TYPE>::MobileEntity(entity_id id, std::shared_ptr<Time::TimeSlotManager const> timeslot)
		: Identifiable<entity_id>(id), trajectory(std::make_shared<TRAJECTORY_TYPE>(timeslot)),
		cross_flg(std::make_shared<std::vector<bool>>(timeslot->phase_count(), false)), total_cross_count(0)
	{
	}


	///<summary>
	/// �R���X�g���N�^
	///</summary>
	template <typename POSITION_TYPE, typename TRAJECTORY_TYPE>
	MobileEntity<POSITION_TYPE, TRAJECTORY_TYPE>::MobileEntity(entity_id id, std::shared_ptr<TRAJECTORY_TYPE> trajectory)
		: Identifiable<entity_id>(id), trajectory(trajectory)
	{

	}


	///<summary>
	/// �f�X�g���N�^
	///</summary>
	template <typename POSITION_TYPE, typename TRAJECTORY_TYPE>
	MobileEntity<POSITION_TYPE, TRAJECTORY_TYPE>::~MobileEntity()
	{
	}


	///<summary>
	/// �w�肵��Phase�ɂ�����ʒu��ݒ肷��
	///</summary>
	template <typename POSITION_TYPE, typename TRAJECTORY_TYPE>
	void MobileEntity<POSITION_TYPE, TRAJECTORY_TYPE>::set_position_of_phase(int phase, const Graph::MapNodeIndicator& node_id, const POSITION_TYPE& position)
	{
		trajectory->set_position_of_phase(phase, node_id, position);
	}

	///<summary>
	/// ����t�ɂ�����ʒu��ݒ肷��
	///</summary>
	template <typename POSITION_TYPE, typename TRAJECTORY_TYPE>
	void MobileEntity<POSITION_TYPE, TRAJECTORY_TYPE>::set_position_at(time_t time, const Graph::MapNodeIndicator& node_id, const POSITION_TYPE& position)
	{
		trajectory->set_position_at(time, node_id, position);
	}


	///<summary>
	/// ���L�n�_�̐ݒ������
	///</summary>
	template <typename POSITION_TYPE, typename TRAJECTORY_TYPE>
	void MobileEntity<POSITION_TYPE, TRAJECTORY_TYPE>::set_crossing_position_of_phase(int phase, const Graph::MapNodeIndicator& node_id, POSITION_TYPE position)
	{
		register_as_cross_position(phase);
		set_position_of_phase(phase, node_id, position);
	}

	///<summary>
	/// ���L�n�_�̐ݒ������
	///</summary>
	template <typename POSITION_TYPE, typename TRAJECTORY_TYPE>
	void MobileEntity<POSITION_TYPE, TRAJECTORY_TYPE>::set_crossing_position_at(time_t time, const Graph::MapNodeIndicator& node_id, POSITION_TYPE position)
	{
		int phase = trajectory->find_phase_of_time(time);
		set_crossing_position_of_phase(phase, node_id, position);
	}


	///<summary>
	/// phase�ɖK�₵���n�_�������n�_�Ƃ��Đݒ肵�C�����񐔂��C���N�������g���܂�
	///</summary>
	template <typename POSITION_TYPE, typename TRAJECTORY_TYPE>
	void MobileEntity<POSITION_TYPE, TRAJECTORY_TYPE>::register_as_cross_position(int phase)
	{
		cross_flg->at(phase) = true;
		total_cross_count++;
	}

	///<summary>
	/// ���v�����񐔂��擾����
	///</summary>
	template <typename POSITION_TYPE, typename TRAJECTORY_TYPE>
	int MobileEntity<POSITION_TYPE, TRAJECTORY_TYPE>::get_cross_count() const
	{
		return total_cross_count;
	}


	///<summary>
	/// �w�莞���Ɍ������ݒ�ς��ǂ����`�F�b�N����
	///</summary>
	template <typename POSITION_TYPE, typename TRAJECTORY_TYPE>
	bool MobileEntity<POSITION_TYPE, TRAJECTORY_TYPE>::is_cross_set_at_phase(int phase) const
	{
		return cross_flg->at(phase);
	}


	///<summary>
	/// �w�莞���Ɍ������ݒ�ς��ǂ����`�F�b�N����
	///</summary>
	template <typename POSITION_TYPE, typename TRAJECTORY_TYPE>
	bool MobileEntity<POSITION_TYPE, TRAJECTORY_TYPE>::is_cross_set_at(time_t time) const
	{
		int phase = trajectory->find_phase_of_time(time);
		return cross_flg->at(phase);
	}


	///<summary>
	/// �������ݒ肳��Ă��Ȃ�Phase��S�Ď擾����
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
	/// �������ݒ肳��Ă��Ȃ�������������_���Ɏ擾����
	/// �ݒ肳��Ă��Ȃ�phase�����݂��Ȃ��ꍇ��INVALID��Ԃ�
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
	/// �w�肵��phase���O�̊m�肵�Ă���|�C���g�̓��B�����ƍ��W���擾���܂�
	/// ����ς݂̓_���Ȃ��ꍇ��Phase��INVALID�̒l��Ԃ��܂�
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
	/// �w�肵��phase����̊m�肵�Ă���|�C���g�̓��B�����ƍ��W���擾���܂�
	/// ����ς݂̓_���Ȃ��ꍇ��Phase��INVALID�̒l��Ԃ��܂�
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
	/// phase���̈ʒu���w�肵�܂�
	/// ���݂��Ȃ��ꍇ��nullptr���Ԃ�܂�
	///</summary>
	template <typename POSITION_TYPE, typename TRAJECTORY_TYPE>
	std::shared_ptr<POSITION_TYPE const> MobileEntity<POSITION_TYPE, TRAJECTORY_TYPE>::read_position_of_phase(int phase) const
	{
		return trajectory->position_of_phase(phase);
	}


	///<summary>
	/// �w�肵�������ɂ�����ʒu��Ԃ�
	/// �Y������G���g�����Ȃ��ꍇ��nullptr��Ԃ�
	///</summary>
	template <typename POSITION_TYPE, typename TRAJECTORY_TYPE>
	std::shared_ptr<POSITION_TYPE const> MobileEntity<POSITION_TYPE, TRAJECTORY_TYPE>::read_position_at(time_t time) const
	{
		return trajectory->position_at(time);
	}


	///<summary>
	/// �w�肵��Phase�ɑ��݂����m�[�h��ID�ƈʒu���擾���܂�
	///</summary>
	template <typename POSITION_TYPE, typename TRAJECTORY_TYPE>
	std::pair<Graph::MapNodeIndicator, std::shared_ptr<POSITION_TYPE const>> MobileEntity<POSITION_TYPE, TRAJECTORY_TYPE>::read_node_pos_info_of_phase(int phase) const
	{
		return trajectory->read_node_pos_info_of_phase(phase);
	}

	///<summary>
	/// �w�肵��Phase�ɑ��݂����m�[�h��ID�ƈʒu���擾���܂�
	///</summary>
	template <typename POSITION_TYPE, typename TRAJECTORY_TYPE>
	std::pair<Graph::MapNodeIndicator, std::shared_ptr<POSITION_TYPE const>> MobileEntity<POSITION_TYPE, TRAJECTORY_TYPE>::read_node_pos_info_at(time_t time) const
	{
		return trajectory->read_node_pos_info_at(time);
	}


	///<summary>
	/// �g���W�F�N�g���f�[�^���擾����
	///</summary>
	template <typename POSITION_TYPE, typename TRAJECTORY_TYPE>
	std::shared_ptr<TRAJECTORY_TYPE const> MobileEntity<POSITION_TYPE, TRAJECTORY_TYPE>::read_trajectory() const
	{
		return trajectory;
	}
	
}

