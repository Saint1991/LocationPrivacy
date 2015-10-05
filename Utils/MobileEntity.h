#ifdef UTILS_EXPORTS
#define MOBILE_ENTITY_API __declspec(dllexport)
#else
#define MOBILE_ENTITY_API __declspec(dllimport)
#endif

#pragma once
#include "Identifiable.h"
#include "Coordinate.h"
#include "LatLng.h"
#include "TimeSlotManager.h"

namespace Entity
{

	typedef unsigned int entity_id;

	///<summary>
	/// 移動体を表すクラス
	/// ユーザ，ダミーを表すのに用いるクラス (ここから派生して別個にUser, Dummyクラスを作ってもよい)
	///</summary>
	template <typename POSITION_TYPE = Geography::LatLng>
	class MOBILE_ENTITY_API MobileEntity : public Identifiable<entity_id>
	{
	
	protected:
		std::shared_ptr<Time::TimeSlotManager const> timeslot;
		std::shared_ptr<std::vector<std::shared_ptr<POSITION_TYPE>>> positions;
		std::shared_ptr<std::vector<bool>> cross_flg;
		int total_cross_count;
		
	public:
		MobileEntity(entity_id id, std::shared_ptr<Time::TimeSlotManager const> timeslot);
		virtual ~MobileEntity();

		void set_position_of_phase(int phase, POSITION_TYPE position);
		void set_position_at(time_t time, POSITION_TYPE position);

		void set_crossing_position_of_phase(int phase, POSITION_TYPE position);
		void set_crossing_position_at(time_t time, POSITION_TYPE position);

		int get_cross_count() const;
		bool is_cross_set_at_phase(int phase) const;
		bool is_cross_set_at(time_t time) const;
		
		const std::shared_ptr<POSITION_TYPE const> read_position_of_phase(int phase) const;
		const std::shared_ptr<POSITION_TYPE const> read_position_at(time_t time) const;
		
	};

	//明示的特殊化
	template class MobileEntity<Graph::Coordinate>;
	template class MobileEntity<Geography::LatLng>;
}



