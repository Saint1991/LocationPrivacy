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
	/// ��~���Ԃ��܂񂾈ړ��̂�\���N���X
	/// ���[�U�C�_�~�[��\���̂ɗp����N���X (PauseMobileEntity����h��)
	/// ��~���ԁC���x�̕ύX���\�b�h�͂��̃N���X�ɏ����D
	/// Trajectory(path,poi)�̕ύX��RevisableTrajectory�N���X�ɏ���
	///</summary>
	template <typename POSITION_TYPE = Geography::LatLng, typename TRAJECTORY_TYPE = Graph::RevisableTrajectory<POSITION_TYPE>>
	class REVISABLE_PAUSE_MOBILE_ENTITY_API RevisablePauseMobileEntity : public PauseMobileEntity<POSITION_TYPE, TRAJECTORY_TYPE>
	{
	public:
		RevisablePauseMobileEntity(entity_id id, std::shared_ptr<Time::TimeSlotManager const> timeslot);
		~RevisablePauseMobileEntity();

		//��~�t�F�[�Y�̏C��
		void revise_pause_phases(int start_pause_phase, int num);
		void revise_pause_phases_of_any_visited_poi(int start_pause_phase, int num, int id);
		void add_pause_phases(int num, int phase);
		void remove_pause_phases(int num, int phase);

		//����phase�̏C��
		void revise_arrive_phase(int arrive_phase);
		void revise_arrive_phase_of_any_visited_poi(int arrive_phase, int id);
		void revise_all_arrive_phase();


		//��~���Ԃ̏C��
		void revise_pause_time(double new_pause_time);
		void revise_pause_time_using_pause_phase(double new_pause_time, int phase);
		void revise_pause_time_of_any_visited_poi(double new_pause_time, int id);

		//���݂̒�~���Ԃ̏C��
		void add_now_pause_time(int now_phase, double change_time);
		void revise_now_pause_time(int phase_id, double new_pause_time);
		void revise_now_pause_time_of_any_visited_poi(double new_pause_time, int id);

		//�o�����x�̏C��
		void revise_starting_speed(double speed);
		

		//�o�����̗]�莞�Ԃ̏C��


		//�������̗]�莞�Ԃ̏C��
		void revise_dest_rest_time(double dest_rest_time);
		void revise_dest_rest_time_of_any_visited_poi(double dest_rest_time, int id);


		//���݂̈ړ����x�̏C��
		void revise_now_speed(int phase, double speed);

		//trajectory�̏C��
		std::shared_ptr<TRAJECTORY_TYPE> get_trajectory();
		void revise_path();
		void recalculation_path(const Graph::MapNodeIndicator& source, const Graph::MapNodeIndicator& destination, int phase_id);
	};

	//�����I���ꉻ
	template class RevisablePauseMobileEntity<Graph::Coordinate>;
	template class RevisablePauseMobileEntity<Geography::LatLng>;

}
