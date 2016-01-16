#include "stdafx.h"
#include "MizunoMethod.h"

namespace Method
{


	///<summary>
	/// コンストラクタ
	///</summary>
	MizunoMethod::MizunoMethod(
		std::shared_ptr<Map::BasicDbPreprocessedMap const> map, 
		std::shared_ptr<User::BasicUser<Geography::LatLng>> user, 
		std::shared_ptr<User::PreferenceTree> observed_preference_tree,
		std::shared_ptr<Requirement::PreferenceRequirement const> requirement, 
		std::shared_ptr<Time::TimeSlotManager const> time_manager)
		: Framework::IProposedMethod<Map::BasicDbPreprocessedMap, User::BasicUser<Geography::LatLng>, Entity::Dummy<Geography::LatLng>, Requirement::PreferenceRequirement, Geography::LatLng, Graph::SemanticTrajectory<Geography::LatLng>>(map, user, requirement, time_manager),
		observed_preference_tree(observed_preference_tree), observed_preference_tree_copy(std::make_shared<User::PreferenceTree>(*observed_preference_tree)),
		setting_anonymous_areas(std::make_unique<std::vector<double>>(requirement->dummy_num))
	{
	}

	MizunoMethod::~MizunoMethod()
	{
	}

	//以下設定匿名領域決定のための関数
	//線形
	double linear(int dummy_id, size_t dummy_num, double required_anonymous_area)
	{
		double size = required_anonymous_area * dummy_id / dummy_num;
		return dummy_id == 1 ? std::sqrt(size) : size;
	}


	///<summary>
	/// 初期化
	/// 設定匿名領域の決定
	///</summary>
	void MizunoMethod::initialize()
	{
		set_setting_anonymous_area(linear);
	}


	///<summary>
	/// 提案手法の内容
	///</summary>
	void MizunoMethod::decide_dummy_positions()
	{
		std::shared_ptr<User::BasicUser<>> user = entities->get_user();
		int num_of_group_a_dummy = requirement->required_preference_conservation * requirement->dummy_num;
		int num_of_group_b_dummy = requirement->dummy_num - num_of_group_a_dummy;

		decide_dummy_positions_of_group_a(num_of_group_a_dummy);
		decide_dummy_positions_of_group_b(num_of_group_b_dummy);
	}

	#pragma region Setting Anonymous Area
	///<summary>
	/// 設定匿名領域の生成
	///</summary>
	void MizunoMethod::set_setting_anonymous_area(const std::function<double(int, size_t, double)>& generator)
	{
		for (int dummy_id = 1; dummy_id <= setting_anonymous_areas->size(); dummy_id++) {
			setting_anonymous_areas->at(dummy_id - 1) = generator(dummy_id, setting_anonymous_areas->size(), requirement->required_anonymous_area);
		}
	}

	

	#pragma endregion

	#pragma region GroupA
	
