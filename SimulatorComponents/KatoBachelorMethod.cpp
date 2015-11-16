#include "stdafx.h"
#include "KatoBachelorMethod.h"

namespace Method
{

	///<summary>
	/// コンストラクタ
	/// これにSimulatorで作成した各種入力への参照を渡す
	///</summary>
	KatoBachelorMethod::KatoBachelorMethod(std::shared_ptr<Map::BasicDbMap const> map, std::shared_ptr<Entity::PauseMobileEntity<Geography::LatLng>> user, std::shared_ptr<Requirement::KatoMethodRequirement const> requirement, std::shared_ptr<Time::TimeSlotManager> time_manager)
		: Framework::IProposedMethod<Map::BasicDbMap, Entity::PauseMobileEntity<Geography::LatLng>, Entity::PauseMobileEntity<Geography::LatLng>, Requirement::KatoMethodRequirement, Geography::LatLng, Graph::Trajectory<Geography::LatLng>>(map, user, requirement, time_manager),
		grid_list(std::vector<Grid>((time_manager->phase_count() / requirement->interval_of_base_phase))),
		creating_dummy(nullptr)
	{
	}


	///<summary>
	/// デストラクタ
	///</summary>
	KatoBachelorMethod::~KatoBachelorMethod()
	{
	}


	///<summary>
	/// T[s]ごとのグリッド領域を作成
	/// grid_lengthはグリッド全体の長さ
	///</summary>
	std::vector <Graph::Rectangle<Geography::LatLng>> KatoBachelorMethod::make_grid(double grid_area, const Geography::LatLng& center, int cell_num_on_side)
	{
		double grid_length = std::sqrt(grid_area);//grid全体の一辺の長さ．匿名領域の√
		double cell_length = grid_length / cell_num_on_side;//セル一つ分の長方形の長さ

															//長さ(cell_length)を緯度経度の単位に変換
		double length_translated_lat = Geography::GeoCalculation::calc_translated_point(center, cell_length, M_PI_2 * 3).lat() - center.lat();
		double length_translated_lng = Geography::GeoCalculation::calc_translated_point(center, cell_length, 0).lng() - center.lng();

		//centerを中心としたgridを生成した時の，一番左上に相当するcellの四点の緯度経度
		double top = center.lat() + 1.5 * length_translated_lat;
		double left = center.lng() - 1.5 * length_translated_lng;
		double bottom = center.lat() + 0.5 * length_translated_lat;
		double right = center.lng() - 0.5 * length_translated_lng;

		std::vector<Graph::Rectangle<Geography::LatLng>> grid_list;//グリッド全体を管理するリスト

		double base_left = left;//左上の正方形のleftをループの際の基準とする
		double base_right = right;//左上の正方形のrightをループの際の基準とする

		for (int i = 0; i < cell_num_on_side; i++)
		{
			for (int j = 0; j < cell_num_on_side; j++)
			{
				grid_list.push_back(Graph::Rectangle<Geography::LatLng>(top, left, bottom, right));
				right += length_translated_lng;
				left += length_translated_lng;
			}

			top -= length_translated_lat;
			bottom -= length_translated_lat;
			left = base_left;
			right = base_right;
		}

		return grid_list;
	}


	///<summary>
	/// グリッドテーブルのstartからendまでのエンティティの合計を取得
	///</summary>
	std::vector<int> KatoBachelorMethod::get_total_num_of_each_cell_at_interval_phase(std::vector<std::vector<int>>& entities_num_table, int start, int end) {
		std::vector<int> total_entity_num_interval_phase(CELL_NUM_ON_SIDE*CELL_NUM_ON_SIDE, 0);

		for (int cell_id = 0; cell_id < CELL_NUM_ON_SIDE*CELL_NUM_ON_SIDE; cell_id++)
		{
			int temp = 0;
			for (int interval_of_base_phase = start; interval_of_base_phase < end; interval_of_base_phase++)
			{
				temp += entities_num_table.at(cell_id).at(interval_of_base_phase);
			}
			total_entity_num_interval_phase.at(cell_id) = temp;
		}
		return total_entity_num_interval_phase;
	}

	///<summary>
	/// グリッドテーブルの各グリッドごとの全フェーズのエンティティの合計を取得
	///</summary>
	std::vector<int> KatoBachelorMethod::get_total_num_of_each_cell_at_all_phase(std::vector<std::vector<int>>& entities_num_table) {
		std::vector<int> total_entity_num_all_phase = get_total_num_of_each_cell_at_interval_phase(entities_num_table, 0, entities_num_table.at(0).size());
		return total_entity_num_all_phase;
	}


