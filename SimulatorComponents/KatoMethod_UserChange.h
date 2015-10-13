#pragma once
#include "BasicDbMap.h"
#include "LatLng.h"
#include "BasicUser.h"
#include "Dummy.h"
#include "BasicRequirement.h"
#include "IProposedMethod.h"
#include "Rectangle.h"

namespace Method
{

	///<summary>
	/// 加藤さん提案手法の実装
	///</summary>
	class KatoMethod_UserChange
		: public Framework::IProposedMethod<Map::BasicDbMap, User::BasicUser<Geography::LatLng>, Entity::Dummy<Geography::LatLng>, Requirement::BasicRequirement>
	{
	private:
		//変数
		int cell_nu_on_side;

		std::vector<Graph::Rectangle> make_grid(double grid_length, const Geography::LatLng& center, int cell_num_on_side);//Gridの作成
		std::vector<std::vector<int>> make_table_of_entity_num_in_cell_at_phase(std::vector<Graph::Rectangle> grid_list, int phase);
		std::vector<int> get_total_num_of_each_cell(std::vector<std::vector<int>>& entities_table);//各セルのフェーズトータルのエンティティの数
		Graph::Rectangle get_min_dummy_cell();//ユーザおよびダミーが存在する数が最小のセルを取得
		time_t get_min_dummy_num_time();//T秒間のユーザ及びダミーが存在する数が最小となる最初の時刻を取得
		void decide_base_positions_and_arrive_time(int dummy_id);// 生成中ダミー(k番目)の基準地点および基準地点到着時間の決定
		void decide_share_positions_and_arrive_time(int dummy_id);// 生成中ダミー(k番目)の共有地点および共有地点到着時間の決定
		void decide_dummy_path(int dummy_id);// 生成中ダミー(k番目)の移動経路の決定
		void revise_dummy_movement_plan();//ダミーの行動プランを修正する
		void revise_dummy_pause_time();//ダミーの停止時間の修正
		void revise_dummy_trajectory();//ダミーの移動経路の修正
		void revise_dummy_speed();//ダミーの行動速度の修正
		void revise_dummy_pose_position();//ダミーの停止地点の修正
		void check_user_plan();//ユーザの行動プラン変更のチェック

	protected:
		void initialize();
		void decide_dummy_positions();
		void revise_dummy_positions();
		void evaluate();
		void export_results();
		void terminate();

		
	public:
		KatoMethod_UserChange(std::shared_ptr<Map::BasicDbMap const> map, std::shared_ptr<User::BasicUser<Geography::LatLng>> user, std::shared_ptr<Requirement::BasicRequirement const> requirement, std::shared_ptr<Time::TimeSlotManager> time_manager);
		~KatoMethod_UserChange();

		void run();

	};
}


