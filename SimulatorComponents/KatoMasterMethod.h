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
	
	class KatoMasterMethod :
		public KatoBachelorMethod
	{
	private:
		time_t time_to_change;//行動の変化分

		void revise_dummy_movement_plan(int phase_id);//ダミーの行動プランを修正する
		void revise_dummy_pause_time(int phase_id);//ダミーの停止時間の修正
		void revise_dummy_trajectory(int phase_id);//ダミーの移動経路の修正
		void revise_dummy_speed(int phase_id);//ダミーの行動速度の修正
		void revise_dummy_pose_position(int phase_id);//ダミーの停止地点の修正

		bool check_going_pause_position_in_plan();//ユーザの行動プランに含まれる停止地点に向かっているかどうかをチェック
		int check_user_plan();//ユーザの行動プラン変更のチェック
		bool check_user_pause_time();
		bool check_user_speed();
		bool check_user_path();
		bool check_user_position();

		std::shared_ptr<Entity::PauseMobileEntity<Geography::LatLng>> predict_user_plan(std::shared_ptr<Entity::PauseMobileEntity<Geography::LatLng>> input_user_plan);//ユーザの行動を予測する.
		void predict_user_next_pause_position_time(int check_num);
		void update_user_plan();



	protected:
		std::shared_ptr<Entity::PauseMobileEntity<Geography::LatLng>> predicted_user;//予測されたユーザ

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

