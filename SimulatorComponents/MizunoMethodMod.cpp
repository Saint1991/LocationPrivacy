#include "stdafx.h"
#include "MizunoMethodMod.h"

namespace Method
{


	///<summary>
	/// コンストラクタ
	///</summary>
	MizunoMethodMod::MizunoMethodMod(
		std::shared_ptr<Map::BasicDbMap const> map,
		std::shared_ptr<User::BasicUser<Geography::LatLng>> user,
		std::shared_ptr<User::PreferenceTree> observed_preference_tree,
		std::shared_ptr<Requirement::PreferenceRequirement const> requirement,
		std::shared_ptr<Time::TimeSlotManager const> time_manager) : MizunoMethod(map, user, observed_preference_tree, requirement, time_manager)
	{
	}


	MizunoMethodMod::~MizunoMethodMod()
	{
	}

	double large_fixed(int dummy_id, size_t dummy_num, double required_anonymous_area)
	{
		double size = required_anonymous_area * 1.1;
		return dummy_id == 1 ? std::sqrt(size) : size;
	}

	///<summary>
	/// 初期化
	/// 設定匿名領域の決定
	///</summary>
	void MizunoMethodMod::initialize()
	{
		set_setting_anonymous_area(large_fixed);
	}

	double MizunoMethodMod::calc_poi_score(double ar_size, double setting_anonymous_area, int reachable_entity_count)
	{
		return std::abs(ar_size - setting_anonymous_area) * std::pow(1 + reachable_entity_count, 2 * requirement->required_preference_conservation - 1);
	}

