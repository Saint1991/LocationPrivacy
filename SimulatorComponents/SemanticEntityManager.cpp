#include "stdafx.h"
#include "SemanticEntityManager.h"

namespace Entity
{

	///<summary>
	/// コンストラクタ
	///</summary>
	template <typename POSITION_TYPE>
	SemanticEntityManager<POSITION_TYPE>::SemanticEntityManager(std::shared_ptr<User::BasicUser<POSITION_TYPE>> user, int num_of_dummy, std::shared_ptr<Time::TimeSlotManager> timeslot) 
		: EntityManager<Dummy<POSITION_TYPE>, User::BasicUser<POSITION_TYPE>, POSITION_TYPE>(user, num_of_dummy, timeslot)
	{
	}


	///<summary>
	/// デストラクタ
	///</summary>
	template <typename POSITION_TYPE>
	SemanticEntityManager<POSITION_TYPE>::~SemanticEntityManager()
	{
	}


	///<summary>
	/// 指定したPhaseのカテゴリを設定する
	/// crossing_point設定かつカテゴリを設定したい場合はset_crossing_point_atとこれも呼ぶ必要がある
	/// crossing_point以外の場合はset_state系を使った方が効率がよい
	///</summary>
	template <typename POSITION_TYPE>
	void SemanticEntityManager<POSITION_TYPE>::set_category_of_phase(entity_id id, const category_id& category, int phase)
	{
		if (id < 1 || dummies->size() <= id) {
			throw std::invalid_argument("INVALID dummy ID");
			return;
		}

		std::shared_ptr<Dummy<POSITION_TYPE>> dummy = dummies->at(id - 1);
		if (dummy != nullptr && dummy->get_id() == id) {
			dummy->set_category_of_phase(phase, category);
		}
	}


	///<summary>
	/// 指定した時刻のカテゴリを設定する
	/// crossing_point設定かつカテゴリを設定したい場合はset_crossing_point_atとこれも呼ぶ必要がある
	/// crossing_point以外の場合はset_state系を使った方が効率がよい
	///</summary>
	template <typename POSITION_TYPE>
	void SemanticEntityManager<POSITION_TYPE>::set_category_at(entity_id id, const category_id& category, time_t time)
	{
		int phase = timeslot->find_phase_of_time(time);
		if (phase != INVALID) {
			set_category_of_phase(id, category, phase);
		}
	}


	///<summary>
	/// 指定したphaseにおける位置とカテゴリを設定する
	///</summary>
	template <typename POSITION_TYPE>
	void SemanticEntityManager<POSITION_TYPE>::set_state_of_phase(entity_id id, const POSITION_TYPE& position, const category_id& category, int phase)
	{
		if (id < 1 || dummies->size() <= id) {
			throw std::invalid_argument("INVALID dummy ID");
			return;
		}

		std::shared_ptr<Dummy<POSITION_TYPE>> dummy = dummies->at(id - 1);
		if (dummy != nullptr && dummy->get_id() == id) {
			dummy->set_position_of_phase(phase, position);
			dummy->set_category_of_phase(phase, category);
		}
	}

	///<summary>
	/// 指定した時刻における位置とカテゴリを設定する
	///</summary>
	template <typename POSITION_TYPE>
	void SemanticEntityManager<POSITION_TYPE>::set_state_at(entity_id id, const POSITION_TYPE& position, const category_id& category, time_t time)
	{
		int phase = timeslot->find_phase_of_time(time);
		if (phase != INVALID) {
			set_state_of_phase(id, position, category, time);
		}
	}
}

