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
	/// ダミーを表すクラス
	/// これにはカテゴリ情報が付随しているが，不要な場合はそのままMobileEntityを使っておけばよい
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

	//明示的特殊化
	template class Dummy<Graph::Coordinate>;
	template class Dummy<Geography::LatLng>;
}


