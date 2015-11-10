#ifdef SIMULATORCOMPONENTS_EXPORTS
#define PAUSE_MOBILE_ENTITY_API __declspec(dllexport)
#else
#define PAUSE_MOBILE_ENTITY_API __declspec(dllimport)
#endif

#pragma once
#include "MobileEntity.h"
#include "KatoMethodRequirement.h"

namespace Entity
{

	typedef unsigned int entity_id;

	///<summary>
	/// 停止時間を含んだ移動体を表すクラス
	/// ユーザ，ダミーを表すのに用いるクラス (MobileEntityから派生)
	///</summary>
	template <typename POSITION_TYPE = Geography::LatLng>
	class PAUSE_MOBILE_ENTITY_API PauseMobileEntity : public MobileEntity<POSITION_TYPE>
	{
	protected:
		std::vector<int> pause_time_list;
		std::vector<double> speed_list;
		//Requirement::KatoMethodRequirement kato_requirement;
		//std::shared_ptr<Time::TimeSlotManager> revise_time_manager;//コンストラクタで初期化処理が必要


	public:
		PauseMobileEntity(entity_id id, std::shared_ptr<Time::TimeSlotManager const> timeslot);
		virtual ~PauseMobileEntity();

		int get_pause_time(int phase) const;
		void set_pause_time(int phase, time_t pause_time);
		void set_random_pause_time(int phase, int min, int max);

		double get_speed(int phase) const;
		void set_speed(int phase, double speed);
		void set_random_speed(int phase, double average_speed, double speed_range);

		int get_random_phase(int start, int end);

	};

	//明示的特殊化
	template class PauseMobileEntity<Graph::Coordinate>;
	template class PauseMobileEntity<Geography::LatLng>;
}