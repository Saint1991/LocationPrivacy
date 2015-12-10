#include "stdafx.h"
#include "PauseMobileEntity.h"


namespace Entity
{
	
	///<summary>
	/// �R���X�g���N�^
	///</summary>
	VisitedPoiInfo::VisitedPoiInfo()
	{
	}

	///<summary>
	/// �f�X�g���N�^
	///</summary>
	VisitedPoiInfo::~VisitedPoiInfo()
	{
	}

	///<summary>
	/// �R���X�g���N�^
	///</summary>
	template <typename POSITION_TYPE, typename TRAJECTORY_TYPE>
	PauseMobileEntity<POSITION_TYPE, TRAJECTORY_TYPE>::PauseMobileEntity(entity_id id, std::shared_ptr<Time::TimeSlotManager const> timeslot)
		: MobileEntity<POSITION_TYPE, TRAJECTORY_TYPE>(id, timeslot), 
		  pause_time_list(std::vector<double>(timeslot->phase_count(),0)), 
		  speed_list(std::vector<double>(timeslot->phase_count(), 0))
	{
	}

	///<summary>
	/// �f�X�g���N�^
	///</summary>
	template <typename POSITION_TYPE, typename TRAJECTORY_TYPE>
	PauseMobileEntity<POSITION_TYPE, TRAJECTORY_TYPE>::~PauseMobileEntity()
	{
	}


	///<summary>
	/// ��~���Ԃ����߂�
	///</summary>
	template <typename POSITION_TYPE, typename TRAJECTORY_TYPE>
	int PauseMobileEntity<POSITION_TYPE, TRAJECTORY_TYPE>::get_pause_time(int phase) const
	{
		return pause_time_list.at(phase);
	}

	///<summary>
	/// ����̒l��int�^�Œ�~���ԂƂ���set����
	///</summary>
	template <typename POSITION_TYPE, typename TRAJECTORY_TYPE>
	void PauseMobileEntity<POSITION_TYPE, TRAJECTORY_TYPE>::set_pause_time(int phase, int pause_time)
	{
		pause_time_list.at(phase) = pause_time;
		//(visited_poi_info.at(visited_poi_id)).pause_phase.push_back(phase);
	}

	///<summary>
	/// ����̒l��double�^�Œ�~���ԂƂ���set����
	///</summary>
	template <typename POSITION_TYPE, typename TRAJECTORY_TYPE>
	void PauseMobileEntity<POSITION_TYPE, TRAJECTORY_TYPE>::set_pause_time(int phase, double pause_time)
	{
		pause_time_list.at(phase) = pause_time;
		//(visited_poi_info.at(visited_poi_id)).pause_phase.push_back(phase);
	}

	
	///<summary>
	/// �����_���Ȓl��int�^�Œ�~���ԂƂ���set����
	///</summary>
	template <typename POSITION_TYPE, typename TRAJECTORY_TYPE>
	void PauseMobileEntity<POSITION_TYPE, TRAJECTORY_TYPE>::set_random_pause_time(int phase, int min, int max)
	{
		Math::Probability generator;
		int pause_time = generator.uniform_distribution(min, max);
		
		pause_time_list.at(phase) = pause_time;
	}

	///<summary>
	/// �����_���Ȓl��double�^�Œ�~���ԂƂ���set����
	///</summary>
	template <typename POSITION_TYPE, typename TRAJECTORY_TYPE>
	void PauseMobileEntity<POSITION_TYPE, TRAJECTORY_TYPE>::set_random_pause_time(int phase, double min, double max)
	{
		Math::Probability generator;
		double pause_time = generator.uniform_distribution(min, max);

		pause_time_list.at(phase) = pause_time;
	}


