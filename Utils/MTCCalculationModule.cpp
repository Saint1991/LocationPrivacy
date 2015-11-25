#include "stdafx.h"
#include "MTCCalculationModule.h"

namespace Evaluation {

	///<summary>
	/// コンストラクタ
	///</summary>
	template <typename MAP_TYPE, typename ENTITY_MANAGER, typename DUMMY, typename REQUIREMENT, typename POSITION_TYPE>
	MTCCalculationModule<MAP_TYPE, ENTITY_MANAGER, DUMMY, REQUIREMENT, POSITION_TYPE>::MTCCalculationModule(std::shared_ptr<MAP_TYPE const> map, std::shared_ptr<ENTITY_MANAGER> entities, std::shared_ptr<REQUIREMENT> requirement, std::shared_ptr<Time::TimeSlotManager> time_manager)
		: map(map), entities(entities), requirement(requirement), time_manager(time_manager), 
		user_probability(std::vector<double>(requirement->dummy_num + 1, 0.0)), entropy(0.0)
	{
		user_probability.at(0) = 1.0;//ユーザのユーザ確率を1にする
	}
	

	///<summary>
	/// デストラクタ
	///</summary>
	template <typename MAP_TYPE, typename ENTITY_MANAGER, typename DUMMY, typename REQUIREMENT, typename POSITION_TYPE>
	MTCCalculationModule<MAP_TYPE, ENTITY_MANAGER, DUMMY, REQUIREMENT, POSITION_TYPE>::~MTCCalculationModule()
	{
	}


	///<summary>
	/// ２つのエンティティが移動後，両者の移動可能範囲に属するかどうかを判定
	///</summary>
	template <typename MAP_TYPE, typename ENTITY_MANAGER, typename DUMMY, typename REQUIREMENT, typename POSITION_TYPE>
	bool MTCCalculationModule<MAP_TYPE, ENTITY_MANAGER, DUMMY, REQUIREMENT, POSITION_TYPE>::contains_in_moveable_range(std::shared_ptr<DUMMY>& mobile_entity1, std::shared_ptr<DUMMY>& mobile_entity2, std::shared_ptr<REQUIREMENT> requirement, int phase) const{
		return
			map->is_reacheable(mobile_entity1->read_node_pos_info_of_phase(phase-1).first, mobile_entity2->read_node_pos_info_of_phase(phase).first, mobile_entity1->get_speed(phase -1), requirement->SERVICE_INTERVAL)
			&& map->is_reachable(mobile_entity2->read_node_pos_info_of_phase(phase - 1).first, mobile_entity1->read_node_pos_info_of_phase(phase).first, mobile_entity2->get_speed(phase - 1), requirement->SERVICE_INTERVAL)
			? true : false;
	}

	///<summary>
	/// ２つのエンティティの交差角度が30°より大きいかどうかを判定
	/// ２点が同じ場所の場合の対応は要確認
	///</summary>
	template <typename MAP_TYPE, typename ENTITY_MANAGER, typename DUMMY, typename REQUIREMENT, typename POSITION_TYPE>
	bool MTCCalculationModule<MAP_TYPE, ENTITY_MANAGER, DUMMY, REQUIREMENT, POSITION_TYPE>::check_angle_greater_than_30_degrees(std::shared_ptr<DUMMY>& mobile_entity1, std::shared_ptr<DUMMY>& mobile_entity2, int phase) const {
		double angle1_prev_and_now = Geography::GeoCalculation::lambert_azimuth_angle(mobile_entity1->find_previous_fixed_position(phase), mobile_entity1->read_position_of_phase(phase));
		double angle2_now_and_next = Geography::GeoCalculation::lambert_azimuth_angle(mobile_entity2->read_position_of_phase(phase), mobile_entity2->find_next_fixed_position(phase));
		
		double angle2_prev_and_now = Geography::GeoCalculation::lambert_azimuth_angle(mobile_entity2->find_previous_fixed_position(phase), mobile_entity2->read_position_of_phase(phase));
		double angle1_now_and_next = Geography::GeoCalculation::lambert_azimuth_angle(mobile_entity1->read_position_of_phase(phase), mobile_entity1->find_next_fixed_position(phase));

		double angle_1to2 = 
			angle1_prev_and_now != -1.0 && angle2_now_and_next != -1.0 ? std::abs(angle1_prev_and_now - angle2_now_and_next) : 0.0;
		double angle_2to1 = 
			angle2_prev_and_now != -1.0 && angle1_now_and_next != -1.0 ? std::abs(angle2_prev_and_now - angle1_now_and_next) : 0.0;
		
		//Piより小さく変換
		if (angle_1to2 > M_PI) angle_1to2 = std::abs(2 * M_PI - angle_1to2);
		if (angle_2to1 > M_PI) angle_2to1 = std::abs(2 * M_PI - angle_2to1);

		return angle_1to2 > M_PI/6 && angle_2to1 > M_PI / 6 ? true : false;
	}

