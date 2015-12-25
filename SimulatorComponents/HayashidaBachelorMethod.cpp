#include "stdafx.h"
#include "HayashidaBachelorMethod.h"

namespace Method
{

	///<summary>
	/// コンストラクタ
	/// これにSimulatorで作成した各種入力への参照を渡す
	///</summary>
	HayashidaBachelorMethod::HayashidaBachelorMethod(std::shared_ptr<Map::HayashidaDbMap const> map, std::shared_ptr<Entity::DifferentMovementUser<Geography::LatLng>> user, std::shared_ptr<Requirement::KatoMethodRequirement const> requirement, std::shared_ptr<Time::TimeSlotManager> time_manager)
		: KatoMasterMethod(map, user, requirement, time_manager)
	{
	}

	///<summary>
	/// デストラクタ
	///</summary>
	HayashidaBachelorMethod::~HayashidaBachelorMethod()
	{
	}

	///<summary>
	/// ユーザが訪問予定POIに向かっているかどうかをチェックする
	///</summary>
	bool HayashidaBachelorMethod::check_user_going_to_sheduled_POI()
	{
		return true;
	}

	///<summary>
	/// ユーザの行動を再予測する
	///</summary>
	void HayashidaBachelorMethod::repredicted_user_trajectory()
	{
		
	}

	///<summary>
	/// 提案手法の核になる部分
	/// ユーザの交差を再設定する
	///</summary>
	void HayashidaBachelorMethod::re_setting_of_user_cross()
	{
		
	}


	///<summary>
	/// 初期化 (今回は特にやることはない)
	///</summary>
	void HayashidaBachelorMethod::initialize()
	{
		//ユーザの動きの変更→新しく作る．
	}


	///<summary>
	/// ここが提案手法の核になる部分.ダミーの行動を修正する
	///</summary>
	void HayashidaBachelorMethod::adjust_dummy_positions()
	{
		clear_visited_pois_info_list_id_of_users();//usersのvisited_pois_info_list_idのクリア
		for (int phase_id = 0; phase_id < time_manager->phase_count(); phase_id++) {
			for (size_t dummy_id = 1; dummy_id <= entities->get_dummy_count(); dummy_id++) {
				revising_dummy = entities->get_dummy_by_id(dummy_id);
				//ダミーのvisited_pois_info_list_idの更新
				if ((phase_id - 1) == revising_dummy->get_pause_phases().back()) revising_dummy->increment_visited_pois_info_list_id();
				//ユーザのvisited_pois_info_list_idの更新
				increment_visited_pois_info_list_id_of_users(phase_id);
				
				if (!check_user_going_to_sheduled_POI()) {
					repredicted_user_trajectory();
					re_setting_of_user_cross();
				}
				revise_dummy_positions(phase_id, dummy_id);
			}
		}
	}

	/*
	///<summary>
	/// 決定した位置を基にMTC等各種評価値を算出する
	///</summary>
	void HayashidaBachelorMethod::evaluate()
	{

	}


	///<summary>
	/// 結果のファイルへのエクスポート
	///</summary>
	void HayashidaBachelorMethod::export_results()
	{

	}
	

	///<summary>
	/// 終了処理 (今回はスマートポインタを利用しているので，特にやることはない)
	///</summary>
	void HayashidaBachelorMethod::terminate()
	{

	}
	*/

	void HayashidaBachelorMethod::run()
	{
		//ここで実行時間の計測を開始
		timer->start();

		//初期化
		Method::KatoMasterMethod::initialize();

		//ここが実行部分(加藤さん卒論手法[Kato 13])
		Method::KatoBachelorMethod::decide_dummy_positions();

		//ここでユーザの行動の予測やダミーの行動を修正する(林田さん卒論手法[Hayashida 14])
		adjust_dummy_positions();
		
		//ここで計測を終了
		timer->end();

		//終了処理
		terminate();
	}


}
