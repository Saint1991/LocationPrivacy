#ifdef SIMULATORCOMPONENTS_EXPORTS
#define DUMMY_API __declspec(dllexport)
#else
#define DUMMY_API __declspec(dllimport)
#endif

#pragma once
#include "MobileEntity.h"
#include "Sequence.h"

namespace Entity
{

	typedef std::string category_id;

	///<summary>
	/// ダミーの状態データ
	///</summary>
	template <typename POSITION_TYPE>
	struct DUMMY_API DummyState
	{
		DummyState(time_t time, POSITION_TYPE position, category_id category) : time(time), position(position), category(category){}
		time_t time;
		POSITION_TYPE position;
		category_id category;
	};
	template struct DummyState<Geography::LatLng>;
	template struct DummyState<Graph::Coordinate>;

	///<summary>
	/// ダミーを表すクラス
	/// これにはカテゴリ情報が付随しているが，不要な場合はそのままMobileEntityを使っておけばよい
	///</summary>
	template <typename POSITION_TYPE = Geography::LatLng>
	class DUMMY_API Dummy : public MobileEntity<POSITION_TYPE>
	{	
	protected:
		std::shared_ptr<Collection::Sequence<category_id>> category_sequence;
		
	public:
		Dummy(entity_id id, std::shared_ptr<Time::TimeSlotManager const> timeslot);
		virtual ~Dummy();

		void set_state_of_phase(int phase, POSITION_TYPE position, const category_id& category);
		void set_state_at(time_t time, POSITION_TYPE position, const category_id& category);

		DummyState<POSITION_TYPE> read_state_of_phase(int phase) const;
		DummyState<POSITION_TYPE> read_state_at(time_t time) const;

		void for_each_state(const std::function<void(time_t, DummyState<POSITION_TYPE const>)>& execute_function) const;
	};


	//明示的特殊化
	template class Dummy<Graph::Coordinate>;
	template class Dummy<Geography::LatLng>;
}