	///<summary>
	/// start_phaseからend_phaseまでのcell_idのうち，エンティティ数を昇順に並べ替えたlistで取得します
	/// pairは(cell_id,enntities_num)
	///</summary>
	std::list<std::pair<int, int>> KatoBachelorMethod::get_cell_id_list_order_by_entities_num(std::vector<int>& total_entity_num) const
	{
		int cell_id = 0;
		std::list<std::pair<int, int>> cell_list_of_min_entities_num;
		for (std::vector<int>::const_iterator iter = total_entity_num.begin(); iter != total_entity_num.end(); iter++, cell_id++) {
			cell_list_of_min_entities_num.push_back(std::make_pair(cell_id, *iter));
		}
		cell_list_of_min_entities_num.sort([](const std::pair<int, int>& cell_id1, const std::pair<int, int>& cell_id2) {
			return cell_id1.second < cell_id2.second;
		});

		return cell_list_of_min_entities_num;
	}


	///<summary>
	/// start_phaseからend_phaseまでで，min_cell_idのセルでエンティティ数が昇順となるbase_phaseをlistで取得
	/// pairは(base_phase,entities_num)
	///</summary>
	std::list<std::pair<int, int>> KatoBachelorMethod::get_phase_of_min_cell_id_list_ordered_by_entities_num(std::vector<std::vector<int>>& entities_num_table, int min_cell_id, int start, int end) const
	{
		std::list<std::pair<int, int>> base_phase_list_of_min_entities_num;
		for (int base_phase_id = start; base_phase_id < end; base_phase_id++) {
			base_phase_list_of_min_entities_num.push_back(std::make_pair(base_phase_id, entities_num_table.at(base_phase_id).at(min_cell_id)));
		}

		base_phase_list_of_min_entities_num.sort([](const std::pair<int, int>& base_phase_id1, const std::pair<int, int>& base_phase_id2) {
			return base_phase_id1.second < base_phase_id2.second;
		});
		return base_phase_list_of_min_entities_num;
	}


	///<summary>
	/// Rectangleに含まれるPOIのリストを取得
	/// boundary内にpoiが見つからない場合は範囲を広げて再探索
	///</summary>
	std::vector<std::shared_ptr<Map::BasicPoi const>> KatoBachelorMethod::get_candidate_pois_list(Graph::Rectangle<Geography::LatLng>& boundary) {
		std::vector<std::shared_ptr<Map::BasicPoi const>> candidate_pois_list = map->find_pois_within_boundary(boundary);
		double length = 0.005;
		//もし範囲内のPOIが見つからなかったら，範囲を広げて再計算
		if (candidate_pois_list.size() == 0) {
			while (candidate_pois_list.size() == 0) {
				length += 0.001;
				boundary.top += 0.5 * length;
				boundary.left -= 0.5 * length;
				boundary.bottom -= 0.5 * length;
				boundary.right += 0.5 * length;
				candidate_pois_list = map->find_pois_within_boundary(boundary);
			}
		}

		std::random_device device;
		std::mt19937_64 generator(device());
		std::shuffle(candidate_pois_list.begin(), candidate_pois_list.end(), generator);

		return candidate_pois_list;
	}



