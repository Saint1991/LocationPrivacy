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

		void revise_rest_pause_time(int phase_id, double new_pause_time);
		void revise_pause_time();


	};

	//�����I���ꉻ
	template class RevisablePauseMobileEntity<Graph::Coordinate>;
	template class RevisablePauseMobileEntity<Geography::LatLng>;

}
