#ifdef SIMULATORCOMPONENTS_EXPORTS
#define DUMMY_API __declspec(dllexport)
#else
#define DUMMY_API __declspec(dllimport)
#endif

#pragma once
#include "MobileEntity.h"
#include "Sequence.h"
#include "MapNodeIndicator.h"
#include "SemanticTrajectory.h"

namespace Entity
{

	typedef std::string category_id;

	///<summary>
	/// �_�~�[��\���N���X
	/// ����ɂ̓J�e�S����񂪕t�����Ă��邪�C�s�v�ȏꍇ�͂��̂܂�MobileEntity���g���Ă����΂悢
	///</summary>
	template <typename POSITION_TYPE = Geography::LatLng>
	class DUMMY_API Dummy : public MobileEntity<POSITION_TYPE, Graph::SemanticTrajectory<POSITION_TYPE>>
	{		
	public:
		Dummy(entity_id id, std::shared_ptr<Time::TimeSlotManager const> timeslot);
		Dummy(entity_id id, std::shared_ptr<Graph::SemanticTrajectory<POSITION_TYPE>> trajectory);
		virtual ~Dummy();

		void set_state_of_phase(int phase, const Graph::MapNodeIndicator& node_id, const POSITION_TYPE& position, const category_id& category);
		void set_state_at(time_t time, const Graph::MapNodeIndicator& node_id, const POSITION_TYPE& position, const category_id& category);

		void set_category_of_phase(int phase, const category_id& category);
		void set_category_at(time_t time, const category_id& category);
	};

	//�����I���ꉻ
	template class Dummy<Graph::Coordinate>;
	template class Dummy<Geography::LatLng>;
}


