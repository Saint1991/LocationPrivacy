#include "stdafx.h"
#include "KatoMethod_UserChange.h"

namespace Method
{
	
	///<summary>
	/// コンストラクタ
	/// これにSimulatorで作成した各種入力への参照を渡す
	///</summary>
	KatoMethod_UserChange::KatoMethod_UserChange(std::shared_ptr<Map::BasicDbMap const> map, std::shared_ptr<User::BasicUser<Geography::LatLng>> user, std::shared_ptr<Requirement::BasicRequirement const> requirement, std::shared_ptr<Time::TimeSlotManager> time_manager)
		: Framework::IProposedMethod<Map::BasicDbMap, User::BasicUser<Geography::LatLng>, Entity::Dummy<Geography::LatLng>, Requirement::BasicRequirement>(map, user, requirement, time_manager)
	{
	}

	///<summary>
	/// デストラクタ
	///</summary>
	KatoMethod_UserChange::~KatoMethod_UserChange()
	{
	}


	///<summary>
	/// T[s]ごとのグリッド領域を作成
	/// grid_lengthはグリッド全体の長さ
	///</summary>
	std::vector<Graph::Rectangle> KatoMethod_UserChange::make_grid(double grid_length, const Geography::LatLng& center, int cell_num_on_side)
	{
		double side_length = grid_length / cell_num_on_side;//セル一つ分の長方形の長さ
		//centerの四点の座標
		double top = center.y() + side_length * 1.5;
		double left = center.x() - side_length * 1.5;
		double bottom = center.y() + side_length * 0.5;
		double right = center.x() - side_length * 0.5;
		 
		std::vector<Graph::Rectangle> grid_list;//グリッド全体を管理するリスト
		
		double base_left = left;//左上の正方形のleftをループの際の基準とする
		double base_right = right;//左上の正方形のrightをループの際の基準とする

		for (int i = 0; i < cell_num_on_side; i++)
		{
			for (int j = 0; j < cell_num_on_side; j++)
			{
				grid_list.push_back(Graph::Rectangle(top, left, bottom, right));
				right += side_length;
				left += side_length;
			}

			top -= side_length;
			bottom -= side_length;
			left = base_left;
			right = base_right;
		}

		return grid_list;
	}


	
	///<summary>
	/// tの時のDの平均位置を中心を求める
	///</summary>
	Geography::LatLng KatoMethod_UserChange::get_center_position()
	{
		return Geography::LatLng(10.0, 10.0);
	}


	/*
	///<summary>
	/// ユーザおよびダミーが存在する数が最小のセルを取得
	///</summary>
	void KatoMethod_UserChange::get_min_dummy_cell()
	{

	}

	///<summary>
	/// T秒間のユーザ及びダミーが存在する数が最小となる最初の時刻を取得
	///</summary>
	void KatoMethod_UserChange::get_min_dummy_num_time()
	{

	}
	*/