	///<summary>
	/// グループAのダミーの生成
	///</summary>
	void MizunoMethod::decide_dummy_positions_of_group_a(int num_of_group_a_dummy)
	{
		std::random_device rd;
		std::mt19937_64 generator(rd());

		for (int current_dummy_id = 1; current_dummy_id <= num_of_group_a_dummy; current_dummy_id++) {
			
			//ユーザの嗜好の木に含まれる各プレフィックスについてカテゴリシーケンスとスコアの組を計算する
			std::vector<sequence_score_set> all_sequence_scores = calc_sequence_score_set(current_dummy_id);
			std::sort(all_sequence_scores.begin(), all_sequence_scores.end(), [](const sequence_score_set& ss1, const sequence_score_set& ss2) {
				return ss1.second > ss2.second;
			});
			std::vector<sequence_score_set>::iterator delimiter = std::max_element(all_sequence_scores.begin(), all_sequence_scores.end(), [](const sequence_score_set& ss1, const sequence_score_set& ss2) {
				return ss1.second <= ss2.second;
			});
			std::shuffle(all_sequence_scores.begin(), delimiter + 1, generator);

			//実際の経路の生成 (5つ作成できた時点で打ち切る)
			std::vector<trajectory_score_set> trajectory_scores = calc_trajectory_score_set(current_dummy_id, all_sequence_scores);
			
			//経路が生成できなかった場合は例外を投げて終了 (暫定的処置)
			if (trajectory_scores.size() == 0) {
				throw Framework::TrajectoryNotFoundException("trajectory_scores is empty");
			}
			std::sort(trajectory_scores.begin(), trajectory_scores.end(), [](const trajectory_score_set& left, const trajectory_score_set& right) {
				return left.second > right.second;
			});

			//スコアが最大のものの中からランダムで1つ選出し，経路を確定させる
			//経路の選択
			Math::Probability generator;
			int pick_index = generator.uniform_distribution(0, trajectory_scores.size() - 1);
			std::pair<std::vector<Graph::MapNodeIndicator>, Entity::cross_target> trajectory = trajectory_scores.at(pick_index).first;

			//現在決定中のエンティティの経路を確定させる
			std::shared_ptr<Entity::Dummy<Geography::LatLng>> current_dummy = entities->get_dummy_by_id(current_dummy_id);
			std::vector<Graph::MapNodeIndicator> route = trajectory.first;
			int cross_phase = trajectory.second.first;

			std::cout << "Dummy" << current_dummy_id << ": " << std::endl;
			std::cout << "Cross with " << trajectory.second.second << " at phase " << trajectory.second.first << std::endl;
			for (int phase = 0; phase < route.size(); phase++) {
				Graph::MapNodeIndicator node_id = route.at(phase);
				std::shared_ptr<Map::BasicPoi const> visit_poi = map->get_static_poi(node_id.id());
				Geography::LatLng position(visit_poi->lat(), visit_poi->lng());
				phase == cross_phase ? current_dummy->set_crossing_position_of_phase(phase, node_id, position, visit_poi->name()) : current_dummy->set_position_of_phase(phase, node_id, position, visit_poi->name());
				current_dummy->set_category_of_phase(phase, visit_poi->category_id(), visit_poi->category_name());
				std::cout << visit_poi->category_name() << "\t";
			}
			std::cout << std::endl;

			//observed_preference_tree_copyに決定トラジェクトリの結果を反映する
			observed_preference_tree_copy->add_sequence_counter(current_dummy->read_trajectory()->get_category_sequence());
		}
	}

