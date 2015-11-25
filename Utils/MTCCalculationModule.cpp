#include "stdafx.h"
#include "MTCCalculationModule.h"

namespace Evaluation {

	///<summary>
	/// �R���X�g���N�^
	///</summary>
	template <typename MAP_TYPE, typename ENTITY_MANAGER, typename DUMMY, typename REQUIREMENT, typename POSITION_TYPE>
	MTCCalculationModule<MAP_TYPE, ENTITY_MANAGER, DUMMY, REQUIREMENT, POSITION_TYPE>::MTCCalculationModule(std::shared_ptr<MAP_TYPE const> map, std::shared_ptr<ENTITY_MANAGER> entities, std::shared_ptr<REQUIREMENT> requirement, std::shared_ptr<Time::TimeSlotManager> time_manager)
		: map(map), entities(entities), requirement(requirement), time_manager(time_manager), 
		user_probability(std::vector<double>(requirement->dummy_num + 1, 0.0)), entropy(0.0)
	{
		user_probability.at(0) = 1.0;//���[�U�̃��[�U�m����1�ɂ���
		//entities_id_list�̍쐬
		for (int i = 0; i <= requirement->dummy_num; i++) {
			entities_id_list.at(i) = i;
		}
	}
	

	///<summary>
	/// �f�X�g���N�^
	///</summary>
	template <typename MAP_TYPE, typename ENTITY_MANAGER, typename DUMMY, typename REQUIREMENT, typename POSITION_TYPE>
	MTCCalculationModule<MAP_TYPE, ENTITY_MANAGER, DUMMY, REQUIREMENT, POSITION_TYPE>::~MTCCalculationModule()
	{
	}


	///<summary>
	/// �Q�̃G���e�B�e�B���ړ���C���҂̈ړ��\�͈͂ɑ����邩�ǂ����𔻒�
	///</summary>
	template <typename MAP_TYPE, typename ENTITY_MANAGER, typename DUMMY, typename REQUIREMENT, typename POSITION_TYPE>
	bool MTCCalculationModule<MAP_TYPE, ENTITY_MANAGER, DUMMY, REQUIREMENT, POSITION_TYPE>::contains_in_moveable_range(std::shared_ptr<DUMMY>& mobile_entity1, std::shared_ptr<DUMMY>& mobile_entity2, std::shared_ptr<REQUIREMENT> requirement, int phase) const{
		return
			map->is_reacheable(mobile_entity1->read_node_pos_info_of_phase(phase-1).first, mobile_entity2->read_node_pos_info_of_phase(phase).first, mobile_entity1->get_speed(phase -1), requirement->SERVICE_INTERVAL)
			&& map->is_reachable(mobile_entity2->read_node_pos_info_of_phase(phase - 1).first, mobile_entity1->read_node_pos_info_of_phase(phase).first, mobile_entity2->get_speed(phase - 1), requirement->SERVICE_INTERVAL)
			? true : false;
	}

	///<summary>
	/// �Q�̃G���e�B�e�B�̌����p�x��30�����傫�����ǂ����𔻒�
	///</summary>
	template <typename MAP_TYPE, typename ENTITY_MANAGER, typename DUMMY, typename REQUIREMENT, typename POSITION_TYPE>
	bool MTCCalculationModule<MAP_TYPE, ENTITY_MANAGER, DUMMY, REQUIREMENT, POSITION_TYPE>::check_angle_greater_than_30_degrees(std::shared_ptr<DUMMY>& mobile_entity1, std::shared_ptr<DUMMY>& mobile_entity2, int phase) const {
		return true;
	}

	///<summary>
	/// ���������������ۂ̃��[�U�m�����v�Z����
	///</summary>
	template <typename MAP_TYPE, typename ENTITY_MANAGER, typename DUMMY, typename REQUIREMENT, typename POSITION_TYPE>
	void MTCCalculationModule<MAP_TYPE, ENTITY_MANAGER, DUMMY, REQUIREMENT, POSITION_TYPE>::calculate_user_probability(int entity_id1, int entity_id2) {
		user_probability.at(entity_id1) = (user_probability.at(entity_id1) + user_probability.at(entity_id2)) / 2;
		user_probability.at(entity_id2) = user_probability.at(entity_id1);
	}
	 

	///<summary>
	/// entropy���v�Z
	///</summary>
	template <typename MAP_TYPE, typename ENTITY_MANAGER, typename DUMMY, typename REQUIREMENT, typename POSITION_TYPE>
	double MTCCalculationModule<MAP_TYPE, ENTITY_MANAGER, DUMMY, REQUIREMENT, POSITION_TYPE>::calculate_entropy(std::vector<double> user_probability) const {
		for (int i = 0; i < user_probability.size(); i++) entropy += -user_probability.at(i) * log(i);
		return entropy;
	}

	///<summary>
	/// MTC�̌v�Z (LatLng)
	/// phase�̒��O�ƒ��������̂ŁCstart�͂P����Cend�͍Ō�̂Q�O����ł��邱�Ƃɒ���
	///</summary>
	template <typename MAP_TYPE, typename ENTITY_MANAGER, typename DUMMY, typename REQUIREMENT, typename POSITION_TYPE>
	double MTCCalculationModule<MAP_TYPE, ENTITY_MANAGER, DUMMY, REQUIREMENT, POSITION_TYPE>::calculate_MTC(std::shared_ptr<MAP_TYPE const> map, std::shared_ptr<ENTITY_MANAGER> entities, std::shared_ptr<REQUIREMENT> requirement, std::shared_ptr<Time::TimeSlotManager> time_manager, int start_phase, int end_phase) const
	{
		for (int phase_id = start_phase; phase_id <= end_phase; phase_id++) {
			int iter1 = 0;
			int iter2 = 1;
			//BasicUser��Dummy���p�����Ă���̂Ń|�C���^����͉\
			for (std::shared_ptr<DUMMY> target1 = iter1 == 0 ? entities->get_user() : entities->get_dummy_by_id(iter1); iter1 < requirement->dummy_num; iter1++)
			{
				for (std::shared_ptr<DUMMY> target2 = entities->get_dummy_by_id(iter1 + 1); iter2 <= requirement->dummy_num; iter2++) {
					if (contains_in_moveable_range(target1, target2, requirement, phase_id)
						&& check_angle_greater_than_30_degrees(target1, target2, phase_id)) calculate_user_probability();
				}	
			}
			if (calculate_entropy(user_probability) >= 1) MTC.push_back(phase_id * requirement->SERVICE_INTERVAL);
		}

		return std::accumulate(MTC.begin(),MTC.end(),0.0)/ MTC.size();
	}

}
