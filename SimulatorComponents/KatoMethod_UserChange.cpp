#include "stdafx.h"
#include "KatoMethod_UserChange.h"

namespace Method
{
	
	///<summary>
	/// コンストラクタ
	/// これにSimulatorで作成した各種入力への参照を渡す
	///</summary>
	KatoMethod_UserChange::KatoMethod_UserChange(std::shared_ptr<Map::BasicDbMap const> map, std::shared_ptr<Entity::PauseMobileEntity<Geography::LatLng>> user, std::shared_ptr<Requirement::KatoMethodRequirement const> requirement, std::shared_ptr<Time::TimeSlotManager> time_manager)
		: Framework::IProposedMethod<Map::BasicDbMap, Entity::PauseMobileEntity<Geography::LatLng>, Entity::PauseMobileEntity<Geography::LatLng>, Requirement::KatoMethodRequirement>(map, user, requirement, time_manager),grid_list(std::vector<Grid>(time_manager->phase_count()))
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
		//一様分布でランダム取得
		//poiがなかった時の場合分けも考慮が必要かもしれない
		Graph::Rectangle<Geography::LatLng> cell = grid_list.at(base_phase).at(min_cell_id);
		std::vector<std::shared_ptr<Map::BasicPoi const>> poi_within_base_point_grid = map->find_pois_within_boundary(cell);
		Math::Probability generator;
		int index = generator.uniform_distribution(0, poi_within_base_point_grid.size() - 1);
		std::shared_ptr<Map::BasicPoi const> poi = poi_within_base_point_grid.at(index);
		
		Graph::node_id base_poi_id = poi->get_id();
		Geography::LatLng base_point = poi->data->get_position();
		