	///<summary>
	/// ���ɖK��\��̒�~POI�̓�������phase�����߂�D
	/// ������~���̏ꍇ�͌��݂�phase��Ԃ��D
	///</summary>
	template <typename POSITION_TYPE, typename TRAJECTORY_TYPE>
	int PauseMobileEntity<POSITION_TYPE, TRAJECTORY_TYPE>::get_pause_phase(int phase)
	{
		//��~���̏ꍇ�́C����phase��Ԃ��C�ړ����̏ꍇ�́C�������Ă���K��n�_�̗\�肵�Ă��铞�����Ԃɒl����phase��Ԃ�
		return pause_flag == 1 ? phase : visited_pois_info_list.at(visited_pois_info_list_id + 1).pause_phase.front();
	}


	///<summary>
	/// �ړ����x�����߂�
	///</summary>
	template <typename POSITION_TYPE, typename TRAJECTORY_TYPE>
	double PauseMobileEntity<POSITION_TYPE, TRAJECTORY_TYPE>::get_speed(int phase) const
	{
		return speed_list.at(phase);
	}

	///<summary>
	/// ����̒l�̈ړ����x��set����
	///</summary>
	template <typename POSITION_TYPE, typename TRAJECTORY_TYPE>
	void PauseMobileEntity<POSITION_TYPE, TRAJECTORY_TYPE>::set_speed(int phase, double speed)
	{
		speed_list.at(phase) = speed;
	}

	

	///<summary>
	/// �����_���Ȓl�̈ړ����x��set����
	///</summary>
	template <typename POSITION_TYPE, typename TRAJECTORY_TYPE>
	void PauseMobileEntity<POSITION_TYPE, TRAJECTORY_TYPE>::set_random_speed(int phase, double average_speed, double speed_range)
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
	template <typename POSITION_TYPE, typename TRAJECTORY_TYPE>
	std::vector<int> PauseMobileEntity<POSITION_TYPE, TRAJECTORY_TYPE>::find_cross_not_set_phases_of_poi() const
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
	template <typename POSITION_TYPE, typename TRAJECTORY_TYPE>
	int PauseMobileEntity<POSITION_TYPE, TRAJECTORY_TYPE>::randomly_pick_cross_not_set_phase_of_poi() const
	{
		Math::Probability generator;
		std::vector<int> not_set_phases = find_cross_not_set_phases_of_poi();
		if (not_set_phases.size() == 0) return INVALID;
		return not_set_phases.at(generator.uniform_distribution(0, not_set_phases.size() - 1));
	}

	///<summary>
	/// POI���o�����̑��x���擾����
	///</summary>
	template <typename POSITION_TYPE, typename TRAJECTORY_TYPE>
	double PauseMobileEntity<POSITION_TYPE, TRAJECTORY_TYPE>::get_starting_speed(int visited_poi_id)
	{
		return visited_pois_info_list.at(visited_poi_id).starting_speed;
	}
	
	
	
	///<summary>
	/// �K��POI�����Z�b�g����
	/// 
	///</summary>
	template <typename POSITION_TYPE, typename TRAJECTORY_TYPE>
	void PauseMobileEntity<POSITION_TYPE, TRAJECTORY_TYPE>::set_visited_poi_of_phase(int phase, const Graph::MapNodeIndicator& node_id, const Geography::LatLng& position)
	{
		trajectory->set_position_of_phase(phase, node_id, position);
		visited_pois_info_list.at(visited_pois_info_list_id).visited_poi = std::make_pair(node_id, position);
		visited_pois_info_list.at(visited_pois_info_list_id).arrive_phase = phase;
	}
	

	
	///<summary>
	/// �g���W�F�N�g���f�[�^��ύX�\�ȏ�ԂŎ擾����
	///</summary>
	template <typename POSITION_TYPE, typename TRAJECTORY_TYPE>
	std::shared_ptr<TRAJECTORY_TYPE> PauseMobileEntity<POSITION_TYPE, TRAJECTORY_TYPE>::get_trajectory()
	{
		return trajectory;
	}
	
	///<summary>
	/// ��~���Ă��邩�ǂ����m�F����
	///</summary>
	template <typename POSITION_TYPE, typename TRAJECTORY_TYPE>
	bool PauseMobileEntity<POSITION_TYPE, TRAJECTORY_TYPE>::check_pause_flag()
	{
		return pause_flag == 1 ? true : false;
	}


}
