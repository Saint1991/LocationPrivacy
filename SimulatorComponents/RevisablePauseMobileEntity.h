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

		//停止フェーズの修正
		void revise_pause_phases(int start_pause_phase, int num);
		void revise_pause_phases_of_any_visited_poi(int start_pause_phase, int num, int id);
		void add_pause_phases(int num, int phase);
		void remove_pause_phases(int num, int phase);

		//到着phaseの修正
		void revise_arrive_phase(int arrive_phase);
		void revise_arrive_phase_of_any_visited_poi(int arrive_phase, int id);
		void revise_all_arrive_phase();


		//停止時間の修正
		void revise_pause_time(double new_pause_time);
		void revise_pause_time_using_pause_phase(double new_pause_time, int phase);
		void revise_pause_time_of_any_visited_poi(double new_pause_time, int id);

		//現在の停止時間の修正
		void add_now_pause_time(int now_phase, double change_time);
		void revise_now_pause_time(int phase_id, double new_pause_time);
		void revise_now_pause_time_of_any_visited_poi(double new_pause_time, int id);

		//出発速度の修正
		void revise_starting_speed(double speed);
		

		//出発時の余り時間の修正


		//到着時の余り時間の修正
		void revise_dest_rest_time(double dest_rest_time);
		void revise_dest_rest_time_of_any_visited_poi(double dest_rest_time, int id);


		//現在の移動速度の修正
		void revise_now_speed(int phase, double speed);

		//trajectoryの修正
		std::shared_ptr<TRAJECTORY_TYPE> get_trajectory();
		void revise_path();
		void recalculation_path(const Graph::MapNodeIndicator& source, const Graph::MapNodeIndicator& destination, int phase_id);
	};

	//明示的特殊化
	template class RevisablePauseMobileEntity<Graph::Coordinate>;
	template class RevisablePauseMobileEntity<Geography::LatLng>;

}
