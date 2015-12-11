#ifdef SIMULATORCOMPONENTS_EXPORTS
#define REVISABLE_PAUSE_MOBILE_ENTITY_API __declspec(dllexport)
#else
#define REVISABLE_PAUSE_MOBILE_ENTITY_API __declspec(dllimport)
#endif


#pragma once
#include "PauseMobileEntity.h"

namespace Entity
{
	///<summary>
	/// 停止時間を含んだ移動体を表すクラス
	/// ユーザ，ダミーを表すのに用いるクラス (PauseMobileEntityから派生)
	/// 停止時間，速度の変更メソッドはこのクラスに書く．
	/// Trajectory(path,poi)の変更はRevisableTrajectoryクラスに書く
	///</summary>
	template <typename POSITION_TYPE = Geography::LatLng, typename TRAJECTORY_TYPE = Graph::RevisableTrajectory<POSITION_TYPE>>
	class REVISABLE_PAUSE_MOBILE_ENTITY_API RevisablePauseMobileEntity : public PauseMobileEntity<POSITION_TYPE, TRAJECTORY_TYPE>
	{
	public:
		RevisablePauseMobileEntity(entity_id id, std::shared_ptr<Time::TimeSlotManager const> timeslot);
		~RevisablePauseMobileEntity();

		void revise_rest_pause_time(int phase_id, double new_pause_time);
		void revise_pause_time();


	};

	//明示的特殊化
	template class RevisablePauseMobileEntity<Graph::Coordinate>;
	template class RevisablePauseMobileEntity<Geography::LatLng>;

}