	///<summary>
	/// position間のパスを線形補間する．
	/// sourceには決定点,destinationには目的地，source_phaseには，決定しているphaseを入力すること.
	/// sourceには既に停止地点は入力されている
	/// dest_rest_timeをポインタ引数にすることで，複数の返り値を実現.次の到着時間の余りを返す．
	///</summary>
	void KatoBachelorMethod::linear_interpolation_of_path_between_positions(const Graph::MapNodeIndicator& source, const Graph::MapNodeIndicator& destination, int *phase_id, int *dest_rest_time) {
			//全体の停止時間から，前回の到着分を差し引いた停止時間を引いた時間
		int rest_pause_time = creating_dummy->get_pause_time(*phase_id) - *dest_rest_time;

		//残りの停止時間をphaseに換算し，phase分埋める．
		div_t variable_of_converted_pause_time_to_phase = std::div(rest_pause_time, requirement->service_interval);

		//停止時間分，各phaseに停止場所と移動速度(0)を登録
		for (int i = 0; i < variable_of_converted_pause_time_to_phase.quot; i++)
		{
			(*phase_id)++;
			creating_dummy->set_position_of_phase(*phase_id, source, map->get_static_poi(source.id())->data->get_position());
			creating_dummy->set_speed(*phase_id, 0);
		}

		std::vector<Graph::MapNodeIndicator> shortests_path_between_pois = map->get_shortest_path(source, destination);
		std::vector<Graph::MapNodeIndicator>::iterator path_iter = shortests_path_between_pois.begin();//pathを検索するためのindex

		double pause_position_speed = creating_dummy->get_speed(*phase_id - variable_of_converted_pause_time_to_phase.quot);//停止位置の出発速度を保持しておく

		//sourceからの距離
		//最初だけ停止時間をphaseに換算した時の余りをtimeとし，それ以外はservice_intervalをtimeとして，現在地から求めたい地点のdistanceを計算
		//速度はphaseで埋める前を参照しなければならないことに注意
		int now_time = requirement->service_interval - variable_of_converted_pause_time_to_phase.rem;
		int total_time_from_source_to_destination = map->calc_necessary_time(source, destination, pause_position_speed);
		Graph::MapNodeIndicator nearest_position = source;
		//pathを作成．場所は一番近いintersection同士で線形補間する．MapNodeIndicatorのTypeはINVALIDとする．
		while (now_time < total_time_from_source_to_destination)
		{
			//最初は停止時間をphaseに換算したときの余り分をdistanceとして，最短路の中で一番近いintersectionを探し，線形補間する．
			//double total_path_length = map->shortest_distance(source, *path_iter);
			while (now_time > map->calc_necessary_time(source, *path_iter, pause_position_speed))
			{
				nearest_position = *path_iter;
				path_iter++;
			}

			int distance = now_time * pause_position_speed;
			int distance_between_nearest_intersection_and_arrive_position = distance - map->shortest_distance(source, nearest_position);
			Geography::LatLng nearest_latlng
				= nearest_position.type() == Graph::NodeType::POI ? map->get_static_poi(nearest_position.id())->data->get_position() : *map->get_static_node(nearest_position.id())->data;
			Geography::LatLng next_nearest_latlang
				= (*path_iter).type() == Graph::NodeType::POI ? map->get_static_poi((*path_iter).id())->data->get_position() : *map->get_static_node((*path_iter).id())->data;
			double angle = Geography::GeoCalculation::lambert_azimuth_angle(nearest_latlng, next_nearest_latlang);

			Geography::LatLng arrive_position = Geography::GeoCalculation::calc_translated_point(nearest_latlng, distance_between_nearest_intersection_and_arrive_position, angle);

			(*phase_id)++;
			creating_dummy->set_position_of_phase(*phase_id, Graph::MapNodeIndicator(Graph::NodeType::OTHERS, Graph::NodeType::OTHERS), arrive_position);
			creating_dummy->set_speed(*phase_id, pause_position_speed);

			now_time += requirement->service_interval;
		}

		//destinationのところまで補完できたら，rest_timeを保持しておく！
		int time_between_arrive_position_and_dest_position = now_time - total_time_from_source_to_destination;
		*dest_rest_time
			= time_between_arrive_position_and_dest_position == requirement->service_interval ? 0 : time_between_arrive_position_and_dest_position;
		//目的地の登録
		//speedは別途設定のため不要
		(*phase_id)++;
		creating_dummy->set_position_of_phase(*phase_id, destination, map->get_static_poi(destination.id())->data->get_position());
	}




