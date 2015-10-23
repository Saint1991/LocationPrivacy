#include "stdafx.h"
#include "KatoMethod_UserChange.h"

namespace Method
{
	
	///<summary>
	/// コンストラクタ
	/// これにSimulatorで作成した各種入力への参照を渡す
	///</summary>
	KatoMethod_UserChange::KatoMethod_UserChange(std::shared_ptr<Map::BasicDbMap const> map, std::shared_ptr<Entity::PauseMobileEntity<Geography::LatLng>> user, std::shared_ptr<Requirement::KatoMethodRequirement const> requirement, std::shared_ptr<Time::TimeSlotManager> time_manager, std::shared_ptr<Time::TimeSlotManager> revise_time_manager)
		: Framework::IProposedMethod<Map::BasicDbMap, Entity::PauseMobileEntity<Geography::LatLng>, Entity::PauseMobileEntity<Geography::LatLng>, Requirement::KatoMethodRequirement>(map, user, requirement, time_manager),grid_list(std::vector<Grid>(time_manager->phase_count())),creating_dummy(nullptr),revise_time_manager(revise_time_manager)
	{
		/*
		for (int i = 0; i < revise_time_manager->phase_count(); i++)
		{
			std::shared_ptr<std::vector<time_t>> timesslot = revise_time_manager->time_of_phase(i);
		}
		*/
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
	/// Rectangleに含まれるPOIのリストを取得
	///</summary>
	std::vector<std::shared_ptr<Map::BasicPoi const>> KatoMethod_UserChange::candidate_pois_list(const Graph::Rectangle<Geography::LatLng>& boundary) {
		std::vector<std::shared_ptr<Map::BasicPoi const>> candidate_pois_list = map->find_pois_within_boundary(boundary);
		std::random_device device;
		std::mt19937_64 generator(device());
		std::shuffle(candidate_pois_list.begin(), candidate_pois_list.end(), generator);

		return candidate_pois_list;
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
		
		creating_dummy->set_position_of_phase(base_phase, Graph::MapNodeIndicator(base_poi_id),base_point);
		creating_dummy->set_random_speed(base_phase,requirement->average_speed_of_dummy, requirement->speed_range_of_dummy);
		creating_dummy->set_pause_time(base_phase, entities->get_user()->get_pause_time(base_phase));
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
				if (creating_dummy->read_position_of_phase(time_manager->phase_count()) <= share_phase)
				{
					std::pair<int, std::pair<Graph::MapNodeIndicator, std::shared_ptr<Geography::LatLng const>>> previous_info = creating_dummy->find_previous_fixed_position(share_phase);
					time_t previous_time_limit = time_manager->find_phase_of_time(share_phase) - time_manager->find_phase_of_time(previous_info.first) - creating_dummy->get_pause_time(previous_info.first);
					//共有場所に到達可能ならその位置を設定し，到達不能ならばもう一度別のフェーズを検討
					if (map->is_reachable(previous_info.second.first, share_position.first, requirement->average_speed_of_dummy, previous_time_limit)) {
						break;
					}else {
						goto ONCE_AGAIN;
					}
				}
				//生成中ダミーの既に停止位置が決定しているフェーズよりも共有フェーズが小さい場合
				else if (creating_dummy->find_next_fixed_position(0).first >= share_phase)
				{
					std::pair<int, std::pair<Graph::MapNodeIndicator, std::shared_ptr<Geography::LatLng const>>> next_info = creating_dummy->find_next_fixed_position(0);
					time_t next_time_limit = time_manager->find_phase_of_time(next_info.first) - time_manager->find_phase_of_time(share_phase) - creating_dummy->get_pause_time(share_phase);
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
					std::pair<int, std::pair<Graph::MapNodeIndicator, std::shared_ptr<Geography::LatLng const>>> previous_info = creating_dummy->find_previous_fixed_position(share_phase);
					std::pair<int, std::pair<Graph::MapNodeIndicator, std::shared_ptr<Geography::LatLng const>>> next_info = creating_dummy->find_next_fixed_position(share_phase);
					//time_limitはpreviousでの停止時間を考慮しなければならないことに注意
					time_t previous_time_limit = time_manager->find_phase_of_time(share_phase) - time_manager->find_phase_of_time(previous_info.first) - creating_dummy->get_pause_time(previous_info.first);
					time_t next_time_limit = time_manager->find_phase_of_time(next_info.first) - time_manager->find_phase_of_time(share_phase) -  creating_dummy->get_pause_time(share_phase);

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
				creating_dummy->set_crossing_position_of_phase(share_phase, share_position.first, *share_position.second);
				creating_dummy->set_random_speed(share_phase, requirement->min_pause_time, requirement->max_pause_time);
				creating_dummy->set_pause_time(share_phase, entities->get_user()->get_pause_time(share_phase));
				int target_dummy_cross_count = target->get_cross_count();
				target_dummy_cross_count++;//このやり方でtargetの交差回数の合計を数えられる？
				break;
					
				ONCE_AGAIN:
					continue;
			}
			//ユーザ及び生成済みダミーの平均交差回数よりも，生成中ダミーの交差回数が多くなるまで共有地点を設定する
			if(creating_dummy->get_cross_count() > entities->get_all_entities_total_crossing_count() / dummy_id) break;
		}
	}


