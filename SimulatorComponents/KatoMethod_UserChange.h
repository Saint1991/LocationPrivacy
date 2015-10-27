#pragma once
#include "BasicDbMap.h"
#include "LatLng.h"
#include "BasicUser.h"
#include "Dummy.h"
#include "BasicRequirement.h"
#include "IProposedMethod.h"
#include "Rectangle.h"
#include "PauseMobileEntity.h"
#include "KatoMethodRequirement.h"
#include "Probability.h"
#include "GeoCalculation.h"

namespace Method
{

	///<summary>
	/// 加藤さん提案手法の実装
	///</summary>
	class KatoMethod_UserChange
		: public Framework::IProposedMethod<Map::BasicDbMap, Entity::PauseMobileEntity<Geography::LatLng>, Entity::PauseMobileEntity<Geography::LatLng>, Requirement::KatoMethodRequirement>
	{
	private:
		//変数
		const int CELL_NUM_ON_SIDE = 3;//グリッドのセルの一辺の数

		typedef std::vector<Graph::Rectangle<Geography::LatLng>> Grid;
		std::vector<Grid> grid_list;

		typedef unsigned int entity_id;

		std::shared_ptr<Entity::PauseMobileEntity<Geography::LatLng>> creating_dummy;
		std::shared_ptr<Entity::PauseMobileEntity<Geography::LatLng>> predict_user;
		std::shared_ptr<Time::TimeSlotManager> revise_time_manager;

		time_t time_to_change;//行動の変化分

		double a = 6378137;//赤道半径
		double b = 6356752.314;//極半径


		//メソッド
		std::vector<Graph::Rectangle<Geography::LatLng>> make_grid(double grid_length, const Geography::LatLng& center, int cell_num_on_side);//Gridの作成
		//std::vector<std::vector<int>> make_table_of_entity_num_in_cell_at_phase(std::vector<Graph::Rectangle<Geography::LatLng>> grid_list, int phase);
		std::vector<int> get_total_num_of_each_cell_at_interval_phase(std::vector<std::vector<int>>& entities_table, int start_phase, int end_phase);//各セルのフェーズトータルのエンティティの数
		std::vector<int> get_total_num_of_each_cell_at_all_phase(std::vector<std::vector<int>>& entities_table);//各セルのフェーズトータルのエンティティの数
		std::vector<std::shared_ptr<Map::BasicPoi const>> candidate_pois_list(const Graph::Rectangle<Geography::LatLng>& boundary);
		void decide_base_positions_and_arrive_time(int dummy_id);// 生成中ダミー(k番目)の基準地点および基準地点到着時間の決定
		void decide_share_positions_and_arrive_time(int dummy_id);// 生成中ダミー(k番目)の共有地点および共有地点到着時間の決定
		void decide_destination_on_the_way(int dummy_id);// 生成中ダミー(k番目)の移動経路の決定
		
		void revise_dummy_movement_plan(int phase_id);//ダミーの行動プランを修正する
		void revise_dummy_pause_time(int phase_id);//ダミーの停止時間の修正
		void revise_dummy_trajectory(int phase_id);//ダミーの移動経路の修正
		void revise_dummy_speed(int phase_id);//ダミーの行動速度の修正
		void revise_dummy_pose_position(int phase_id);//ダミーの停止地点の修正
		
		bool check_going_pause_position_in_plan();//ユーザの行動プランに含まれる停止地点に向かっているかどうかをチェック
		bool check_user_plan();//ユーザの行動プラン変更のチェック
		void predict_user_plan();//ユーザの行動を予測する.
		Geography::LatLng position_from_node_with_distance(Geography::LatLng position, double distance);

	protected:
		
		void initialize();
		void decide_dummy_positions();
		void revise_dummy_positions();
		void evaluate();
		void export_results();
		void terminate();

		
	public:
		KatoMethod_UserChange(std::shared_ptr<Map::BasicDbMap const> map, std::shared_ptr<Entity::PauseMobileEntity<Geography::LatLng>> user, std::shared_ptr<Requirement::KatoMethodRequirement const> requirement, std::shared_ptr<Time::TimeSlotManager> time_manager, std::shared_ptr<Time::TimeSlotManager> revise_time_manager);
		~KatoMethod_UserChange();

		void run();

	};
}


