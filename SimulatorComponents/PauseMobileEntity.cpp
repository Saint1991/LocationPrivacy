#include "stdafx.h"
#include "PauseMobileEntity.h"


namespace Entity
{

	///<summary>
	/// �R���X�g���N�^
	///</summary>
	template <typename POSITION_TYPE>
	PauseMobileEntity<POSITION_TYPE>::PauseMobileEntity(entity_id id, std::shared_ptr<Time::TimeSlotManager const> timeslot)
		: MobileEntity<POSITION_TYPE>(id, timeslot), 
		  pause_time_list(std::vector<double>(timeslot->phase_count(),0)), 
		  speed_list(std::vector<double>(timeslot->phase_count(), 0))
	{
	}

	///<summary>
	/// �f�X�g���N�^
	///</summary>
	template <typename POSITION_TYPE>
	PauseMobileEntity<POSITION_TYPE>::~PauseMobileEntity()
	{
	}


	///<summary>
	/// ��~���Ԃ����߂�
	///</summary>
	template <typename POSITION_TYPE>
	int PauseMobileEntity<POSITION_TYPE>::get_pause_time(int phase) const
	{
		return pause_time_list.at(phase);
	}

	///<summary>
	/// ����̒l��int�^�Œ�~���ԂƂ���set����
	///</summary>
	template <typename POSITION_TYPE>
	void PauseMobileEntity<POSITION_TYPE>::set_pause_time(int phase, int pause_time)
	{
		pause_time_list.at(phase) = pause_time;
		//(visited_poi_info.at(visited_poi_id)).pause_phase.push_back(phase);
	}

	///<summary>
	/// ����̒l��double�^�Œ�~���ԂƂ���set����
	///</summary>
	template <typename POSITION_TYPE>
	void PauseMobileEntity<POSITION_TYPE>::set_pause_time(int phase, double pause_time)
	{
		pause_time_list.at(phase) = pause_time;
		//(visited_poi_info.at(visited_poi_id)).pause_phase.push_back(phase);
	}

	
	///<summary>
	/// �����_���Ȓl��int�^�Œ�~���ԂƂ���set����
	///</summary>
	template <typename POSITION_TYPE>
	void PauseMobileEntity<POSITION_TYPE>::set_random_pause_time(int phase, int min, int max)
	{
		Math::Probability generator;
		int pause_time = generator.uniform_distribution(min, max);
		
		pause_time_list.at(phase) = pause_time;
	}

	///<summary>
	/// �����_���Ȓl��double�^�Œ�~���ԂƂ���set����
	///</summary>
	template <typename POSITION_TYPE>
	void PauseMobileEntity<POSITION_TYPE>::set_random_pause_time(int phase, double min, double max)
	{
		Math::Probability generator;
		double pause_time = generator.uniform_distribution(min, max);

		pause_time_list.at(phase) = pause_time;
	}


	///<summary>
	/// �ړ����x�����߂�
	///</summary>
	template <typename POSITION_TYPE>
	double PauseMobileEntity<POSITION_TYPE>::get_speed(int phase) const
	{
		return speed_list.at(phase);
	}

	///<summary>
	/// ����̒l�̈ړ����x��set����
	///</summary>
	template <typename POSITION_TYPE>
	void PauseMobileEntity<POSITION_TYPE>::set_speed(int phase, double speed)
	{
		speed_list.at(phase) = speed;
	}

	

	///<summary>
	/// �����_���Ȓl�̈ړ����x��set����
	///</summary>
	template <typename POSITION_TYPE>
	void PauseMobileEntity<POSITION_TYPE>::set_random_speed(int phase, double average_speed, double speed_range)
	{
		Math::Probability generator;
		double min = average_speed -0.5* speed_range;
		double max = average_speed + 0.5* speed_range;
		double dummy_speed = generator.uniform_distribution(min, max);

		speed_list.at(phase) = dummy_speed;
	}

	///<summary>
	/// �������ݒ肳��Ă��Ȃ�Phase��S�Ď擾����
	///</summary>
	template <typename POSITION_TYPE>
	std::vector<int> PauseMobileEntity<POSITION_TYPE>::find_cross_not_set_phases_of_poi() const
	{
		std::vector<int> ret;
		for (int phase = 0; phase < cross_flg->size(); phase++) {
			node_pos_info info = trajectory->read_node_pos_info_of_phase(phase);
			if (!cross_flg->at(phase) && info.first.type() == Graph::NodeType::POI) ret.push_back(phase);
		}
		return ret;
	}


	///<summary>
	/// �������ݒ肳��Ă��Ȃ�������������_���Ɏ擾����
	/// �ݒ肳��Ă��Ȃ�phase�����݂��Ȃ��ꍇ��INVALID��Ԃ�
	///</summary>
	template <typename POSITION_TYPE>
	int PauseMobileEntity<POSITION_TYPE>::randomly_pick_cross_not_set_phase_of_poi() const
	{
		Math::Probability generator;
		std::vector<int> not_set_phases = find_cross_not_set_phases_of_poi();
		if (not_set_phases.size() == 0) return INVALID;
		return not_set_phases.at(generator.uniform_distribution(0, not_set_phases.size() - 1));
	}

	///<summary>
	/// visited_poi_list���擾����
	///</summary>
	template <typename POSITION_TYPE>
	std::vector<std::shared_ptr<Map::BasicPoi const>> PauseMobileEntity<POSITION_TYPE>::get_visited_poi_list()
	{
		return visited_poi_list;
	}

	///<summary>
	/// POI���Z�b�g����
	///</summary>
	template <typename POSITION_TYPE>
	void PauseMobileEntity<POSITION_TYPE>::set_visited_poi_list(std::shared_ptr<Map::BasicPoi const> visited_poi)
	{
		visited_poi_list.push_back(visited_poi);
	}

	/*
	///<summary>
	/// visited_poi_info���擾����
	///</summary>
	template <typename POSITION_TYPE>
	std::vector<PauseMobileEntity<>::VisitedPoiInfo> PauseMobileEntity<POSITION_TYPE>::get_visited_poi_info()
	{
		return visited_poi_info;
	}
	
	

	///<summary>
	/// POI���Z�b�g����
	///</summary>
	template <typename POSITION_TYPE>
	void PauseMobileEntity<POSITION_TYPE>::set_visit_POI_of_phase(int phase, std::shared_ptr<Map::BasicPoi const>& visited_poi)
	{

	}


	*/

	/*
	///<summary>
	/// �g���W�F�N�g���f�[�^��ύX�\�ȏ�ԂŎ擾����
	///</summary>
	template <typename POSITION_TYPE>
	std::shared_ptr<TRAJECTORY_TYPE> MobileEntity<POSITION_TYPE, TRAJECTORY_TYPE>::get_trajectory()
	{
		return trajectory;
	}
	*/
	

}