	/*
	///<summary>
	/// 生成中ダミー(k番目)の基準地点および基準地点到着時間の決定
	///</summary>
	void KatoMethod_UserChange::decide_base_positions_and_arrive_time()
	{
		int t;
		int T = 10;//周期
		double anonymous_area = 1600.0;


		for (t = 0; t < t_end; t++)
		{
			t = t + T;
			Geography::LatLng center = get_center_position();
			std::vector<Graph::Rectangle> grid_list = make_grid(std::sqrt(Requirement::BasicRequirement().required_anonymous_area),center,3);

			for (int i = 0; i < 8; i++)
			{
				//領域Gに存在するダミーの移動経路の数
			}

		}

		get_min_dummy_cell();
		get_min_dummy_num_time();

		while (//p_baseがG_baseに存在
			)
		{
			p_base = GetPausePosition();
		}

		return PP <p_base, t_base>;
	}

	/*

	///<summary>
	/// 生成中ダミー(k番目)の共有地点および共有地点到着時間の決定
	///</summary>
	void KatoMethod_UserChange::decide_share_positions_and_arrive_time()
	{
		for (size_t i = 0; 生成中のダミーの交差回数 > ave(D0cross); i++)
		{
			D_min = std::min();
			p_shared = random(Dmincrossの停止地点);
			t_shared = p_sharedに到着する時間;

			if (PPinに含まれる全ての停止地点到着時間<=t_shared)
			{
				while (到達可能かどうか) {
					t_last = std::max(PPinに含まれる停止地点到着時間);
					p_last = PPinに含まれるt_lastに到着する停止地点;
				}
				
			}
			else if (PPinに含まれる全ての停止地点到着時間 >= t_shared)
			{
				while (到達可能かどうか) {
					t_first = std::min(PPinに含まれる停止地点到着時間);
					p_first = PPinに含まれるt_firstに到着する停止地点;
				}
			}
			else
			{
				while (previousとsharedの到達可能かどうか) {
					t_previous = a;
				}
			}

			PPに<p_shared, t_shared>を追加
			Dmincross = += 1;
			生成中のダミーの交差回数 += 1;
		}
		
	
	}

	///<summary>
	/// 生成中ダミー(k番目)の移動経路の決定
	///</summary>
	void KatoMethod_UserChange::decide_dummy_path()
	{

	}

	///<summary>
	/// ダミーの行動プランを修正する
	///</summary>
	void KatoMethod_UserChange::revise_dummy_movement_plan()
	{

	}


	///<summary>
	/// ダミーの停止時間の修正
	///</summary>
	void KatoMethod_UserChange::revise_dummy_pause_time()
	{

	}


	///<summary>
	/// ダミーの移動経路の修正
	///</summary>
	void KatoMethod_UserChange::revise_dummy_trajectory()
	{

	}


	///<summary>
	/// ダミーの行動速度の修正
	///</summary>
	void KatoMethod_UserChange::revise_dummy_speed()
	{

	}


	///<summary>
	/// ダミーの停止位置の修正
	///</summary>
	void KatoMethod_UserChange::revise_dummy_pose_position()
	{

	}


	///<summary>
	/// ユーザの行動プラン変更の判断
	///</summary>
	void KatoMethod_UserChange::check_user_plan()
	{

	}

	///<summary>
	/// 初期化 (今回は特にやることはない)
	///</summary>
	void KatoMethod_UserChange::initialize()
	{
		//ユーザの動きの変更→新しく作る．
	}


	///<summary>
	/// ここが提案手法の核になる部分
	///</summary>
	void KatoMethod_UserChange::decide_dummy_positions()
	{
		decide_base_positions_and_arrive_time();// 生成中ダミー(k番目)の基準地点および基準地点到着時間の決定
		decide_share_positions_and_arrive_time();// 生成中ダミー(k番目)の共有地点および共有地点到着時間の決定
		decide_dummy_path();// 生成中ダミー(k番目)の移動経路の決定
	}
	
	
	///<summary>
	/// ここが提案手法の核になる部分.ダミーの行動を修正する
	///</summary>
	void KatoMethod_UserChange::revise_dummy_positions()
	{

	}

		

	///<summary>
	/// 決定した位置を基にMTC等各種評価値を算出する
	///</summary>
	void KatoMethod_UserChange::evaluate()
	{

	}


	///<summary>
	/// 結果のファイルへのエクスポート
	///</summary>
	void KatoMethod_UserChange::export_results()
	{

	}


	///<summary>
	/// 終了処理 (今回はスマートポインタを利用しているので，特にやることはない)
	///</summary>
	void KatoMethod_UserChange::terminate()
	{

	}

	void KatoMethod_UserChange::run()
	{
		//ここで実行時間の計測を開始
		timer->start();

		//初期化
		initialize();

		//ここが実行部分(各時刻のダミー位置を計算する)
		decide_dummy_positions();

		//ここでダミーの行動を修正する
		revise_dummy_positions();

		//ここで計測を終了
		timer->end();

		//設定したダミー，ユーザの位置を基にMTCなどの評価指標を計算する
		evaluate();

		//実行時間以外のエクスポート
		export_results();

		//終了処理
		terminate();
	}
	*/
}