	///<summary>
	/// シーケンスと共有地点の候補の組<S, SP>に対するスコアの計算
	/// 負になる場合への対応が必要
	///</summary>
	std::vector<std::pair<std::pair<Collection::Sequence<User::category_id>, Entity::cross_target>, double>> MizunoMethod::calc_sequence_score_set(Entity::entity_id current_dummy_id)
	{
		std::shared_ptr<User::BasicUser<Geography::LatLng> const> user = entities->get_user();
		std::shared_ptr<User::PreferenceTree const> preference_tree = user->preference;
		std::shared_ptr<Graph::SemanticTrajectory<> const> user_trajectory  = user->read_trajectory();
		std::vector<sequence_score_set> ret;

		// 今回のトラジェクトリが嗜好の木の最大長より長い場合，プレフィックスの最大長の長さのものを対象とする．
		size_t target_prefix_length = user_trajectory->phase_count();
		
		//score_prefの最大値を求める
		double max_score_pref = 0.0;
		preference_tree->for_each_prefix(target_prefix_length, [&](const Collection::Sequence<User::category_id>& prefix, double sup_u) {
			double sup_o = observed_preference_tree_copy->get_support_of(prefix);
			double score_pref = preference_based_score(sup_u, sup_o);
			if (max_score_pref < score_pref) max_score_pref = score_pref;
		});

		//真の嗜好の木に含まれるすべてのPrefixについて
		preference_tree->for_each_prefix(target_prefix_length, [&](const Collection::Sequence<User::category_id>& prefix, double sup_u) {

			//Score_{pref}の計算
			double sup_o = observed_preference_tree_copy->get_support_of(prefix);
			double score_pref = preference_based_score(sup_u, sup_o) / max_score_pref;

			//Score_{cross}の計算
			for (int target_entity_id = 0; target_entity_id < current_dummy_id; target_entity_id++) {

				std::shared_ptr<Entity::MobileEntity<Geography::LatLng, Graph::SemanticTrajectory<Geography::LatLng>> const> target_entity = entities->read_entity_by_id(target_entity_id);
				std::shared_ptr<Graph::SemanticTrajectory<> const> trajectory = target_entity->read_trajectory();
				Collection::Sequence<Entity::category_id> target_sequence = trajectory->get_category_sequence();

				for (int target_phase = 0; target_phase < time_manager->phase_count(); target_phase++) {

					//交差の時刻と相手
					Entity::cross_target cross = std::make_pair(target_phase, target_entity_id);
					
					//n_share(t)の計算
					int n_share_t = std::pow(entities->get_total_cross_count_of_phase(target_phase), 1.0);
					//n_share(ei)の計算
					int n_share_e = std::pow(target_entity->get_cross_count(), 2.0);

					int delta = target_sequence.subsequence(0, target_phase) == prefix.subsequence(0, target_phase) ? 1 : 0;

					//distroの計算
					Collection::Sequence<Entity::category_id> combined_sequence1 = Collection::concat<Entity::category_id>(target_sequence.subsequence(0, target_phase), prefix.subsequence(target_phase + 1, prefix.size() - 1));
					Collection::Sequence<Entity::category_id> combined_sequence2 = Collection::concat<Entity::category_id>(prefix.subsequence(0, target_phase), target_sequence.subsequence(target_phase + 1, prefix.size() - 1));
					double sup_u_es = preference_tree->get_support_of(combined_sequence1);
					double sup_u_se = preference_tree->get_support_of(combined_sequence2);
					double sup_u_e = preference_tree->get_support_of(target_sequence);
					double distro = (sup_u + sup_u_es) / (sup_u_e + sup_u_se + sup_u + sup_u_es)  ;

					// Score_Crossの計算
					double score_cross = cross_based_score(n_share_t, n_share_e, distro, delta);
					if (score_cross == 0.0) cross = Entity::NOTHING;

					// 合わせたスコアを計算し，追加する
					double score = total_sequence_score(score_pref, score_cross);
					ret.push_back(std::make_pair(std::make_pair(prefix, cross), score));
				}
			}
		});

		return ret;
	}


	///<summary>
	/// シーケンスに対するスコア
	///</summary>
	double MizunoMethod::total_sequence_score(double score_pref, double score_cross)
	{
		return B * score_pref + (1 - B) * score_cross;
	}

	///<summary>
	/// Score_prefの計算式
	///</summary>
	double MizunoMethod::preference_based_score(double sup_u, double sup_o)
	{
		return sup_u - sup_o;
	}


