#pragma once
#include "IProposedMethod.h"
#include "BasicDbMap.h"
#include "LatLng.h"
#include "BasicUser.h"
#include "Dummy.h"
#include "BasicRequirement.h"
#include "Rectangle.h"
#include "PauseMobileEntity.h"
#include "KatoMethodRequirement.h"
#include "Probability.h"
#include "GeoCalculation.h"
#include "KatoBachelorMethod.h"

namespace Method {
	///<summary>
	/// ��������C�m���Ƙ_���̒�Ď�@�̎���
	///</summary>
	
	class KatoMasterMethod :
		public KatoBachelorMethod
	{
	private:
		time_t time_to_change;//�s���̕ω���

		void revise_dummy_movement_plan(int phase_id);//�_�~�[�̍s���v�������C������
		void revise_dummy_pause_time(int phase_id);//�_�~�[�̒�~���Ԃ̏C��
		void revise_dummy_trajectory(int phase_id);//�_�~�[�̈ړ��o�H�̏C��
		void revise_dummy_speed(int phase_id);//�_�~�[�̍s�����x�̏C��
		void revise_dummy_pose_position(int phase_id);//�_�~�[�̒�~�n�_�̏C��

		bool check_going_pause_position_in_plan();//���[�U�̍s���v�����Ɋ܂܂���~�n�_�Ɍ������Ă��邩�ǂ������`�F�b�N
		int check_user_plan();//���[�U�̍s���v�����ύX�̃`�F�b�N
		bool check_user_pause_time();
		bool check_user_speed();
		bool check_user_path();
		bool check_user_position();

		std::shared_ptr<Entity::PauseMobileEntity<Geography::LatLng>> predict_user_plan(std::shared_ptr<Entity::PauseMobileEntity<Geography::LatLng>> input_user_plan);//���[�U�̍s����\������.
		void predict_user_next_pause_position_time(int check_num);
		void update_user_plan();



	protected:
		std::shared_ptr<Entity::PauseMobileEntity<Geography::LatLng>> predicted_user;//�\�����ꂽ���[�U

		void initialize();
		void decide_dummy_positions();
		void revise_dummy_positions();
		void evaluate();
		void export_results();
		void terminate();


	public:
		KatoMasterMethod(std::shared_ptr<Map::BasicDbMap const> map, std::shared_ptr<Entity::PauseMobileEntity<Geography::LatLng>> user, std::shared_ptr<Requirement::KatoMethodRequirement const> requirement, std::shared_ptr<Time::TimeSlotManager> time_manager);
		~KatoMasterMethod();

		void run();
	};

}

