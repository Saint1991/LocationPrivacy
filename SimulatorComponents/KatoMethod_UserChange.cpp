#include "stdafx.h"
#include "KatoMethod_UserChange.h"

namespace Method
{
	
	///<summary>
	/// コンストラクタ
	/// これにSimulatorで作成した各種入力への参照を渡す
	///</summary>
	KatoMethod_UserChange::KatoMethod_UserChange(std::shared_ptr<Map::BasicDbMap const> map, std::shared_ptr<User::BasicUser<Geography::LatLng>> user, std::shared_ptr<Requirement::BasicRequirement const> requirement, std::shared_ptr<Time::TimeSlotManager> time_manager)
		: Framework::IProposedMethod<Map::BasicDbMap, User::BasicUser<Geography::LatLng>, Entity::Dummy<Geography::LatLng>, Requirement::BasicRequirement>(map, user, requirement, time_manager),grid_list(std::vector<Grid>(time_manager->phase_count()))
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
	std::vector <Graph::Rectangle<Geography::LatLng>> KatoMethod_UserChange::make_grid(double grid_length, const Geography::LatLng& center, int cell_num_on_side)
	{
		double side_length = grid_length / cell_num_on_side;//セル一つ分の長方形の長さ
		//centerの四点の座標
		double top = center.y() + side_length * 1.5;
		double left = center.x() - side_length * 1.5;
		double bottom = center.y() + side_length * 0.5;
		double right = center.x() - side_length * 0.5;
		 
		std::vector<Graph::Rectangle<Geography::LatLng>> grid_list;//グリッド全体を管理するリスト
		
		double base_left = left;//左上の正方形のleftをループの際の基準とする
		double base_right = right;//左上の正方形のrightをループの際の基準とする

		for (int i = 0; i < cell_num_on_side; i++)
		{
			for (int j = 0; j < cell_num_on_side; j++)
			{
				grid_list.push_back(Graph::Rectangle<Geography::LatLng>(top, left, bottom, right));
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


	/*
	///<summary>
	/// T[s]ごとのグリッド領域を作成
	/// grid_lengthはグリッド全体の長さ
	///</summary>
	std::vector<std::vector<int>> KatoMethod_UserChange::make_table_of_entity_num_in_cell_at_phase(std::vector<Graph::Rectangle> grid_list, int phase)
	{

	}
	*/

	
	///<summary>
	/// グリッドテーブルの各グリッドごとのエンティティの合計を取得
	///</summary>
	std::vector<int> KatoMethod_UserChange::get_total_num_of_each_cell(std::vector<std::vector<int>>& entities_table) {
		std::vector<int> total_entity_num_all_phase;
		for (int grid_id = 0; grid_id < CELL_NUM_ON_SIDE*CELL_NUM_ON_SIDE; grid_id++)
		{
			int temp = 0;
			for (int phase = 0; phase < time_manager->phase_count(); phase++)
			{
				temp += entities_table.at(grid_id).at(phase);
			}
			total_entity_num_all_phase.at(grid_id) = temp;
		}
		return total_entity_num_all_phase;
	}

	/*
	///<summary>
	/// ユーザおよびダミーが存在する数が最小のセルを取得
	///</summary>
	Graph::Rectangle KatoMethod_UserChange::get_min_dummy_cell()
	{
		return Graph::Rectangle(1.0,1.0,1.0,1.0);//ここに返すのは最小のセル
	}

	
	///<summary>
	/// T秒間のユーザ及びダミーが存在する数が最小となる最初の時刻を取得
	///</summary>
	time_t KatoMethod_UserChange::get_min_dummy_num_time()
	{
		return 0;
	}
	*/
	
	
	///<summary>
	/// 生成中ダミー(k番目)の基準地点および基準地点到着時間の決定
	///</summary>
	void KatoMethod_UserChange::decide_base_positions_and_arrive_time(int dummy_id)
	{
		int phase = 0;//phase
		const int GRID_TOTAL_NUM = CELL_NUM_ON_SIDE*CELL_NUM_ON_SIDE;//グリッドの数
		
		//各グリッドの各フェイズにおけるentitiesの数を記憶するためのtable(動的配列)の作成
		std::vector<std::vector<int>> entities_num_table(GRID_TOTAL_NUM, std::vector<int>(time_manager->phase_count(),0));
		

		while (phase <= time_manager->phase_count())
		{
			std::shared_ptr<Geography::LatLng const> center = entities->get_average_position_of_phase(phase);//中心位置を求める
			Grid grid = make_grid(requirement->required_anonymous_area, *center, CELL_NUM_ON_SIDE);//phaseごとにグリッドを作成
			grid_list.at(phase) = grid;

			//あるphaseにおける各セルに存在するユーザ及び生成済みダミーの移動経路(停止地点)の数
			//横がセルのid，縦がphaseを表す動的２次元配列で記憶
			//k-2個目までのtableを作っておいて，k-1個目を＋１して更新すればより効率が良い
			int cell_id = 0;//セルのid
			for (std::vector<Graph::Rectangle<Geography::LatLng>>::iterator iter = grid.begin(); iter != grid.end(); iter++)
			{
				entities_num_table.at((cell_id++) - 1).at(phase) = entities->get_entity_count_within_boundary(phase, *iter);
			}

			phase++;//サービス利用間隔をtime_managerから逐一求めないといけないかも
		}

		//全てのフェーズにおける各セルのエンティティの合計
		std::vector<int> total_entity_num_all_phase = get_total_num_of_each_cell(entities_num_table);
		
				
		//全てのフェーズにおいて，エンティティが最小になるセルidを取得
		std::vector<int>::iterator cell_iter = std::min_element(total_entity_num_all_phase.begin(), total_entity_num_all_phase.end());
		size_t min_cell_id = std::distance(total_entity_num_all_phase.begin(), cell_iter);

		//min_cell_idのセルで最小になる最初のphaseを取得
		std::vector<int>::iterator phase_iter = std::min_element(entities_num_table.at(min_cell_id).begin(), entities_num_table.at(min_cell_id).end());
		int base_phase = std::distance(entities_num_table.at(min_cell_id).begin(), phase_iter);

		
		//取得したcell_id,phaseにおける停止地点を取得
		//poiがなかった時の場合分けも考慮が必要かもしれない
		Graph::Rectangle<Geography::LatLng> cell = grid_list.at(base_phase).at(min_cell_id);
		std::vector<std::shared_ptr<Map::BasicPoi const>> poi_within_base_point_grid = map->find_pois_within_boundary(cell);
		Math::Probability generator;
		int index = generator.uniform_distribution(0, poi_within_base_point_grid.size() - 1);
		std::shared_ptr<Map::BasicPoi const> poi = poi_within_base_point_grid.at(index);
		
		Geography::LatLng base_point = poi->data->get_position();
		
		entities->get_dummy_by_id(dummy_id)->set_position_of_phase(base_phase,base_point);
		
	}

	
	/*
	
	///<summary>
	/// 生成中ダミー(k番目)の共有地点および共有地点到着時間の決定
	///</summary>
	void KatoMethod_UserChange::decide_share_positions_and_arrive_time(int dummy_id)
	{
		while (生成中のダミーの交差回数 > ave(D0cross))
		{
			unsigned int min_cross_entity_id = entities->get_min_cross_entity_id();
			
			//userかdummyで場合分け
			Geography::LatLng share_position = 
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
	void KatoMethod_UserChange::decide_dummy_path(int dummy_id)
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
	*/
	

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
		/*
		for (size_t dummy_id = 1; dummy_id <= entities->get_dummy_count(); dummy_id++)
		{
			//decide_base_positions_and_arrive_time(dummy_id);// 生成中ダミー(k番目)の基準地点および基準地点到着時間の決定
			//decide_share_positions_and_arrive_time(dummy_id);// 生成中ダミー(k番目)の共有地点および共有地点到着時間の決定
			//decide_dummy_path(dummy_id);// 生成中ダミー(k番目)の移動経路の決定
			
		}*/
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
	
}