	///<summary>
	/// Score_crossの計算式
	///</summary>
	double MizunoMethod::cross_based_score(int n_share_t, int n_share_e, double distro, int delta)
	{
		return delta * distro / (1 + n_share_t + n_share_e);
	}


	
	///<summary>
	/// 計算したカテゴリシーケンスに基づいて実際の移動経路を生成する
	/// sequence_scoreが負の場合の対応が必要なのでここは改善の余地アリ
	///</summary>
	std::vector<std::pair<std::pair<std::vector<Graph::MapNodeIndicator>, Entity::cross_target>, double>> MizunoMethod::calc_trajectory_score_set(Entity::entity_id current_dummy_id, const std::vector<sequence_score_set>& sequence_scores)
	{
		std::vector<trajectory_score_set> ret;

		std::random_device rd;
		std::mt19937_64 generator(rd());

		auto max_score_element = std::max_element(sequence_scores.begin(), sequence_scores.end(), [](const sequence_score_set& largest, const sequence_score_set& target) {
			return largest.second < target.second;
		});
		bool is_all_sequence_score_negative = max_score_element->second < 0;

		//実際の経路の生成
		for (std::vector<sequence_score_set>::const_iterator iter = sequence_scores.begin(); iter != sequence_scores.end(); iter++) {
			
			Collection::Sequence<User::category_id> category_sequence = iter->first.first;
			double score = iter->second;
			Entity::cross_target cross = iter->first.second;

			if (category_sequence.size() < time_manager->phase_count()) {
				for (int i = 0; i < time_manager->phase_count() - category_sequence.size(); i++) category_sequence.push_back(User::ANY);
			}
			
			//カテゴリシーケンスと共有地点候補を基に基準の点を一つ定め，それを基準に経路を生成
			std::shared_ptr<std::vector<Graph::MapNodeIndicator>> trajectory = nullptr;
			std::pair<int, Graph::MapNodeIndicator> basis = determine_point_basis(category_sequence, cross, current_dummy_id);
			for (int i = 0; i < MAX_TRAJECTORY_CREATION_TRIAL && trajectory == nullptr; i++) {
				trajectory = create_trajectory(current_dummy_id, basis, category_sequence);
			}
			
			if (trajectory == nullptr) continue;

			//ここでトラジェクトリに対してスコアを計算してretに追加する
			double trajectory_score = 0.0;
			double size_sum = 0.0;
			for (int phase = 0; phase < time_manager->phase_count(); phase++) {
				std::vector<std::shared_ptr<Geography::LatLng const>> positions = entities->get_all_fixed_positions_of_phase(phase);
				std::shared_ptr<Geography::LatLng const> new_pos = std::make_shared<Geography::LatLng const>(map->get_static_poi(trajectory->at(phase).id())->get_point());
				positions.push_back(new_pos);
				double ar_size = positions.size() > 2 ? Geography::GeoCalculation::calc_convex_hull_size(positions) : Geography::GeoCalculation::lambert_distance(*positions.at(0), *positions.at(1));
				size_sum += ar_size;
			}
			trajectory_score = score * size_sum;
			if (is_all_sequence_score_negative) trajectory_score = size_sum;
			ret.push_back(std::make_pair(std::make_pair(*trajectory, cross), trajectory_score));
			if (ret.size() >= 5) return ret;
		}

		return ret;
	}
	

	///<summary>
	/// 経路中の一点を定める
	/// 交差が設定できていた場合は，設定された共有地点を，されていない場合は時刻0の時点にグリッドを用いてエンティティが均一になるようにランダムな基準点を決定する
	///</summary>
	std::pair<int, Graph::MapNodeIndicator> MizunoMethod::determine_point_basis(const Collection::Sequence<User::category_id>& category_sequence, const Entity::cross_target& cross, Entity::entity_id current_dummy_id)
	{

		std::random_device rd;
		std::mt19937_64 generator(rd());

		//交差フェーズと交差相手
		Entity::entity_id cross_target = cross.second;
		
		int phase_basis = cross.first;
		Graph::MapNodeIndicator point_basis(INVALID, Graph::NodeType::INVALID);

		//共有地点が設定できていた場合
		if (cross != Entity::NOTHING) {
			point_basis = entities->read_entity_by_id(cross.second)->read_node_pos_info_of_phase(cross.first).first;
		}

		//共有地点が設定できなかった場合
		//9×9のグリッド領域を生成し，エンティティが最小の地点から一点決定する
		else {
			//phase 0における決定済みエンティティの位置のリスト
			phase_basis = 0;
			std::vector<std::shared_ptr<Geography::LatLng const>> fixed_positions = entities->get_all_fixed_positions_of_phase(phase_basis);

			//グリッドの生成
			std::shared_ptr<Geography::LatLng const> center = entities->get_average_position_of_phase(phase_basis);
			int cell_num_on_side = 3;
			double grid_side_length = std::sqrt(requirement->required_anonymous_area);
			std::unique_ptr<Graph::Grid<Geography::LatLng>> grid = std::make_unique<Graph::Grid<Geography::LatLng>>(*center, cell_num_on_side, grid_side_length);

			//グリッドに地点を登録しセル中の点が少ない順に並べる
			//かつ同数の場所の並びはランダムにする
			std::shared_ptr<Map::BasicPoi const> visit_target = nullptr;
			grid->add_points(fixed_positions);
			std::vector<std::pair<Graph::cell_id, size_t>> min_cells = grid->get_cell_ids_order_by_point_count();

			std::vector<std::pair<Graph::cell_id, size_t>>::iterator right;
			for (std::vector<std::pair<Graph::cell_id, size_t>>::iterator left = min_cells.begin(); left != min_cells.end(); left = right + 1) {
				right = std::min_element(left, min_cells.end(), [](const std::pair<Graph::cell_id, size_t >& left, const std::pair<Graph::cell_id, size_t>& min) {
					return left <= min;
				});
				std::shuffle(left, right + 1, generator);
			}

			//要素数が少ないセルから順にPOIを探索する
			for (std::vector<std::pair<Graph::cell_id, size_t>>::const_iterator iter = min_cells.begin(); iter != min_cells.end(); iter++) {
				std::shared_ptr<Graph::Rectangle<Geography::LatLng> const> boundary = grid->read_cell_by_id(iter->first);
				User::category_id target_category = category_sequence.at(phase_basis);
				visit_target = map->find_random_poi_within_boundary(*boundary, target_category);
				if (visit_target != nullptr) {
					point_basis = Graph::MapNodeIndicator(visit_target->get_id(), Graph::NodeType::POI);
					break;
				}
			}

			//該当するカテゴリの点が見つからなかった場合
			if (visit_target == nullptr) throw Framework::TrajectoryNotFoundException("There is no POI with the certain category");
		}

		return std::make_pair(phase_basis, point_basis);
	}

