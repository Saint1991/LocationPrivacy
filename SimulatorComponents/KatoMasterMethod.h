#ifdef SIMULATORCOMPONENTS_EXPORTS
#define KATO_MASTER_METHOD_API __declspec(dllexport)
#else
#define KATO_MASTER_METHOD_API __declspec(dllimport)
#endif


#pragma once
#include "IProposedMethod.h"
#include "BasicDbMap.h"
#include "LatLng.h"
#include "BasicUser.h"
#include "Dummy.h"
#include "BasicRequirement.h"
#include "Rectangle.h"
#include "RevisablePauseMobileEntity.h"
#include "KatoMethodRequirement.h"
#include "Probability.h"
#include "GeoCalculation.h"
#include "KatoBachelorMethod.h"

namespace Method {
	///<summary>
	/// ��������C�m���Ƙ_���̒�Ď�@�̎���
	///</summary>
	
	class KATO_MASTER_METHOD_API KatoMasterMethod : public KatoBachelorMethod
	{
	protected:
		//�����o�ϐ�
		double Tu;//���[�U�̓������Ԃ̕ω���
		enum ChangeParameter { LONGER_PAUSE_TIME, SHORTER_PAUSE_TIME, PATH, FASTER_SPEED, SLOER_SPEED, VISIT_POI, NO_CHANGE };
		static constexpr double MAX_VARIATION_OF_PAUSE_TIME = 0.7;
		static constexpr double MAX_VARIATION_OF_SPEED = 0.7;

		//���\�b�h
		std::shared_ptr<Entity::RevisablePauseMobileEntity<Geography::LatLng>> copy_predicted_user_plan(std::shared_ptr<Entity::PauseMobileEntity<Geography::LatLng>> input_user);

		std::pair<double, double> calc_max_variable_speed(double speed);
		std::pair<double, double> calc_max_variable_pause_time(double pause_time);

		bool check_going_same_poi_as_plan();//���[�U�̍s���v�����Ɋ܂܂���~�n�_�Ɍ������Ă��邩�ǂ������`�F�b�N
		ChangeParameter check_user_plan(int now_phase);//���[�U�̍s���v�����ύX�̃`�F�b�N
			ChangeParameter check_user_pause_time(int now_phase);
			ChangeParameter check_user_speed(int now_phase);
				double calc_user_speed();
			ChangeParameter check_user_path(int now_phase);
			ChangeParameter check_user_position(int now_phase);
			bool is_on_the_path(int phase_id);

		void update_user_plan(ChangeParameter check_parameter, int phase_id);
			void revise_user_trajectory_when_LONGER_PAUSE_TIME(int phase_id);//user�̒�~���Ԃ̏C��
			void revise_user_trajectory_when_SHORTER_PAUSE_TIME(int phase_id);//user�̒�~���Ԃ̏C��
			void revise_user_trajectory_when_PATH(int phase_id);//user�̈ړ��o�H�̏C��
			void revise_user_trajectory_when_FASTER_SPEED(int phase_id);//user�̍s�����x�̏C��
			void revise_user_trajectory_when_SLOER_SPEED(int phase_id);//user�̍s�����x�̏C��
			void revise_user_trajectory_when_VISIT_POI(int phase_id);//user�̒�~�n�_�̏C��

		void revise_dummy_trajectory(int phase_id);//�_�~�[�̍s���v�������C������
			void revise_dummy_pause_time(int phase_id);//�_�~�[�̒�~���Ԃ̏C��
			void revise_dummy_path(int phase_id);//�_�~�[�̈ړ��o�H�̏C��
			void revise_dummy_speed(int phase_id);//�_�~�[�̍s�����x�̏C��
			void revise_dummy_visit_poi(int phase_id);//�_�~�[�̒�~�n�_�̏C��
			
		void recalculation_path(const Graph::MapNodeIndicator& source, const Graph::MapNodeIndicator& destination, int *revise_phase, int visited_poi_id);
		void update_visited_pois_info_of_dummy();

		void increment_visited_pois_info_list_id_of_users(int phase_id);
		void clear_visited_pois_info_list_id_of_users();


	protected:
		//�����o�ϐ�
		std::shared_ptr<Entity::PauseMobileEntity<Geography::LatLng>> input_user;//�V�X�e���ւ̓��͂ɂ���ē���ꂽ���[�U�DSimulator�P�ʂō쐬����.�ύX����
		std::shared_ptr<Entity::PauseMobileEntity<Geography::LatLng>> real_user;//���ۂ̃��[�U.get_user()�Ŏ擾�\
		std::shared_ptr<Entity::RevisablePauseMobileEntity<Geography::LatLng>> predicted_user;//�\�����ꂽ���[�U�D��������C�����Ă����D

		std::shared_ptr<Entity::RevisablePauseMobileEntity<Geography::LatLng>> revising_dummy;//�C���Ώۃ_�~�[
		
		//���\�b�h
		void initialize();
		void revise_dummy_positions(int phase_id, entity_id dummy_id);
	
	public:
		KatoMasterMethod(std::shared_ptr<Map::HayashidaDbMap const> map, std::shared_ptr<Entity::DifferentMovementUser<Geography::LatLng>> user, std::shared_ptr<Requirement::KatoMethodRequirement const> requirement, std::shared_ptr<Time::TimeSlotManager> time_manager);
		virtual ~KatoMasterMethod();

		void run();
	};

}

