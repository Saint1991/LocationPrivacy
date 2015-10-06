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
	/// �_�~�[�̏�ԃf�[�^
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
	/// �_�~�[��\���N���X
	/// ����ɂ̓J�e�S����񂪕t�����Ă��邪�C�s�v�ȏꍇ�͂��̂܂�MobileEntity���g���Ă����΂悢
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


	//�����I���ꉻ
	template class Dummy<Graph::Coordinate>;
	template class Dummy<Geography::LatLng>;
}