	#pragma endregion GroupAのダミー生成
	
	///<summary>
	/// グループBのダミーの生成
	///</summary>
	void MizunoMethod::decide_dummy_positions_of_group_b(int num_of_group_b_dummy)
	{
		std::random_device rd;
		std::mt19937_64 generator(rd());

		for (int current_dummy_id = requirement->dummy_num - num_of_group_b_dummy + 1; current_dummy_id <= requirement->dummy_num;  current_dummy_id++) {

			//基準点の決定
			//設定済みの共有地点数が最小のエンティティをランダムに一つ選出
			std::vector<std::pair<Entity::entity_id, int>> entity_cross_counts = entities->get_entity_id_list_order_by_cross_count();
			std::vector<std::pair<Entity::entity_id, int>>::iterator last_min_entity_element = std::min_element(entity_cross_counts.begin(), entity_cross_counts.end(), [](const std::pair<Entity::entity_id, int>& compare, const std::pair<Entity::entity_id, int>& min) {
				return compare.first <= min.first;
			});
			std::shuffle(entity_cross_counts.begin(), ++last_min_entity_element, generator);
			Entity::entity_id min_entity = entity_cross_counts.begin()->first;
			std::shared_ptr<Entity::Dummy<>> target_entity = min_entity == 0 ? entities->get_user() : entities->get_dummy_by_id(min_entity);

			//交差未設定の時刻のうち共有地点が設定されているエンティティ数が最小の時刻をランダムに選択
			std::vector<std::pair<int, size_t>> cross_counts;
			for (int phase = 0; phase < time_manager->phase_count(); phase++) {
				if (target_entity->is_cross_set_at_phase(phase)) continue;
				size_t cross_count = entities->get_total_cross_count_of_phase(phase);
				cross_counts.push_back(std::make_pair(phase, cross_count));
			}
			std::sort(cross_counts.begin(), cross_counts.end(), [](const std::pair<int, size_t>& left, const std::pair<int, size_t>& right) {
				return left.second < right.second;
			});
			std::vector<std::pair<int, size_t>>::iterator last_min_phase_element = std::min_element(cross_counts.begin(), cross_counts.end(), [](const std::pair<int, size_t>& compare, const std::pair<int, size_t>& min) {
				return compare.second <= min.second;
			});
			std::shuffle(cross_counts.begin(), ++last_min_phase_element, generator);
			int target_phase = cross_counts.at(0).first;
			Graph::MapNodeIndicator point_basis = target_entity->read_node_pos_info_of_phase(target_phase).first;

			//基準点を基に経路を決定
			Collection::Sequence<User::category_id> any_category_sequence(time_manager->phase_count(), User::ANY);
			std::shared_ptr<std::vector<Graph::MapNodeIndicator>> trajectory = nullptr;
			for (int i = 0; i < MAX_TRAJECTORY_CREATION_TRIAL; i++) {
				trajectory = create_trajectory(current_dummy_id, std::make_pair(target_phase, point_basis), any_category_sequence);
				if (trajectory != nullptr) break;
			}
			if (trajectory == nullptr) throw Framework::TrajectoryNotFoundException("Can't found reachable Trajectory");

			//経路の登録
			int phase = 0;
			for (std::vector<Graph::MapNodeIndicator>::const_iterator iter = trajectory->begin(); iter != trajectory->end(); iter++, phase++) {
				std::shared_ptr<Map::BasicPoi const> visit_poi = map->get_static_poi(iter->id());
				std::shared_ptr<Geography::LatLng const> position = std::make_shared<Geography::LatLng const>(visit_poi->lat(), visit_poi->lng());
				User::category_id category = visit_poi->data->get_category_id();
				target_entity->set_category_of_phase(phase, category, visit_poi->category_name());
				target_entity->set_position_of_phase(phase, *iter, *position, visit_poi->name());
				if (phase == target_phase) target_entity->register_as_cross_position(phase);
			}
		}
	}