	///<summary>
	/// 生成中ダミー(k番目)の基準地点および基準地点到着時間の決定
	///</summary>
	void KatoBachelorMethod::decide_base_positions_and_arrive_time(int dummy_id)
	{
		//phaseはもし匿名領域をphase0にも設定したい場合
		//1:初期値を0にする
		//2:grid_listとentities_num_tableのvectorの個数を(time_manager->phase_count()/requirement->interval_of_base_phase)+1にする
		//3:real_phaseをbase_phase+1でなはく，base_phaseにする
		int phase = requirement->interval_of_base_phase;
		const int GRID_TOTAL_NUM = CELL_NUM_ON_SIDE*CELL_NUM_ON_SIDE;//グリッドの数

																	 //各グリッドの各フェイズにおけるentitiesの数を記憶するためのtable(動的配列)の作成
		std::vector<std::vector<int>> entities_num_table(GRID_TOTAL_NUM, std::vector<int>((time_manager->phase_count() / requirement->interval_of_base_phase), 0));

		//あるphaseにおける各セルに存在するユーザ及び生成済みダミーの移動経路(停止地点)の数
		//横がセルのid，縦がphaseを表す動的２次元配列で記憶
		//phase_intervalの間隔で記録していく
		//k-2個目までのtableを作っておいて，k-1個目を＋１して更新すればより効率が良い
		std::shared_ptr<Geography::LatLng const> center = entities->get_average_position_of_phase(phase);//中心位置を求める
		Grid grid = make_grid(requirement->required_anonymous_area, *center, CELL_NUM_ON_SIDE);//phaseごとにグリッドを作成
		int grid_list_id = 0;
		int cell_id = 0;//セルのid
		int end_phase = time_manager->phase_count();

		while (phase <= end_phase)
		{
			cell_id = 0;//セルのidのリセット
			center = entities->get_average_position_of_phase(phase);//中心位置を求める
			grid = make_grid(requirement->required_anonymous_area, *center, CELL_NUM_ON_SIDE);//phaseごとにグリッドを作成
			grid_list.at(grid_list_id) = grid;//あるphaseのGrid.grud_list_idで何回目かのgrid生成かを記録する

											  //あるphaseの全てのセルの，エンティティ数を計算(表の列を計算することに相当)
			for (std::vector<Graph::Rectangle<Geography::LatLng>>::iterator iter = grid_list.at(grid_list_id).begin(); iter != grid_list.at(grid_list_id).end(); iter++, cell_id++)
			{
				entities_num_table.at(cell_id).at(grid_list_id) = entities->get_entity_count_within_boundary(phase, *iter);
			}
			phase += requirement->interval_of_base_phase;//phaseの更新
			grid_list_id++;//grid_list_idのインクリメント
		}

		//周期をphaseで設定し，その周期ベースで匿名領域確保のための地点を作成
		int start_of_cycle = 0;

		//終了条件はentities_tableのsizeになるまで!
		for (int end_of_cycle = requirement->cycle_of_interval_of_base_num; end_of_cycle <= entities_num_table.at(0).size();
		start_of_cycle += requirement->cycle_of_interval_of_base_num, end_of_cycle += requirement->cycle_of_interval_of_base_num)
		{
			//各セルのstart~endのエンティティの合計(表の行の和を計算していることに相当)
			std::vector<int> total_entity_num_at_interval_phase = get_total_num_of_each_cell_at_interval_phase(entities_num_table, start_of_cycle, end_of_cycle);
			//start_phaseからend_phaseまでで，エンティティ数が昇順となるセルidをlistで求める．
			std::list<std::pair<int, int>> min_cell_id_list = get_cell_id_list_order_by_entities_num(total_entity_num_at_interval_phase);

			//min_cell_idのセルでエンティティ数が昇順となるbase_phaseをlistで取得
			Math::Probability generator;
			int base_phase = generator.uniform_distribution(start_of_cycle, end_of_cycle - 1);
			if (end_of_cycle == 4) base_phase = 2;
			
			//base_phaseはinterval_of_base_phaseの中の数なので，実際のphaseは別
			//grid_listのみbase_phaseを使う！
			int real_phase = (base_phase + 1) * requirement->interval_of_base_phase;

			//取得したcell_id,phaseにおける停止地点を取得
			//一様分布でランダム取得(見つからなかった場合の広げる大きさは考慮したほうが良いかもしれない)
			Graph::Rectangle<Geography::LatLng> cell = grid_list.at(base_phase).at(min_cell_id_list.front().first);
			std::vector<std::shared_ptr<Map::BasicPoi const>> poi_within_base_point_grid = get_candidate_pois_list(cell);
			std::vector<std::shared_ptr<Map::BasicPoi const>>::const_iterator poi = poi_within_base_point_grid.begin();

			//一番最初のみ到達可能性を考慮せずに停止地点を決定する．
			if (creating_dummy->find_previous_fixed_position(time_manager->phase_count()).first == INVALID) {
				creating_dummy->set_position_of_phase(real_phase, Graph::MapNodeIndicator((*poi)->get_id()), (*poi)->data->get_position());
				creating_dummy->set_random_speed(real_phase, requirement->average_speed_of_dummy, requirement->speed_range_of_dummy);
			}
			//二箇所目以降の基準地点は，以前の基準地点から到達可能性を調べたのちに決定する．
			else
			{
				std::pair<int, std::pair<Graph::MapNodeIndicator, std::shared_ptr<Geography::LatLng const>>> previous_base_info = creating_dummy->find_previous_fixed_position(real_phase);
				int base_time_limit = time_manager->time_of_phase(real_phase) - time_manager->time_of_phase(previous_base_info.first) - requirement->min_pause_time;

				//領域内のPOIを5回まで探索し，到達可能ならその点を採用
				//見つからない場合は違う領域(cell)を参照して再探索
				auto serach_poi = [&](int flag_id = 0) {
					for (int flag = 0; flag < 5; flag++) {
						if (map->is_reachable(previous_base_info.second.first, Graph::MapNodeIndicator((*poi)->get_id()), creating_dummy->get_speed(previous_base_info.first), base_time_limit)) {
							return flag_id + 1;
						}
						else {
							poi++;
						}
					}
					return flag_id;
				};

				for (std::list<std::pair<int, int>>::iterator min_cell_iter = min_cell_id_list.begin(); min_cell_iter != min_cell_id_list.end(); min_cell_iter++) {
					if (serach_poi() == 0) {
						cell = grid_list.at(base_phase).at((*min_cell_iter).first);
						poi_within_base_point_grid = get_candidate_pois_list(cell);
						poi = poi_within_base_point_grid.begin();
					}
					else {
						break;
					}
				}

				creating_dummy->set_position_of_phase(real_phase, Graph::MapNodeIndicator((*poi)->get_id()), (*poi)->data->get_position());
				creating_dummy->set_random_speed(real_phase, requirement->average_speed_of_dummy, requirement->speed_range_of_dummy);
			}
		}
	}