	///<summary>
	/// 経路生成の改良版
	///</summary>
	std::shared_ptr<std::vector<Graph::MapNodeIndicator>> MizunoMethodMod::create_trajectory(Entity::entity_id current_dummy_id, const std::pair<int, Graph::MapNodeIndicator>& basis, const Collection::Sequence<User::category_id>& category_sequence)
	{
		//ランダマイズ関連
		Math::Probability prob;
		std::random_device rd;
		std::mt19937_64 generator(rd());

		const double MAX_SPEED = 1000.0 * (requirement->average_speed_of_dummy + requirement->speed_range_of_dummy) / 3600.0;

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
			Graph::Rectangle<Geography::LatLng> search_boundary = Map::BasicDbMap::create_reachable_rect(current_poi->get_point(), reachable_distance);
			std::vector<std::shared_ptr<Map::BasicPoi const>> poi_candidates = map->find_pois_of_category_within_boundary(search_boundary, category);
			if (poi_candidates.size() == 0) return nullptr;
			std::shuffle(poi_candidates.begin(), poi_candidates.end(), generator);

			//ユーザの進行方向に動く点を一つ定める
			std::shared_ptr<Map::BasicPoi const> next_poi = nullptr;
			double current_anonymous_area_size = entities->calc_convex_hull_size_of_fixed_entities_of_phase(phase);
			if (current_anonymous_area_size > setting_anonymous_area) {
				for (std::vector<std::shared_ptr<Map::BasicPoi const>>::const_iterator poi = poi_candidates.begin(); poi != poi_candidates.end(); poi++) {
					if ((*poi)->get_id() == current_poi->get_id() || map->shortest_distance(Graph::MapNodeIndicator(current_poi->get_id()), Graph::MapNodeIndicator((*poi)->get_id())) > reachable_distance) continue;
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
				std::vector<poi_score_tuple> poi_scores;
				for (std::vector<std::shared_ptr<Map::BasicPoi const>>::const_iterator poi = poi_candidates.begin(); poi != poi_candidates.end(); poi++) {
					
					Graph::node_id target_poi_id = (*poi)->get_id();
					if (target_poi_id == current_poi->get_id() || map->shortest_distance(Graph::MapNodeIndicator(current_poi->get_id()), Graph::MapNodeIndicator(target_poi_id)) > reachable_distance) continue;
					
					std::vector<std::shared_ptr<Geography::LatLng const>> positions = entities->get_all_fixed_positions_of_phase(phase);
					positions.push_back(std::make_shared<Geography::LatLng>((*poi)->get_point()));
					double ar_size = positions.size() > 2 ? Geography::GeoCalculation::calc_convex_hull_size(positions) : Geography::GeoCalculation::lambert_distance(*positions.at(0), *positions.at(1));
					
					int reachable_entity_count = 0;
					for (Entity::entity_id id = 0; id < current_dummy_id && phase != 0; id++) {
						Graph::MapNodeIndicator previous_check_poi = entities->read_entity_by_id(id)->read_node_pos_info_of_phase(phase - 1).first;
						double max_distance = MAX_SPEED * std::abs(time_manager->time_of_phase(phase) - time_manager->time_of_phase(phase - 1));
						if (map->shortest_distance(previous_check_poi, Graph::MapNodeIndicator((*poi)->get_id(), Graph::NodeType::POI)) <= max_distance) {
							reachable_entity_count++;
						}
					}
					poi_scores.push_back(std::make_tuple(*poi, ar_size, reachable_entity_count));
				}

				if (poi_scores.size() == 0) return nullptr;
				std::vector<poi_score_tuple>::const_iterator best_poi = std::min_element(poi_scores.begin(), poi_scores.end(), [&](const poi_score_tuple& left, const poi_score_tuple& right) {
					double left_score = calc_poi_score(std::get<1>(left), setting_anonymous_area, std::get<2>(left));
					double right_score = calc_poi_score(std::get<1>(right), setting_anonymous_area, std::get<2>(right));
					return left_score < right_score;
				});

				next_poi = std::get<0>(*best_poi);
			}

			if (next_poi == nullptr) return nullptr;
			current_poi = next_poi;
			ret->at(phase) = Graph::MapNodeIndicator(current_poi->get_id(), Graph::NodeType::POI);
		}



		////後半部分の決定
		reachable_distance_list = std::vector<double>(time_manager->phase_count() - basis.first - 1);
		for (int phase = basis.first + 1; phase < time_manager->phase_count(); phase++) {
			double speed = prob.uniform_distribution(requirement->average_speed_of_dummy - requirement->speed_range_of_dummy, requirement->average_speed_of_dummy + requirement->speed_range_of_dummy);
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
			Graph::Rectangle<Geography::LatLng> search_boundary = Map::BasicDbMap::create_reachable_rect(current_poi->get_point(), reachable_distance);
			std::vector<std::shared_ptr<Map::BasicPoi const>> poi_candidates = map->find_pois_of_category_within_boundary(search_boundary, category);
			if (poi_candidates.size() == 0) return nullptr;
			std::shuffle(poi_candidates.begin(), poi_candidates.end(), generator);

			std::shared_ptr<Map::BasicPoi const> next_poi = nullptr;

			//ユーザの進行方向に動かす
			double current_anonymous_area_size = entities->calc_convex_hull_size_of_fixed_entities_of_phase(phase);
			//std::cout << "current_anonymous_area: " << current_anonymous_area_size << std::endl;
			if (current_anonymous_area_size > setting_anonymous_area) {
				for (std::vector<std::shared_ptr<Map::BasicPoi const>>::const_iterator poi = poi_candidates.begin(); poi != poi_candidates.end(); poi++) {
					if ((*poi)->get_id() == current_poi->get_id() || map->shortest_distance(Graph::MapNodeIndicator(current_poi->get_id()), Graph::MapNodeIndicator((*poi)->get_id())) > reachable_distance) continue;
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
				std::vector<poi_score_tuple> poi_scores;
				for (std::vector<std::shared_ptr<Map::BasicPoi const>>::const_iterator poi = poi_candidates.begin(); poi != poi_candidates.end(); poi++) {
					
					Graph::node_id target_poi_id = (*poi)->get_id();
					if (target_poi_id == current_poi->get_id() || map->shortest_distance(Graph::MapNodeIndicator(current_poi->get_id()), Graph::MapNodeIndicator(target_poi_id)) > reachable_distance) continue;

					std::vector<std::shared_ptr<Geography::LatLng const>> positions = entities->get_all_fixed_positions_of_phase(phase);
					positions.push_back(std::make_shared<Geography::LatLng>((*poi)->get_point()));
					double ar_size = positions.size() > 2 ? Geography::GeoCalculation::calc_convex_hull_size(positions) : Geography::GeoCalculation::lambert_distance(*positions.at(0), *positions.at(1));
					
					int reachable_entity_count = 0;
					for (Entity::entity_id id = 0; id < current_dummy_id && phase != 0; id++) {
						double max_distance = MAX_SPEED * std::abs(time_manager->time_of_phase(phase) - time_manager->time_of_phase(phase - 1));
						Graph::MapNodeIndicator previous_check_poi = entities->read_entity_by_id(id)->read_node_pos_info_of_phase(phase - 1).first;
						if (map->shortest_distance(previous_check_poi, Graph::MapNodeIndicator((*poi)->get_id(), Graph::NodeType::POI)) <= max_distance) {
							reachable_entity_count++;
						}
					}
					poi_scores.push_back(std::make_tuple(*poi, ar_size, reachable_entity_count));
				}

				if (poi_scores.size() == 0) return nullptr;
				std::vector<poi_score_tuple>::const_iterator best_poi = std::min_element(poi_scores.begin(), poi_scores.end(), [&](const poi_score_tuple& left, const poi_score_tuple& right) {
					double left_score = calc_poi_score(std::get<1>(left), setting_anonymous_area, std::get<2>(left));
					double right_score = calc_poi_score(std::get<1>(right), setting_anonymous_area, std::get<2>(right));
					return left_score < right_score;
				});

				next_poi = std::get<0>(*best_poi);
			}

			if (next_poi == nullptr) return nullptr;
			current_poi = next_poi;
			ret->at(phase) = Graph::MapNodeIndicator(next_poi->get_id(), Graph::NodeType::POI);
	}

		return ret;
	}

}

