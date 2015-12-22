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

		//��~���Ԃ̏C��
		void revise_pause_time(double new_pause_time);
		void revise_pause_time_using_pause_phase(double new_pause_time, int phase);
		
		//���݂̒�~���Ԃ̏C��
		void add_now_pause_time(int now_phase, double change_time);
		void revise_now_pause_time(int phase_id, double new_pause_time);
		

		void revise_path();
		void recalculation_path(const Graph::MapNodeIndicator& source, const Graph::MapNodeIndicator& destination, int phase_id);
	};

	//�����I���ꉻ
	template class RevisablePauseMobileEntity<Graph::Coordinate>;
	template class RevisablePauseMobileEntity<Geography::LatLng>;

}