	///<summary>
	/// 生成中ダミー(k番目)の共有地点および共有地点到着時間の決定
	///</summary>
	void KatoBachelorMethod::decide_share_positions_and_arrive_time(int dummy_id)
	{
		//交差回数が少ないエンティティを優先的に交差対象にして繰り返す
		std::list<std::pair<Entity::entity_id, int>> entity_list_order_by_cross = entities->get_entity_id_list_order_by_cross_count();
		for (std::list<std::pair<Entity::entity_id, int>>::const_iterator iter = entity_list_order_by_cross.begin(); iter != entity_list_order_by_cross.end(); iter++)
		{

			//未生成のダミーのものはスキップ
			if (iter->first >= dummy_id) continue;

			//交差対象のID(交差回数最小)
			int cross_target = iter->first;
			int target_phase = INVALID;

			//BasicUserはDummyを継承しているのでポインタ代入は可能
			//targetが交差するユーザorダミー
			//target_phasesは交差が設定されていないphaseの一覧
			std::shared_ptr<Entity::PauseMobileEntity<Geography::LatLng>> target = cross_target == 0 ? entities->get_user() : entities->get_dummy_by_id(cross_target);
			std::vector<int> target_phases = target->find_cross_not_set_phases_of_poi();

			//基準地点に設定されているphaseは削除する．
			int interval_of_base_phase = requirement->interval_of_base_phase;
			auto iterNewEnd = std::remove_if(target_phases.begin(), target_phases.end(), [interval_of_base_phase](int phase) {
				return phase % interval_of_base_phase == 0;
			});
			target_phases.erase(iterNewEnd, target_phases.end());


			//交差未設定の時刻からランダムに共有地点設定を試みる
			std::random_device device;
			std::mt19937_64 generator(device());
			std::shuffle(target_phases.begin(), target_phases.end(), generator);

			//フェーズが0でない交差が未設定のフェーズに交差地点を設定する．
			for (std::vector<int>::const_iterator target_phase = target_phases.begin(); target_phase != target_phases.end(); target_phase++) {
				int share_phase = *target_phase;
				std::pair<Graph::MapNodeIndicator, std::shared_ptr<Geography::LatLng const>> share_position = target->read_node_pos_info_of_phase(share_phase);

				//生成中ダミーの既に停止位置が決定しているフェーズよりも共有フェーズが大きい場合
				if (creating_dummy->find_previous_fixed_position(time_manager->phase_count()).first <= share_phase)
				{
					std::pair<int, std::pair<Graph::MapNodeIndicator, std::shared_ptr<Geography::LatLng const>>> previous_info = creating_dummy->find_previous_fixed_position(time_manager->phase_count());
					int previous_time_limit = time_manager->time_of_phase(share_phase) - time_manager->time_of_phase(previous_info.first) - requirement->min_pause_time;

					//共有場所に到達可能ならその位置を設定し，到達不能ならばもう一度別のフェーズを検討
					if (previous_time_limit < 0 || !map->is_reachable(previous_info.second.first, share_position.first, creating_dummy->get_speed(previous_info.first), previous_time_limit)) {
						continue;
					}
				}
				//生成中ダミーの既に停止位置が決定しているフェーズよりも共有フェーズが小さい場合
				//※ここは
				else if (creating_dummy->get_speed(0) == 0 && creating_dummy->find_next_fixed_position(0).first >= share_phase)
				{
					std::pair<int, std::pair<Graph::MapNodeIndicator, std::shared_ptr<Geography::LatLng const>>> next_info = creating_dummy->find_next_fixed_position(0);
					int next_time_limit = time_manager->time_of_phase(next_info.first) - time_manager->time_of_phase(share_phase) - requirement->min_pause_time;

					if (next_time_limit < 0 || !map->is_reachable(share_position.first, next_info.second.first, target->get_speed(share_phase), next_time_limit)) {
						continue;
					}
				}
				//生成中ダミーの既に停止位置が決定しているフェーズの間にある場合
				else
				{
					std::pair<int, std::pair<Graph::MapNodeIndicator, std::shared_ptr<Geography::LatLng const>>> previous_info = creating_dummy->find_previous_fixed_position(share_phase);
					std::pair<int, std::pair<Graph::MapNodeIndicator, std::shared_ptr<Geography::LatLng const>>> next_info = creating_dummy->find_next_fixed_position(share_phase);
					//time_limitはpreviousでの停止時間を考慮しなければならないことに注意
					int previous_time_limit = time_manager->time_of_phase(share_phase) - time_manager->time_of_phase(previous_info.first) - requirement->min_pause_time;
					int next_time_limit = time_manager->time_of_phase(next_info.first) - time_manager->time_of_phase(share_phase) - requirement->min_pause_time;

					if (previous_time_limit < 0 || next_time_limit < 0) continue;
					if (!map->is_reachable(previous_info.second.first, share_position.first, creating_dummy->get_speed(previous_info.first), previous_time_limit)
						&& !map->is_reachable(share_position.first, next_info.second.first, target->get_speed(share_phase), next_time_limit)) {
						//ここで一旦POIを探しなおすことを行う！
						continue;
					}
				}

				//Dmincross = += 1;
				//生成中のダミーの交差回数 += 1;
				creating_dummy->set_crossing_position_of_phase(share_phase, share_position.first, *share_position.second);
				creating_dummy->set_speed(share_phase, target->get_speed(share_phase));
				creating_dummy->set_random_speed(share_phase, requirement->average_speed_of_dummy, requirement->speed_range_of_dummy);
				//targetの交差回数+1
				target->register_as_cross_position(share_phase);
				break;

			}
			//ユーザ及び生成済みダミーの平均交差回数よりも，生成中ダミーの交差回数が多くなるまで共有地点を設定する
			int check_cross_count = creating_dummy->get_cross_count();
			int check_all_entities_total_cross_count = entities->get_all_entities_total_crossing_count();

			//最初だけ，もし共有地点が設定できなかったらbreakさせる
			if (check_cross_count == check_all_entities_total_cross_count) break;
			if (check_cross_count > check_all_entities_total_cross_count / (requirement->dummy_num + 1)) break;
		}
	}