	/*
	///<summary>
	/// 生成中ダミー(k番目)の移動経路の決定
	///</summary>
	void KatoMethod_UserChange::decide_destination_on_the_way(int dummy_id)
	{
		Graph::Rectangle<Geography::LatLng> rect(1.0, 1.0, 1.0, 1.0);
		
		//生成中ダミーの既に決まっている中で最初の地点を取得
		std::pair<int, std::pair<Graph::MapNodeIndicator, std::shared_ptr<Geography::LatLng const>>> dest_position = creating_dummy->find_next_fixed_position(0);

		//------------------------------------↓初期位置の決定↓------------------------------------------------------//

		creating_dummy->set_pause_time(0, 0);//初期phaseの停止時間
		creating_dummy->set_random_speed(0, requirement->average_speed_of_dummy, requirement->speed_range_of_dummy);//初期speedの決定

		//生成中ダミーのプランの中で，一番最初の場所から0秒までの範囲(最大停止時間を考慮)で到着できるPOIを取得
		//一旦リストで取得してから，その中からランダムで選ぶ方が良い
		double distance = creating_dummy->get_speed(0)*(time_manager->time_of_phase(dest_position.first) - time_manager->time_of_phase(0));

		std::vector<std::shared_ptr<Map::BasicPoi const>> init_pois_list = candidate_pois_list(範囲);
		std::vector<std::shared_ptr<Map::BasicPoi const>>::const_iterator init_poi = init_pois_list.begin();
		time_t init_time_limit = time_manager->time_of_phase(dest_position.first) - time_manager->time_of_phase(0) - creating_dummy->get_pause_time(0);

		while (map->is_reachable(Graph::MapNodeIndicator((*init_poi)->get_id()), dest_position.second.first, creating_dummy->get_speed(0), init_time_limit)==false) {
			init_poi++;
		}

		//PPoutに<position, start, pauseのinit>を追加;
		creating_dummy->set_position_of_phase(0, (*init_poi)->get_id(), (*init_poi)->data->get_position());

		//------------------------------------↑初期位置の決定↑------------------------------------------------------//
		

		//初期位置以降の停止地点の決定
		int phase_id = 1;
		while (phase_id < creating_dummy->find_previous_fixed_position(time_manager->phase_count()).first)
		{
			if (creating_dummy->find_next_fixed_position((phase_id)-1).first != phase_id) 
			{
				std::pair<int, std::pair<Graph::MapNodeIndicator, std::shared_ptr<Geography::LatLng const>>> already_decided_position = creating_dummy->find_previous_fixed_position(dest_position.first);
				time_t time_limit = time_manager->time_of_phase(dest_position.first)-time_manager->time_of_phase(already_decided_position.first)-requirement->max_pause_time;
				//position(already_deceded)→position(dest_id)に到達可能ならば，phaseにはintersectionを追加し，そうでないなら途中停止位置を設定
				//途中停止位置を設定
				while (map->is_reachable(already_decided_position.second.first, dest_position.second.first, creating_dummy->get_speed(already_decided_position.first), time_limit)) {
					//phase_id番目の停止時間と移動速度を決定
					creating_dummy->set_random_pause_time(phase_id,requirement->min_pause_time,requirement->max_pause_time);
					creating_dummy->set_random_speed(phase_id,requirement->average_speed_of_dummy,requirement->speed_range_of_dummy);

					//position(phase_id)からposition(dest)番目へ到達可能なPOIからひとつランダムで取得
					//目的地に近づく方向を考慮しなければならないことに注意
					double top = std::abs(already_decided_position.second.second->lat() - dest_position.second.second->lat());
					double left = already_decided_position.second.second->lng();
					double bottom = already_decided_position.second.second->lat();
					double right = std::abs(already_decided_position.second.second->lng() - dest_position.second.second->lng());
					Graph::Rectangle<Geography::LatLng> rect(top, left, bottom, right);
			
					std::vector<std::shared_ptr<Map::BasicPoi const>> on_the_way_pois_list = candidate_pois_list(rect);
					std::vector<std::shared_ptr<Map::BasicPoi const>>::const_iterator poi_on_the_way = on_the_way_pois_list.begin();
					time_t time_limit = time_manager->time_of_phase(dest_position.first) - time_manager->time_of_phase(phase_id) - creating_dummy->get_pause_time(phase_id);

					while (map->is_reachable(Graph::MapNodeIndicator((*poi_on_the_way)->get_id()), dest_position.second.first, creating_dummy->get_speed(phase_id), time_limit) == false) {
						poi_on_the_way++;
					}
					//停止位置のセット
					creating_dummy->set_position_of_phase(phase_id, (*poi_on_the_way)->get_id(),(*poi_on_the_way)->data->get_position());
					phase_id++;
					//決定点を更新
					already_decided_position = creating_dummy->find_previous_fixed_position(phase_id);
					time_limit = time_manager->time_of_phase(dest_position.first) - time_manager->time_of_phase(already_decided_position.first) - creating_dummy->get_pause_time(already_decided_position.first);
				}
				//intersectionを追加
				//次の停止地点が決まっているところまでintersectionを補完
				//mapから最短路のpathを取ってくる！intersectionの設定はどうすれば？
				const std::vector<Graph::MapNodeIndicator> path = map->get_shortest_path(already_decided_position.second.first, dest_position.second.first);
				std::vector<Graph::MapNodeIndicator>::const_iterator iter = path.begin();
				for (; phase_id != dest_position.first; phase_id++,iter++)
				{
					Geography::LatLng position =
						iter->type() == Graph::NodeType::POI ? map->get_static_poi(iter->id())->data->get_position() : *map->get_static_node(iter->id())->data;
					creating_dummy->set_position_of_phase(phase_id, *iter, position);
					creating_dummy->set_speed(phase_id,creating_dummy->get_speed(phase_id-1));
					creating_dummy->set_pause_time(phase_id, 0);
				}
			}
			//連続で停止位置が決まっている場合はskip
			else {
				phase_id++;
			}
						
			//途中目的地の更新
			dest_position = creating_dummy->find_next_fixed_position(phase_id);
			phase_id++;
		}
		//シュミレーション終了までの残りの停止位置を適当に決める
		phase_id++;//調整のためのインクリメント
		while (phase_id <= time_manager->phase_count())
		{
			creating_dummy->set_random_pause_time(phase_id, requirement->min_pause_time, requirement->max_pause_time);
			creating_dummy->set_random_speed(phase_id, requirement->average_speed_of_dummy, requirement->speed_range_of_dummy);
			std::pair<int, std::pair<Graph::MapNodeIndicator, std::shared_ptr<Geography::LatLng const>>> already_decided_position = dest_position;
			//positioni-1からpositioni番目へ到達可能なPOIからひとつランダムで取得
			std::vector<std::shared_ptr<Map::BasicPoi const>> rest_pois_list = map->find_pois_within_boundary(範囲);
			std::vector<std::shared_ptr<Map::BasicPoi const>>::const_iterator rest_poi = rest_pois_list.begin();
			//time_limitは速度と時間使って距離が必要
			double distance = creating_dummy->get_speed(phase_id)*(time_manager->time_of_phase(phase_id + 1) - time_manager->time_of_phase(phase_id));
			time_t time_limit = ;

			while (map->is_reachable(already_decided_position.second.first, Graph::MapNodeIndicator((*rest_poi)->get_id()), creating_dummy->get_speed(phase_id), time_limit) == false) {
				rest_poi++;
			}
			creating_dummy->set_position_of_phase(phase_id, (*rest_poi)->get_id(), (*rest_poi)->data->get_position());
			//決定位置の更新
			already_decided_position = creating_dummy->find_next_fixed_position(phase_id);
			phase_id++;
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
	void KatoMethod_UserChange::revise_dummy_pause_time(int phase_id)
	{
		//前の値の保持
		time_t previous_pause_time = creating_dummy->get_pause_time(phase_id);
		time_t previous_arrive_time = time_manager->time_of_phase(phase_id + 1);

		if (std::abs(time_to_change) > requirement->max_variation_of_pause_time)
		{
			time_t new_pause_time = time_to_change > 0 ? previous_pause_time + requirement->max_variation_of_pause_time : previous_pause_time - requirement->max_variation_of_pause_time;
			creating_dummy->set_pause_time(phase_id, new_pause_time);
		}
		else
		{
			creating_dummy->set_pause_time(phase_id, previous_pause_time + time_to_change);
		}

		if (requirement->max_pause_time < time_manager->time_of_phase(phase_id)) creating_dummy->set_pause_time(phase_id, requirement->max_pause_time);
		if (requirement->min_pause_time > time_manager->time_of_phase(phase_id)) creating_dummy->set_pause_time(phase_id, requirement->min_pause_time);
		
		//停止時間の変化量を求める
		time_t variation_of_pause_time = creating_dummy->get_pause_time(phase_id) - previous_pause_time;

		for (int i = phase_id + 1; i <= time_manager->phase_count(); i++)
		{
			//revise_time_manager->time_of_phase(i) += variation_of_pause_time;
		}
		if (time_manager->time_of_phase(phase_id+1) == previous_arrive_time + time_to_change) return;

		time_to_change -= variation_of_pause_time;//この行は謎笑
	}

	/*
	///<summary>
	/// ダミーの移動経路の修正
	///</summary>
	void KatoMethod_UserChange::revise_dummy_trajectory(int phase_id)
	{
		time_t previous_arrive_time = time_manager->time_of_phase(phase_id + 1);
		time_t tempT = 10000000000000000000;

		while (全てのトラジェクトリをチェックし終えるまで) 
		{
			phase_idのときの存在地点;
			time_manager->time_of_phase(phase_id + 1) = time_manager->time_of_phase(phase_id) + creating_dummy->get_pause_time(phase_id) + distance_of_Tri / creating_dummy->get_speed(phase_id);

		}

	}*/