		entities->get_dummy_by_id(dummy_id)->set_position_of_phase(base_phase, Graph::MapNodeIndicator(base_poi_id),base_point);
		
	}

	
	
	
	///<summary>
	/// 生成中ダミー(k番目)の共有地点および共有地点到着時間の決定
	///</summary>
	void KatoMethod_UserChange::decide_share_positions_and_arrive_time(int dummy_id)
	{
		//交差回数が少ないエンティティを優先的に交差対象にして繰り返す
		std::list<std::pair<Entity::entity_id, int>> entity_list_order_by_cross = entities->get_entity_id_list_order_by_cross_count();
		for (std::list<std::pair<Entity::entity_id, int>>::const_iterator iter = entity_list_order_by_cross.begin(); iter != entity_list_order_by_cross.end(); iter++)
		{

			//未生成のダミーのものはスキップ
			if (iter->first > dummy_id) continue;

			//交差対象のID(交差回数最小)
			int cross_target = iter->first;
			int target_phase = INVALID;

			//BasicUserはDummyを継承しているのでポインタ代入は可能
			//targetが交差するユーザorダミー
			//target_phasesは交差が設定されていないphaseの一覧
			std::shared_ptr<Entity::PauseMobileEntity<Geography::LatLng>> target = cross_target == 0 ? entities->get_user() : entities->get_dummy_by_id(cross_target);
			std::vector<int> target_phases = target->find_cross_not_set_phases();

			//交差未設定の時刻からランダムに共有地点設定を試みる
			std::random_device device;
			std::mt19937_64 generator(device());
			std::shuffle(target_phases.begin(), target_phases.end(), generator);

			//フェーズが0でない交差が未設定のフェーズに交差地点を設定する．
			for (std::vector<int>::const_iterator target_phase = target_phases.begin(); target_phase != target_phases.end(); target_phase++) {
				if (*target_phase == 0) continue;
				int share_phase = *target_phase;
				std::pair<Graph::MapNodeIndicator, std::shared_ptr<Geography::LatLng const>> share_position = target->read_node_pos_info_of_phase(share_phase);

				//生成中ダミーの既に停止位置が決定しているフェーズよりも共有フェーズが大きい場合
				if (entities->read_dummy_by_id(dummy_id)->read_position_of_phase(time_manager->phase_count()) <= share_phase)
				{
					std::pair<int, std::pair<Graph::MapNodeIndicator, std::shared_ptr<Geography::LatLng const>>> previous_info = entities->read_dummy_by_id(dummy_id)->find_previous_fixed_position(share_phase);
					time_t previous_time_limit = time_manager->find_phase_of_time(share_phase) - time_manager->find_phase_of_time(previous_info.first) - entities->read_dummy_by_id(dummy_id)->get_pause_time(previous_info.first);
					//共有場所に到達可能ならその位置を設定し，到達不能ならばもう一度別のフェーズを検討
					if (map->is_reachable(previous_info.second.first, share_position.first, requirement->average_speed_of_dummy, previous_time_limit)) {
						break;
					}else {
						goto ONCE_AGAIN;
					}
				}
				//生成中ダミーの既に停止位置が決定しているフェーズよりも共有フェーズが小さい場合
				else if (entities->read_dummy_by_id(dummy_id)->find_next_fixed_position(0).first >= share_phase)
				{
					std::pair<int, std::pair<Graph::MapNodeIndicator, std::shared_ptr<Geography::LatLng const>>> next_info = entities->read_dummy_by_id(dummy_id)->find_next_fixed_position(0);
					time_t next_time_limit = time_manager->find_phase_of_time(next_info.first) - time_manager->find_phase_of_time(share_phase) - entities->read_dummy_by_id(dummy_id)->get_pause_time(share_phase);
					if (map->is_reachable(share_position.first, next_info.second.first, requirement->average_speed_of_dummy, next_time_limit)) {
						break;
					}
					else {
						goto ONCE_AGAIN;
					}
				}
				//生成中ダミーの既に停止位置が決定しているフェーズの間にある場合
				else
				{
					std::pair<int, std::pair<Graph::MapNodeIndicator, std::shared_ptr<Geography::LatLng const>>> previous_info = entities->read_dummy_by_id(dummy_id)->find_previous_fixed_position(share_phase);
					std::pair<int, std::pair<Graph::MapNodeIndicator, std::shared_ptr<Geography::LatLng const>>> next_info = entities->read_dummy_by_id(dummy_id)->find_next_fixed_position(share_phase);
					//time_limitはpreviousでの停止時間を考慮しなければならないことに注意
					time_t previous_time_limit = time_manager->find_phase_of_time(share_phase) - time_manager->find_phase_of_time(previous_info.first) - entities->read_dummy_by_id(dummy_id)->get_pause_time(previous_info.first);
					time_t next_time_limit = time_manager->find_phase_of_time(next_info.first) - time_manager->find_phase_of_time(share_phase) -  entities->read_dummy_by_id(dummy_id)->get_pause_time(share_phase);

					if (map->is_reachable(previous_info.second.first, share_position.first, requirement->average_speed_of_dummy, previous_time_limit)
						&& map->is_reachable(share_position.first, next_info.second.first, requirement->average_speed_of_dummy, next_time_limit)) {
						break;
					}
					else {
						goto ONCE_AGAIN;
					}
				}

				//Dmincross = += 1;
				//生成中のダミーの交差回数 += 1;
				entities->get_dummy_by_id(dummy_id)->set_crossing_position_of_phase(share_phase, share_position.first, *share_position.second);
				int target_dummy_cross_count = target->get_cross_count();
				target_dummy_cross_count++;//このやり方でtargetの交差回数の合計を数えられる？
				break;
					
				ONCE_AGAIN:
					continue;
			}
			//ユーザ及び生成済みダミーの平均交差回数よりも，生成中ダミーの交差回数が多くなるまで共有地点を設定する
			if(entities->get_dummy_by_id(dummy_id)->get_cross_count() > entities->get_all_entities_total_crossing_count() / dummy_id) break;
		}
	}


	/*
	///<summary>
	/// 生成中ダミー(k番目)の移動経路の決定
	///</summary>
	void KatoMethod_UserChange::decide_destination_on_the_way(int dummy_id)
	{
		//生成中ダミーの既に決まっている中で最初の地点を取得
		int dest_phase = entities->get_dummy_by_id(dummy_id)->find_next_fixed_position(0).first;
		std::pair<int, std::pair<Graph::MapNodeIndicator, std::shared_ptr<Geography::LatLng const>>> dest_position = entities->get_dummy_by_id(dummy_id)->find_next_fixed_position(0);

		//------------------------------------↓初期位置の決定↓------------------------------------------------------//
		time_t init_pause_time = 0;
		time_t init_arrive_time = time_manager->time_of_phase(0);
		
		
		//生成中ダミーのプランの中で，一番最初の場所から0秒までの範囲(最大停止時間を考慮)で到着できるPOIを取得
		//一旦リストで取得してから，その中からランダムで選ぶ方が良い
		
		std::pair<int, std::pair<Graph::MapNodeIndicator, std::shared_ptr<Geography::LatLng const>>> init_position = entities->get_dummy_by_id(dummy_id)->find_next_fixed_position(0);// getpauseposition;
		
		
		
		//PPoutに<position, start, pauseのinit>を追加;
		entities->get_dummy_by_id(dummy_id)->set_position_of_phase(0, init_position.second.first, *init_position.second.second);

		//------------------------------------↑初期位置の決定↑------------------------------------------------------//
		

		//初期位置以降の停止地点の決定
		int phase_id = 1;
		while (phase_id <= time_manager->phase_count())
		{
			//連続で停止位置が決まっている場合はskip
			if (entities->get_dummy_by_id(dummy_id)->find_next_fixed_position((phase_id)-1).first == phase_id) { break; }
			else {
				//position(phase_id-1)→position(position_id)に到達可能ならば，phaseにはintersectionを追加し，そうでないなら途中停止位置を設定
				time_t time_limit = 0;
				//intersectionを追加
				if (map->is_reachable(entities->get_dummy_by_id(dummy_id)->find_previous_fixed_position(dest_position.first).second.first, dest_position.second.first, requirement->average_speed_of_dummy, requirement->max_pause_time)) {
					//mapから最短路のpathを取ってくる！！
				}
				//途中停止を設定
				else {
					//phase_id番目の停止時間を決定
					time_t pause_time = std::rand() % (requirement->min_pause_time + requirement->max_pause_time + 1.0);
					entities->get_dummy_by_id(dummy_id)->set_pause_time(phase_id, pause_time);

					//positioni-1からpositioni番目へ到達可能なPOIからひとつランダムで取得
					Geography::LatLng position = get_pause_position;

					entities->get_dummy_by_id(dummy_id)->set_position_of_phase(phase_id, position);


				}
			}

			//途中目的地を停止地点として決定
			

		}




	}*/

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