	///<summary>
	/// 生成中ダミー(k番目)の移動経路の決定
	///</summary>
	void KatoBachelorMethod::decide_destination_on_the_way(int dummy_id)
	{
		int phase_id = 0;
		Math::Probability generator;
		//生成中ダミーの既に決まっている中で最初の地点を取得
		std::pair<int, std::pair<Graph::MapNodeIndicator, std::shared_ptr<Geography::LatLng const>>> dest_position = creating_dummy->find_next_fixed_position(phase_id);

		//------------------------------------↓初期位置の決定↓------------------------------------------------------//

		//creating_dummy->set_pause_time(0, requirement->max_pause_time);//初期phaseの停止時間
		creating_dummy->set_random_speed(phase_id, requirement->average_speed_of_dummy, requirement->speed_range_of_dummy);//初期phaseのspeedの決定

		//生成中ダミーのプランの中で，一番最初の場所から0秒までの範囲(最大停止時間を考慮)で到着できるPOIを取得
		//一旦リストで取得してから，その中からランダムで選択
		double distance = creating_dummy->get_speed(phase_id) * (time_manager->time_of_phase(dest_position.first) - time_manager->time_of_phase(phase_id));
		Geography::LatLng candidate_init_latlng = Geography::GeoCalculation::calc_translated_point(*dest_position.second.second, distance, M_PI * 0.5);
		//POIを探索する長方形を取得．目的地に近づく方向を考慮
		double top = dest_position.second.second->lat() - candidate_init_latlng.lat() > 0 ? dest_position.second.second->lat() : candidate_init_latlng.lat();
		double left = dest_position.second.second->lng() - candidate_init_latlng.lng() > 0 ? candidate_init_latlng.lng() : dest_position.second.second->lng();
		double bottom = dest_position.second.second->lat() - candidate_init_latlng.lat() < 0 ? dest_position.second.second->lat() : candidate_init_latlng.lat();
		double right = dest_position.second.second->lng() - candidate_init_latlng.lng() < 0 ? candidate_init_latlng.lng() : dest_position.second.second->lng();
		Graph::Rectangle<Geography::LatLng> rect1(top, left, bottom, right);

		std::vector<std::shared_ptr<Map::BasicPoi const>> init_pois_list = get_candidate_pois_list(rect1);
		std::vector<std::shared_ptr<Map::BasicPoi const>>::const_iterator init_poi = init_pois_list.begin();
		int init_time_limit = time_manager->time_of_phase(dest_position.first) - time_manager->time_of_phase(phase_id) - requirement->min_pause_time;

		while (!map->is_reachable(Graph::MapNodeIndicator((*init_poi)->get_id()), dest_position.second.first, creating_dummy->get_speed(phase_id), init_time_limit)) {
			init_poi++;
		}

		//PPoutに<position, start, pauseのinit>を追加;
		creating_dummy->set_position_of_phase(phase_id, Graph::MapNodeIndicator((*init_poi)->get_id()), (*init_poi)->data->get_position());

		//------------------------------------↑初期位置の決定↑------------------------------------------------------//

		//初期位置以降から既に停止位置決定済みのphaseまでの停止地点の決定
		int dest_rest_time = 0;
		while (phase_id < creating_dummy->find_previous_fixed_position(time_manager->phase_count()).first)
		{
			std::pair<int, std::pair<Graph::MapNodeIndicator, std::shared_ptr<Geography::LatLng const>>> decided_position = creating_dummy->find_previous_fixed_position(dest_position.first);
			int time_between_decided_and_dest_position = time_manager->time_of_phase(dest_position.first) - time_manager->time_of_phase(decided_position.first);
			//二点間が非常に近い，かつ，連続で停止地点が決まっていない場合途中停止位置を設定
			//already→destの到着時間の差>decided→destの移動時間＋MaxPauseTime
			//if (time_between_decided_and_dest_position > 2* requirement->max_pause_time) throw std::invalid_argument("Pause Time will be to long!!");
			if (time_between_decided_and_dest_position > map->calc_necessary_time(decided_position.second.first, dest_position.second.first, creating_dummy->get_speed(decided_position.first)) + requirement->max_pause_time) {
				//position(phase_id)からposition(dest)番目へ到達可能なPOIからひとつランダムで取得
				//alreadyとdestで四角形を作り，その中に存在するPOIを選択することで近づく方向を考慮
				double top
					= decided_position.second.second->lat() >= dest_position.second.second->lat() ? decided_position.second.second->lat() : dest_position.second.second->lat();
				double left
					= decided_position.second.second->lng() <= dest_position.second.second->lng() ? decided_position.second.second->lng() : dest_position.second.second->lng();
				double bottom
					= decided_position.second.second->lat() <= dest_position.second.second->lat() ? decided_position.second.second->lat() : dest_position.second.second->lat();
				double right
					= decided_position.second.second->lng() >= dest_position.second.second->lng() ? decided_position.second.second->lng() : dest_position.second.second->lng();
				Graph::Rectangle<Geography::LatLng> rect2(top, left, bottom, right);

				std::vector<std::shared_ptr<Map::BasicPoi const>> on_the_way_pois_list = get_candidate_pois_list(rect2);
				std::vector<std::shared_ptr<Map::BasicPoi const>>::const_iterator poi_on_the_way = on_the_way_pois_list.begin();

				//途中目的地の速度を設定する
				//PauseTimeがまだ決まらないので，セットはしない．
				double on_the_way_speed = generator.uniform_distribution(requirement->average_speed_of_dummy - 0.5 * requirement->speed_range_of_dummy, requirement->average_speed_of_dummy + 0.5 * requirement->speed_range_of_dummy);

				int total_pause_time_at_decided_and_dest_position
					= time_between_decided_and_dest_position
					- map->calc_necessary_time(decided_position.second.first, (*poi_on_the_way)->get_id(), creating_dummy->get_speed(decided_position.first))
					- map->calc_necessary_time((*poi_on_the_way)->get_id(), dest_position.second.first, on_the_way_speed);

				int percentage_of_spilit_pause_time = generator.uniform_distribution(45, 55);
				double rate_of_spilit_pause_time = percentage_of_spilit_pause_time / 100.0;
				int first_pause_time = total_pause_time_at_decided_and_dest_position * rate_of_spilit_pause_time;
				int second_pause_time = total_pause_time_at_decided_and_dest_position - first_pause_time;

				//決定済みの点の停止時間を設定
				creating_dummy->set_pause_time(decided_position.first, first_pause_time);

				//途中目的地までのpathを決める
				linear_interpolation_of_path_between_positions(decided_position.second.first, (*poi_on_the_way)->get_id(), &phase_id, &dest_rest_time);

				//途中目的地の速度のセット
				creating_dummy->set_speed(phase_id, on_the_way_speed);

				//途中目的地の停止時間のセット
				creating_dummy->set_pause_time(phase_id, second_pause_time);

				//decided_positionの更新
				decided_position = creating_dummy->find_previous_fixed_position(dest_position.first);

				//目的地までのpathを決める
				linear_interpolation_of_path_between_positions(decided_position.second.first, dest_position.second.first, &phase_id, &dest_rest_time);

				//目的地の更新
				dest_position = creating_dummy->find_next_fixed_position(phase_id);
			}
			else {
				//decided_positionの停止時間を決める
				//停止時間 = Moving_time(From decided_position to dest_position)
				int pause_time_at_decided_position
					= time_between_decided_and_dest_position - map->calc_necessary_time(decided_position.second.first, dest_position.second.first, creating_dummy->get_speed(phase_id));

				//停止時間のセット
				creating_dummy->set_pause_time(phase_id, pause_time_at_decided_position);

				//途中目的地から次の停止地点のpathを決める
				linear_interpolation_of_path_between_positions(decided_position.second.first, dest_position.second.first, &phase_id, &dest_rest_time);

				//目的地の更新
				dest_position = creating_dummy->find_next_fixed_position(phase_id);

			}
		}

		//シュミレーション終了までの残りの停止位置を適当に決める
		//複数に設定したほうが良いが，とりあえず，最終地点のみ
		while (phase_id < time_manager->phase_count() - 1)
		{
			Entity::MobileEntity<Geography::LatLng>::node_pos_info now_poi = creating_dummy->read_node_pos_info_of_phase(phase_id);

			double length_of_rect = 0.005;//適切な範囲の緯度経度の選択幅を書く
			int rest_phase_time = time_manager->time_of_phase(time_manager->phase_count() - 1) - time_manager->time_of_phase(phase_id);
			if ((rest_phase_time - requirement->min_pause_time) < 0) {
				for (int i = phase_id; i < time_manager->phase_count() - 1; i++)
				{
					phase_id++;
					creating_dummy->set_position_of_phase(phase_id, now_poi.first, *now_poi.second);
					creating_dummy->set_speed(phase_id, 0);
				}
				break;
			}
			else{
				//距離は届く範囲で調整
				int distance = 0.9 * (rest_phase_time - requirement->min_pause_time) * creating_dummy->get_speed(phase_id);
				double angle_of_positions = generator.uniform_distribution(-M_PI_2, M_PI_2);
				Geography::LatLng next_candidate_poi_position_range
					= Geography::GeoCalculation::calc_translated_point(*now_poi.second, distance, angle_of_positions);

				Graph::Rectangle<Geography::LatLng> range(next_candidate_poi_position_range.lat(), next_candidate_poi_position_range.lng() - length_of_rect, next_candidate_poi_position_range.lat() - length_of_rect, next_candidate_poi_position_range.lng());
				//次のPOIの決定
				std::vector<std::shared_ptr<Map::BasicPoi const>> candidate_pois_list = get_candidate_pois_list(range);
				std::vector<std::shared_ptr<Map::BasicPoi const>>::const_iterator next_poi = candidate_pois_list.begin();

				//decided_positionの停止時間を決める
				//停止時間 = Moving_time(From decided_position to dest_position)
				int pause_time_at_decided_position
					= rest_phase_time
					- map->calc_necessary_time(now_poi.first, (*next_poi)->get_id(), creating_dummy->get_speed(phase_id));
				//停止時間のセット
				creating_dummy->set_pause_time(phase_id, pause_time_at_decided_position);

				//途中目的地から次の停止地点のpathを決める
				linear_interpolation_of_path_between_positions(now_poi.first, (*next_poi)->get_id(), &phase_id, &dest_rest_time);
		
			}
		}
	}

	
	///<summary>
	/// 初期化 (今回は特にやることはない)
	///</summary>
	void KatoBachelorMethod::initialize()
	{

	}

	


	///<summary>
	/// ここが提案手法の核になる部分
	///</summary>
	void KatoBachelorMethod::decide_dummy_positions()
	{
		for (size_t dummy_id = 1; dummy_id <= entities->get_dummy_count(); dummy_id++)
		{
			creating_dummy = entities->get_dummy_by_id(dummy_id);

			decide_base_positions_and_arrive_time(dummy_id);// 生成中ダミー(k番目)の基準地点および基準地点到着時間の決定
			decide_share_positions_and_arrive_time(dummy_id);// 生成中ダミー(k番目)の共有地点および共有地点到着時間の決定
			decide_destination_on_the_way(dummy_id);// 生成中ダミー(k番目)の移動経路の決定
		}

	}

	void KatoBachelorMethod::run()
	{
		//ここで実行時間の計測を開始
		timer->start();

		//初期化
		initialize();

		//ここが実行部分(各時刻のダミー位置を計算する)
		decide_dummy_positions();

		//ここで計測を終了
		timer->end();

		//終了処理
		terminate();
	}

}
