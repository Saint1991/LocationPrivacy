#include "stdafx.h"
#include "HayashidaBachelorMethod.h"

namespace Method
{

	///<summary>
	/// コンストラクタ
	/// これにSimulatorで作成した各種入力への参照を渡す
	///</summary>
	HayashidaBachelorMethod::HayashidaBachelorMethod(std::shared_ptr<Map::BasicDbMap const> map, std::shared_ptr<Entity::PauseMobileEntity<Geography::LatLng>> user, std::shared_ptr<Requirement::KatoMethodRequirement const> requirement, std::shared_ptr<Time::TimeSlotManager> time_manager)
		: KatoBachelorMethod(map, user, requirement, time_manager),
		predicted_user(nullptr)
	{
	}

	///<summary>
	/// デストラクタ
	///</summary>
	HayashidaBachelorMethod::~HayashidaBachelorMethod()
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
	/// ここが提案手法の核になる部分
	///</summary>
	void HayashidaBachelorMethod::decide_dummy_positions()
	{
		
		/*
		for (size_t dummy_id = 1; dummy_id <= entities->get_dummy_count(); dummy_id++)
		{
			creating_dummy = entities->get_dummy_by_id(dummy_id);

			decide_base_positions_and_arrive_time(dummy_id);// 生成中ダミー(k番目)の基準地点および基準地点到着時間の決定
			decide_share_positions_and_arrive_time(dummy_id);// 生成中ダミー(k番目)の共有地点および共有地点到着時間の決定
			decide_destination_on_the_way(dummy_id);// 生成中ダミー(k番目)の移動経路の決定
		}
		*/
	}


	///<summary>
	/// ここが提案手法の核になる部分.ダミーの行動を修正する
	///</summary>
	void HayashidaBachelorMethod::adjust_dummy_positions()
	{
		/*
		time_t time_to_change = 0;// (修正後の)time_manager->time_of_phase(phase_id) - (修正前の)time_manager->time_of_phase(phase_id);
		predict_user = predict_user_plan(entities->get_user());

		for (int phase_id = 0; phase_id < time_manager->phase_count(); phase_id++)
		{
			if (check_going_pause_position_in_plan()) {
				if (check_user_plan() != 0) {
					predict_user_next_pause_position_time(check_user_plan());//次の停止地点の到着時間を予測
					revise_dummy_movement_plan(phase_id);
					update_user_plan();
				}
			}
		}
		*/
		
	}


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


	void HayashidaBachelorMethod::run()
	{
		//ここで実行時間の計測を開始
		timer->start();

		//初期化
		initialize();

		//ここが実行部分(各時刻のダミー位置を計算する)
		decide_dummy_positions();

		//ここでユーザの行動の予測やダミーの行動を修正する
		adjust_dummy_positions();

		//ここで計測を終了
		timer->end();

		//設定したダミー，ユーザの位置を基にMTCなどの評価指標を計算する
		evaluate();

		//実行時間以外のエクスポート
		export_results();

		//終了処理
		terminate();
	}


}