	///<summary>
	/// 基準点と移動可能な距離から，探索範囲を作成する
	///</summary>
	Graph::Rectangle<Geography::LatLng> create_reachable_rect(const Geography::LatLng& center, double reachable_distance)
	{
		double top = center.lat() + 0.000009 * reachable_distance;
		double left = center.lng() - 0.000011 * reachable_distance;
		double bottom = center.lat() - 0.000009 * reachable_distance;
		double right = center.lng() + 0.000011 * reachable_distance;
		return Graph::Rectangle<Geography::LatLng>(top, left, bottom, right);
	}

	///<summary>
	/// 基準の点を基に到達可能性を満たす経路生成を試行する
	/// 経路にはランダム性があり，もし到達可能な経路が生成できなかった場合はnullptrを返す
	///</summary>
	std::shared_ptr<std::vector<Graph::MapNodeIndicator>> MizunoMethod::create_trajectory(Entity::entity_id current_dummy_id, const std::pair<int, Graph::MapNodeIndicator>& basis, const Collection::Sequence<User::category_id>& category_sequence)
	{
		//ランダマイズ関連
		Math::Probability prob;
		std::random_device rd;
		std::mt19937_64 generator(rd());

		//ここにトラジェクトリを作成する
		std::shared_ptr<std::vector<Graph::MapNodeIndicator>> ret = std::make_shared<std::vector<Graph::MapNodeIndicator>>(time_manager->phase_count(), Graph::MapNodeIndicator(INVALID, Graph::NodeType::INVALID));
		ret->at(basis.first) = basis.second;

		////前半部分の決定
		Graph::MapNodeIndicator point_basis = basis.second;
		std::vector<double> reachable_distance_list(basis.first);
		for (int phase = basis.first - 1; 0 <= phase; phase--) {
			double speed = prob.uniform_distribution(requirement->average_speed_of_dummy - requirement->speed_range_of_dummy, requirement->average_speed_of_dummy + requirement->speed_range_of_dummy);
			time_t next_time = time_manager->time_of_phase(phase + 1);
			time_t current_time = time_manager->time_of_phase(phase);
			int interval = next_time - current_time;
			reachable_distance_list.at(phase) = max(0.0, speed * 1000.0 * (interval - MIN_PAUSE_TIME) / 3600.0);
		}
		Collection::Sequence<User::category_id> sequence = category_sequence.subsequence(0, basis.first - 1);
		
		//手法に基づき経路を決定
		std::shared_ptr<Map::BasicPoi const> current_poi = map->get_static_poi(point_basis.id());
		for (int phase = basis.first - 1; 0 <= phase; phase--) {
			//制約
			double reachable_distance = reachable_distance_list.at(phase);
			User::category_id category = sequence.at(phase);
			double setting_anonymous_area = setting_anonymous_areas->at(current_dummy_id - 1);

			//探索範囲
			Graph::Rectangle<Geography::LatLng> search_boundary = create_reachable_rect(current_poi->get_point(), reachable_distance);
			std::vector<std::shared_ptr<Map::BasicPoi const>> poi_candidates = map->find_pois_of_category_within_boundary(search_boundary, category);
			if (poi_candidates.size() == 0) return nullptr;
			std::shuffle(poi_candidates.begin(), poi_candidates.end(), generator);

			//ユーザの進行方向に動く点を一つ定める
			std::shared_ptr<Map::BasicPoi const> next_poi = nullptr;
			double current_anonymous_area_size = entities->calc_convex_hull_size_of_fixed_entities_of_phase(phase);
			if (current_anonymous_area_size > setting_anonymous_area) {
				for (std::vector<std::shared_ptr<Map::BasicPoi const>>::const_iterator poi = poi_candidates.begin(); poi != poi_candidates.end(); poi++) {
					if ((*poi)->get_id() == current_poi->get_id() && map->shortest_distance(Graph::MapNodeIndicator(current_poi->get_id()), Graph::MapNodeIndicator((*poi)->get_id())) > reachable_distance) continue;
					double user_direction = entities->get_user_direction_of_phase(phase + 1, phase);
					double poi_direction = Geography::GeoCalculation::lambert_azimuth_angle(current_poi->get_point(), (*poi)->get_point());
					if (std::abs(user_direction - poi_direction) <= THETA) {
						next_poi = *poi;
						break;
					}
				}
			}

			//設定匿名領域に最も近くなる点を探索
			else {
				std::vector<std::pair<std::shared_ptr<Map::BasicPoi const>, double>> poi_dif_map;
				for (std::vector<std::shared_ptr<Map::BasicPoi const>>::const_iterator poi = poi_candidates.begin(); poi != poi_candidates.end(); poi++) {
					if ((*poi)->get_id() == current_poi->get_id() && map->shortest_distance(Graph::MapNodeIndicator(current_poi->get_id()), Graph::MapNodeIndicator((*poi)->get_id())) > reachable_distance) continue;
					std::vector<std::shared_ptr<Geography::LatLng const>> positions = entities->get_all_fixed_positions_of_phase(phase);
					positions.push_back(std::make_shared<Geography::LatLng>((*poi)->get_point()));
					double ar_size = positions.size() > 2 ? Geography::GeoCalculation::calc_convex_hull_size(positions) : Geography::GeoCalculation::lambert_distance(*positions.at(0), *positions.at(1));
					poi_dif_map.push_back(std::make_pair(*poi, ar_size - setting_anonymous_area));
				}
				//昇順にソート
				std::sort(poi_dif_map.begin(), poi_dif_map.end(), [](const std::pair<std::shared_ptr<Map::BasicPoi const>, double>& left, const std::pair<std::shared_ptr<Map::BasicPoi const>, double>& right) {
					return left.second < right.second;
				});
				
				//POIの選択
				if (poi_dif_map.back().second < 0) next_poi = poi_dif_map.back().first;
				else {
					auto target = std::find_if(poi_dif_map.begin(), poi_dif_map.end(), [](const std::pair<std::shared_ptr<Map::BasicPoi const>, double>& element) {
						return element.second >= 0;
					});
					next_poi = target->first;
				}
			}

			if (next_poi == nullptr) return nullptr;
			current_poi = next_poi;
			ret->at(phase) = Graph::MapNodeIndicator(current_poi->get_id(), Graph::NodeType::POI);
		}

		
		
		////後半部分の決定
		reachable_distance_list = std::vector<double>(time_manager->phase_count() - basis.first - 1);
		for (int phase = basis.first + 1; phase < time_manager->phase_count(); phase++) {
			double speed = prob.gaussian_distribution(requirement->average_speed_of_dummy, requirement->speed_range_of_dummy);
			speed = min(speed, requirement->average_speed_of_dummy, requirement->average_speed_of_dummy + requirement->speed_range_of_dummy / 2);
			time_t previous_time = time_manager->time_of_phase(phase - 1);
			time_t current_time = time_manager->time_of_phase(phase);
			int interval = current_time - previous_time;
			reachable_distance_list.at(phase - basis.first - 1) = max(0.0, speed * 1000.0 * (interval - MIN_PAUSE_TIME) / 3600.0);
		}
		
		sequence = category_sequence.subsequence(basis.first + 1, category_sequence.size() - 1);
		current_poi = map->get_static_poi(point_basis.id());
		for (int phase = basis.first + 1; phase < time_manager->phase_count(); phase++) {
			
			//制約
			int index = phase - basis.first - 1;
			double reachable_distance = reachable_distance_list.at(index);
			User::category_id category = sequence.at(index);
			double setting_anonymous_area = setting_anonymous_areas->at(current_dummy_id - 1);

			//探索範囲
			Graph::Rectangle<Geography::LatLng> search_boundary = create_reachable_rect(current_poi->get_point(), reachable_distance);
			std::vector<std::shared_ptr<Map::BasicPoi const>> poi_candidates = map->find_pois_of_category_within_boundary(search_boundary, category);
			if (poi_candidates.size() == 0) return nullptr;
			std::shuffle(poi_candidates.begin(), poi_candidates.end(), generator);

			std::shared_ptr<Map::BasicPoi const> next_poi = nullptr;

			//ユーザの進行方向に動かす
			double current_anonymous_area_size = entities->calc_convex_hull_size_of_fixed_entities_of_phase(phase);
			//std::cout << "current_anonymous_area: " << current_anonymous_area_size << std::endl;
			if (current_anonymous_area_size > setting_anonymous_area) {
				for (std::vector<std::shared_ptr<Map::BasicPoi const>>::const_iterator poi = poi_candidates.begin(); poi != poi_candidates.end(); poi++) {
					if ((*poi)->get_id() == current_poi->get_id() && map->shortest_distance(Graph::MapNodeIndicator(current_poi->get_id()), Graph::MapNodeIndicator((*poi)->get_id())) > reachable_distance) continue;
					double user_direction = entities->get_user_direction_of_phase(phase - 1, phase);
					double poi_direction = Geography::GeoCalculation::lambert_azimuth_angle(current_poi->get_point(), (*poi)->get_point());
					if (std::abs(user_direction - poi_direction) <= THETA) {
						next_poi = *poi;
						break;
					}
				}
			}

			//設定匿名領域に最も近くなる点を探索
			else {
				std::vector<std::pair<std::shared_ptr<Map::BasicPoi const>, double>> poi_dif_map;
				for (std::vector<std::shared_ptr<Map::BasicPoi const>>::const_iterator poi = poi_candidates.begin(); poi != poi_candidates.end(); poi++) {
					if ((*poi)->get_id() == current_poi->get_id() && map->shortest_distance(Graph::MapNodeIndicator(current_poi->get_id()), Graph::MapNodeIndicator((*poi)->get_id())) > reachable_distance) continue;
					std::vector<std::shared_ptr<Geography::LatLng const>> positions = entities->get_all_fixed_positions_of_phase(phase);
					positions.push_back(std::make_shared<Geography::LatLng>((*poi)->get_point()));
					double ar_size = positions.size() > 2 ? Geography::GeoCalculation::calc_convex_hull_size(positions) : Geography::GeoCalculation::lambert_distance(*positions.at(0), *positions.at(1));
					poi_dif_map.push_back(std::make_pair(*poi, ar_size - setting_anonymous_area));
				}
				//昇順にソート
				std::sort(poi_dif_map.begin(), poi_dif_map.end(), [](const std::pair<std::shared_ptr<Map::BasicPoi const>, double>& left, const std::pair<std::shared_ptr<Map::BasicPoi const>, double>& right) {
					return left.second < right.second;
				});

				//POIの選択
				if (poi_dif_map.back().second < 0) next_poi = poi_dif_map.back().first;
				else {
					auto target = std::find_if(poi_dif_map.begin(), poi_dif_map.end(), [](const std::pair<std::shared_ptr<Map::BasicPoi const>, double>& element) {
						return element.second >= 0;
					});
					next_poi = target->first;
				}
			}

			if (next_poi == nullptr) return nullptr;
			current_poi = next_poi;
			ret->at(phase) = Graph::MapNodeIndicator(next_poi->get_id(), Graph::NodeType::POI);
		}

		return ret;
	}

}

