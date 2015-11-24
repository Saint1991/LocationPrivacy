#include "stdafx.h"
#include "Dummy.h"

namespace Entity
{


	///<summary>
	/// コンストラクタ
	///</summary>
	template <typename POSITION_TYPE>
	Dummy<POSITION_TYPE>::Dummy(entity_id id, std::shared_ptr<Time::TimeSlotManager const> timeslot) 
		: MobileEntity<POSITION_TYPE, Graph::SemanticTrajectory<POSITION_TYPE>>(id, timeslot)
	{
	}


	///<summary>
	/// コンストラクタ
	///</summary>
	template <typename POSITION_TYPE>
	Dummy<POSITION_TYPE>::Dummy(entity_id id, std::shared_ptr<Graph::SemanticTrajectory<POSITION_TYPE>> trajectory)
		: MobileEntity<POSITION_TYPE, Graph::SemanticTrajectory<POSITION_TYPE>>(id, trajectory)
	{

	}

	///<summary>
	/// デストラクタ
	///</summary>
	template <typename POSITION_TYPE>
	Dummy<POSITION_TYPE>::~Dummy()
	{
	}


	///<summary>
	/// 指定したPhaseに位置とカテゴリを設定する
	///</summary>
	template <typename POSITION_TYPE>
	void Dummy<POSITION_TYPE>::set_state_of_phase(int phase, const Graph::MapNodeIndicator& node_id, const POSITION_TYPE& position, const category_id& category)
	{
		set_position_of_phase(phase, node_id, position);
		trajectory->set_category_of_phase(phase, category);
	}


	///<summary>
	/// 指定した時刻に位置とカテゴリを設定する
	///</summary>
	template <typename POSITION_TYPE>
	void Dummy<POSITION_TYPE>::set_state_at(time_t time, const Graph::MapNodeIndicator& node_id, const POSITION_TYPE& position, const category_id& category)
	{
		int phase = trajectory->find_phase_of_time(time);
		if (phase != INVALID) {
			set_state_of_phase(phase, node_id, position, category);
		}
	}


	///<summary>
	/// 指定したPhaseにおけるカテゴリを設定する
	///</summary>
	template <typename POSITION_TYPE>
	void Dummy<POSITION_TYPE>::set_category_of_phase(int phase, const category_id& category)
	{
		trajectory->set_category_of_phase(phase, category);
	}


	///<summary>
	/// 時刻tにおけるカテゴリを設定する
	///</summary>
	template <typename POSITION_TYPE>
	void Dummy<POSITION_TYPE>::set_category_at(time_t time, const category_id& category)
	{
		int phase = trajectory->find_phase_of_time(time);
		if (phase != INVALID) {
			trajectory->set_category_of_phase(phase, category);
		}
	}

}

