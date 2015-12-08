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
	
	class KatoMasterMethod :public KatoBachelorMethod
	{
	private:
		double changes_in_arrival_time;//���[�U�̓������Ԃ̕ω���
		enum ChangeParameter { LONGER_PAUSE_TIME, SHORTER_PAUSE_TIME, PATH, FASTER_SPEED, SLOER_SPEED, VISIT_POI, NO_CHANGE };
		
		std::shared_ptr<Entity::PauseMobileEntity<Geography::LatLng>> make_predicted_user_plan(std::shared_ptr<Entity::PauseMobileEntity<Geography::LatLng>> input_user);
		
		bool check_going_pause_position_in_plan();//���[�U�̍s���v�����Ɋ܂܂���~�n�_�Ɍ������Ă��邩�ǂ������`�F�b�N
		std::pair<ChangeParameter, double> check_user_plan(int now_phase);//���[�U�̍s���v�����ύX�̃`�F�b�N
			std::pair<ChangeParameter, double> check_user_pause_time(int now_phase);
			std::pair<ChangeParameter, double> check_user_speed(int now_phase);
			std::pair<ChangeParameter, double> check_user_path(int now_phase);
			std::pair<ChangeParameter, double> check_user_position(int now_phase);

		void update_user_plan(std::pair<ChangeParameter, double> check_parameter, int phase_id);
			void modification_of_user_trajectory_when_LONGER_PAUSE_TIME(std::pair<ChangeParameter, double> check_parameter, int phase_id);//user�̒�~���Ԃ̏C��
			void modification_of_user_trajectory_when_SHORTER_PAUSE_TIME(std::pair<ChangeParameter, double> check_parameter, int phase_id);//user�̒�~���Ԃ̏C��
			void modification_of_user_trajectory_when_PATH(std::pair<ChangeParameter, double> check_parameter, int phase_id);//user�̈ړ��o�H�̏C��
			void modification_of_user_trajectory_when_FASTER_SPEED(std::pair<ChangeParameter, double> check_parameter, int phase_id);//user�̍s�����x�̏C��
			void modification_of_user_trajectory_when_SLOER_SPEED(std::pair<ChangeParameter, double> check_parameter, int phase_id);//user�̍s�����x�̏C��
			void modification_of_user_trajectory_when_VISIT_POI(std::pair<ChangeParameter, double> check_parameter, int phase_id);//user�̒�~�n�_�̏C��

		void revise_dummy_movement_plan(int phase_id);//�_�~�[�̍s���v�������C������
			void revise_dummy_pause_time(int phase_id);//�_�~�[�̒�~���Ԃ̏C��
			void revise_dummy_path(int phase_id);//�_�~�[�̈ړ��o�H�̏C��
			void revise_dummy_speed(int phase_id);//�_�~�[�̍s�����x�̏C��
			void revise_dummy_visit_poi(int phase_id);//�_�~�[�̒�~�n�_�̏C��

	protected:
		std::shared_ptr<Entity::PauseMobileEntity<Geography::LatLng>> input_user;//�V�X�e���ւ̓��͂ɂ���ē���ꂽ���[�U�DSimulator�P�ʂō쐬����.get_user()�Ŏ擾�\
		std::shared_ptr<Entity::PauseMobileEntity<Geography::LatLng>> real_user;//���ۂ̃��[�U.input_user����m���I�Ɋe�v�f��ύX������.
		std::shared_ptr<Entity::PauseMobileEntity<Geography::LatLng>> predicted_user;//�\�����ꂽ���[�U

		std::shared_ptr<Entity::PauseMobileEntity<Geography::LatLng>> revising_dummy;//�C���Ώۃ_�~�[

		void initialize();
		void revise_dummy_positions();
		//void evaluate();
		//void export_results();
		//void terminate();


	public:
		KatoMasterMethod(std::shared_ptr<Map::BasicDbMap const> map, std::shared_ptr<Entity::PauseMobileEntity<Geography::LatLng>> user, std::shared_ptr<Requirement::KatoMethodRequirement const> requirement, std::shared_ptr<Time::TimeSlotManager> time_manager);
		~KatoMasterMethod();

		void run();
	};

}