	///<summary>
	/// 交差が発生した際のユーザ確率を計算する
	///</summary>
	template <typename MAP_TYPE, typename ENTITY_MANAGER, typename DUMMY, typename REQUIREMENT, typename POSITION_TYPE>
	void MTCCalculationModule<MAP_TYPE, ENTITY_MANAGER, DUMMY, REQUIREMENT, POSITION_TYPE>::calculate_user_probability(int entity_id1, int entity_id2) {
		user_probability.at(entity_id1) = (user_probability.at(entity_id1) + user_probability.at(entity_id2)) / 2;
		user_probability.at(entity_id2) = user_probability.at(entity_id1);
	}
	 

	///<summary>
	/// entropyを計算
	///</summary>
	template <typename MAP_TYPE, typename ENTITY_MANAGER, typename DUMMY, typename REQUIREMENT, typename POSITION_TYPE>
	double MTCCalculationModule<MAP_TYPE, ENTITY_MANAGER, DUMMY, REQUIREMENT, POSITION_TYPE>::calculate_entropy(std::vector<double> user_probability) const {
		for (int i = 0; i < user_probability.size(); i++) entropy += -user_probability.at(i) * log(i);
		return entropy;
	}

	///<summary>
	/// start_phaseからend_phaseのMTCの計算 (LatLng)
	/// phaseの直前と直後を見るので，startは１から，endは最後の２つ前からであることに注意
	///</summary>
	template <typename MAP_TYPE, typename ENTITY_MANAGER, typename DUMMY, typename REQUIREMENT, typename POSITION_TYPE>
	double MTCCalculationModule<MAP_TYPE, ENTITY_MANAGER, DUMMY, REQUIREMENT, POSITION_TYPE>::calculate_MTC_from_start_phase_to_end_phase(std::shared_ptr<MAP_TYPE const> map, std::shared_ptr<ENTITY_MANAGER> entities, std::shared_ptr<REQUIREMENT> requirement, std::shared_ptr<Time::TimeSlotManager> time_manager, int start_phase, int end_phase) const
	{
		for (int phase_id = start_phase; phase_id <= end_phase; phase_id++) {
			int iter1 = 0;
			int iter2 = 1;
			//BasicUserはDummyを継承しているのでポインタ代入は可能
			for (std::shared_ptr<DUMMY> target1 = iter1 == 0 ? entities->get_user() : entities->get_dummy_by_id(iter1); iter1 < requirement->dummy_num; iter1++)
			{
				for (std::shared_ptr<DUMMY> target2 = entities->get_dummy_by_id(iter1 + 1); iter2 <= requirement->dummy_num; iter2++) {
					if (contains_in_moveable_range(target1, target2, requirement, phase_id)
						&& check_angle_greater_than_30_degrees(target1, target2, phase_id)) calculate_user_probability();
				}	
			}
			if (calculate_entropy(user_probability) >= 1) MTC.push_back(phase_id * requirement->SERVICE_INTERVAL);
		}

		return std::accumulate(MTC.begin(), MTC.end(), 0.0) / MTC.size();
	}

	///<summary>
	/// 全phaseのMTCの計算 (LatLng)
	/// phaseの直前と直後を見るので，startは１から，endは最後の２つ前からであることに注意
	///</summary>
	template <typename MAP_TYPE, typename ENTITY_MANAGER, typename DUMMY, typename REQUIREMENT, typename POSITION_TYPE>
	double MTCCalculationModule<MAP_TYPE, ENTITY_MANAGER, DUMMY, REQUIREMENT, POSITION_TYPE>::calculate_MTC_at_all_phase(std::shared_ptr<MAP_TYPE const> map, std::shared_ptr<ENTITY_MANAGER> entities, std::shared_ptr<REQUIREMENT> requirement, std::shared_ptr<Time::TimeSlotManager> time_manager) const
	{
		return calculate_MTC_from_start_phase_to_end_phase(map, entities, requirement, 1, requirement->phase_count() - 2);
	}
}