	///<summary>
	/// ダミーの行動速度の修正
	///</summary>
	void KatoMethod_UserChange::revise_dummy_speed(int phase_id)
	{
		time_t previous_arrive_time = time_manager->time_of_phase(phase_id + 1);
		double previous_speed = creating_dummy->get_speed(phase_id);
		double distance = map->shortest_distance(creating_dummy->read_node_pos_info_of_phase(phase_id).first, creating_dummy->read_node_pos_info_of_phase(phase_id).first);
		time_t time = distance / (time_manager->time_of_phase(phase_id + 1)+time_to_change-time_manager->time_of_phase(phase_id));
		creating_dummy->set_speed(phase_id, time);

		if (std::abs(creating_dummy->get_speed(phase_id)-previous_speed) > requirement->max_variation_of_speed)
		{
			double new_speed = creating_dummy->get_speed(phase_id) - previous_speed > 0 ? previous_speed + requirement->max_variation_of_speed : previous_speed - requirement->max_variation_of_speed;
			creating_dummy->set_pause_time(phase_id, new_speed);
		}
		double max_speed = requirement->average_speed_of_dummy + 0.5* requirement->speed_range_of_dummy;
		double min_speed = requirement->average_speed_of_dummy - 0.5* requirement->speed_range_of_dummy;
		if (max_speed < creating_dummy->get_speed(phase_id)) creating_dummy->set_speed(phase_id, max_speed);
		if (min_speed > creating_dummy->get_speed(phase_id)) creating_dummy->set_speed(phase_id, min_speed);

		//time_manager->time_of_phase(phase_id + 1) = time_manager->time_of_phase(phase_id) + creating_dummy->get_pause_time(phase_id) + (time_t)(distance / creating_dummy->get_speed(phase_id));
		time_t variation_of_arrive_time = time_manager->time_of_phase(phase_id + 1) - previous_arrive_time;
		
		for (int i = phase_id + 1; i <= time_manager->phase_count(); i++)
		{
			//revise_time_manager->time_of_phase(i) += variation_of_pause_time;
		}
		if (time_manager->time_of_phase(phase_id + 1) == previous_arrive_time + time_to_change) return;

	}


	///<summary>
	/// ダミーの停止位置の修正
	///</summary>
	void KatoMethod_UserChange::revise_dummy_pose_position(int phase_id)
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
		
		for (size_t dummy_id = 1; dummy_id <= entities->get_dummy_count(); dummy_id++)
		{	
			creating_dummy = entities->get_dummy_by_id(dummy_id);
			
			//decide_base_positions_and_arrive_time(dummy_id);// 生成中ダミー(k番目)の基準地点および基準地点到着時間の決定
			//decide_share_positions_and_arrive_time(dummy_id);// 生成中ダミー(k番目)の共有地点および共有地点到着時間の決定
			//decide_dummy_path(dummy_id);// 生成中ダミー(k番目)の移動経路の決定
			
		}
	}
	
	
	///<summary>
	/// ここが提案手法の核になる部分.ダミーの行動を修正する
	///</summary>
	void KatoMethod_UserChange::revise_dummy_positions()
	{
		int phase_id = 1;
		time_t time_to_change = 0;// (修正後の)time_manager->time_of_phase(phase_id) - (修正前の)time_manager->time_of_phase(phase_id);
		//revise_dummy_pause_time();

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
