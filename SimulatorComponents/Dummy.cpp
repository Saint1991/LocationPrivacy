#include "stdafx.h"
#include "Dummy.h"

namespace Entity
{


	///<summary>
	/// コンストラクタ
	///</summary>
	template <typename POSITION_TYPE>
	Dummy<POSITION_TYPE>::Dummy(entity_id id, std::shared_ptr<Time::TimeSlotManager const> timeslot) 
		: MobileEntity<POSITION_TYPE>(id, timeslot), category_sequence(std::make_shared<Collection::Sequence<category_id>>(timeslot->phase_count()))
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
	void Dummy<POSITION_TYPE>::set_state_of_phase(int phase, POSITION_TYPE position, const category_id& category)
	{
		set_position_of_phase(phase, position);
		category_sequence->at(phase) = category;
	}


	///<summary>
	/// 指定した時刻に位置とカテゴリを設定する
	///</summary>
	template <typename POSITION_TYPE>
	void Dummy<POSITION_TYPE>::set_state_at(time_t time, POSITION_TYPE position, const category_id& category)
	{
		int phase = timeslot->find_phase_of_time(time);
		set_state_of_phase(phase, position, category);
	}


	///<summary>
	/// 指定したPhaseにおける状態を取得
	///</summary>
	template <typename POSITION_TYPE>
	DummyState<POSITION_TYPE> Dummy<POSITION_TYPE>::read_state_of_phase(int phase) const
	{
		DummyState<POSITION_TYPE> ret(timeslot->time_of_phase(phase), *read_position_of_phase(phase), category_sequence->at(phase));
		return ret;
	}

	///<summary>
	/// 指定した時刻における状態を取得
	///</summary>
	template <typename POSITION_TYPE>
	DummyState<POSITION_TYPE> Dummy<POSITION_TYPE>::read_state_at(time_t time) const
	{
		int phase = timeslot->find_phase_of_time(time);
		return read_state_of_phase(phase);
	}

	///<summary>
	/// TimeSlotの前から順に各状態についてexecute_functionを実行する
	/// 引数は時刻，位置のポインタ，カテゴリID
	///</summary>
	template <typename POSITION_TYPE>
	void Dummy<POSITION_TYPE>::for_each_state(const std::function<void(time_t, DummyState<POSITION_TYPE const>)>& execute_function) const
	{
		timeslot->for_each_time([&](time_t time, long duration, int phase) {
			std::shared_ptr<POSITION_TYPE const> position = read_position_of_phase(phase);
			category_id category = category_sequence->at(phase);
			execute_function(time, DummyState<POSITION_TYPE const>(time, *position, category));
		});
	}
}

