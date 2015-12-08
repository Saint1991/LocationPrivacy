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
	/// 加藤さん修士卒業論文の提案手法の実装
	///</summary>
	
	class KatoMasterMethod :public KatoBachelorMethod
	{
	private:
		double changes_in_arrival_time;//ユーザの到着時間の変化分
		enum ChangeParameter { LONGER_PAUSE_TIME, SHORTER_PAUSE_TIME, PATH, FASTER_SPEED, SLOER_SPEED, VISIT_POI, NO_CHANGE };
		
		std::shared_ptr<Entity::PauseMobileEntity<Geography::LatLng>> make_predicted_user_plan(std::shared_ptr<Entity::PauseMobileEntity<Geography::LatLng>> input_user);
		
		bool check_going_pause_position_in_plan();//ユーザの行動プランに含まれる停止地点に向かっているかどうかをチェック
		std::pair<ChangeParameter, double> check_user_plan(int now_phase);//ユーザの行動プラン変更のチェック
			std::pair<ChangeParameter, double> check_user_pause_time(int now_phase);
			std::pair<ChangeParameter, double> check_user_speed(int now_phase);
			std::pair<ChangeParameter, double> check_user_path(int now_phase);
			std::pair<ChangeParameter, double> check_user_position(int now_phase);

		void update_user_plan(std::pair<ChangeParameter, double> check_parameter, int phase_id);
			void modification_of_user_trajectory_when_LONGER_PAUSE_TIME(std::pair<ChangeParameter, double> check_parameter, int phase_id);//userの停止時間の修正
			void modification_of_user_trajectory_when_SHORTER_PAUSE_TIME(std::pair<ChangeParameter, double> check_parameter, int phase_id);//userの停止時間の修正
			void modification_of_user_trajectory_when_PATH(std::pair<ChangeParameter, double> check_parameter, int phase_id);//userの移動経路の修正
			void modification_of_user_trajectory_when_FASTER_SPEED(std::pair<ChangeParameter, double> check_parameter, int phase_id);//userの行動速度の修正
			void modification_of_user_trajectory_when_SLOER_SPEED(std::pair<ChangeParameter, double> check_parameter, int phase_id);//userの行動速度の修正
			void modification_of_user_trajectory_when_VISIT_POI(std::pair<ChangeParameter, double> check_parameter, int phase_id);//userの停止地点の修正

		void revise_dummy_movement_plan(int phase_id);//ダミーの行動プランを修正する
			void revise_dummy_pause_time(int phase_id);//ダミーの停止時間の修正
			void revise_dummy_path(int phase_id);//ダミーの移動経路の修正
			void revise_dummy_speed(int phase_id);//ダミーの行動速度の修正
			void revise_dummy_visit_poi(int phase_id);//ダミーの停止地点の修正

	protected:
		std::shared_ptr<Entity::PauseMobileEntity<Geography::LatLng>> input_user;//システムへの入力によって得られたユーザ．Simulator単位で作成する.get_user()で取得可能
		std::shared_ptr<Entity::PauseMobileEntity<Geography::LatLng>> real_user;//実際のユーザ.input_userから確率的に各要素を変更させる.
		std::shared_ptr<Entity::PauseMobileEntity<Geography::LatLng>> predicted_user;//予測されたユーザ

		std::shared_ptr<Entity::PauseMobileEntity<Geography::LatLng>> revising_dummy;//修正対象ダミー

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

