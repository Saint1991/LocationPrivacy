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
	/// 経路生成の改良版
	///</summary>
	std::shared_ptr<std::vector<Graph::MapNodeIndicator>> MizunoMethodMod::create_trajectory(Entity::entity_id current_dummy_id, const std::pair<int, Graph::MapNodeIndicator>& basis, const Collection::Sequence<User::category_id>& category_sequence)
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

		//前半部分
		for (int phase = basis.first - 1; 0 <= phase; phase--) {
			//制約
			double reachable_distance = reachable_distance_list.at(phase);
			User::category_id category = sequence.at(phase);
			double setting_anonymous_area = setting_anonymous_areas->at(current_dummy_id - 1);

			//探索範囲
			Graph::Rectangle<Geography::LatLng> search_boundary = create_reachable_rect(current_poi->get_point(), reachable_distance);
			std::vector<std::shared_ptr<Map::BasicPoi const>> poi_candidates = map->find_pois_of_category_within_boundary(search_boundary, category);
			if (poi_candidates.size() == 0) return nullptr;

			std::vector<poi_score_pair> poi_scores;
			//訪問POIの探索
			for (std::vector<std::shared_ptr<Map::BasicPoi const>>::const_iterator poi = poi_candidates.begin(); poi != poi_candidates.end(); poi++) {
				if ((*poi)->get_id() == current_poi->get_id() || map->shortest_distance(Graph::MapNodeIndicator(current_poi->get_id()), Graph::MapNodeIndicator((*poi)->get_id())) > reachable_distance) continue;
				
				//そのPOIへの訪問で形成される匿名領域
				std::vector<std::shared_ptr<Geography::LatLng const>> positions = entities->get_all_fixed_positions_of_phase(phase);
				positions.push_back(std::make_shared<Geography::LatLng>((*poi)->get_point()));
				double ar_size = positions.size() > 2 ? Geography::GeoCalculation::calc_convex_hull_size(positions) : Geography::GeoCalculation::lambert_distance(*positions.at(0), *positions.at(1));

				//到達可能性のスコア
				double score = std::abs(ar_size - setting_anonymous_area);
				if (phase > 0) {
					//そのPOIに到達可能なエンティティ数
					int reachable_entity_count = 0;
					for (Entity::entity_id id = 0; id < current_dummy_id; id++) {
						Graph::MapNodeIndicator previous_check_poi = entities->read_entity_by_id(id)->read_node_pos_info_of_phase(phase - 1).first;
						if (map->shortest_distance(previous_check_poi, Graph::MapNodeIndicator((*poi)->get_id(), Graph::NodeType::POI)) <= reachable_distance) {
							reachable_entity_count++;
						}
					}
					score *= (1 + reachable_entity_count);
				}
				poi_scores.push_back(std::make_pair(*poi, score));
			}
			if (poi_scores.size() == 0) return nullptr;
			std::vector<poi_score_pair>::const_iterator temp = std::min_element(poi_scores.begin(), poi_scores.end(), [](const poi_score_pair& left, const poi_score_pair& right) {
				return left.second < right.second;
			});
			current_poi = temp->first;
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

			//設定匿名領域に最も近くなる点を探索
			std::vector<poi_score_pair> poi_scores;
			for (std::vector<std::shared_ptr<Map::BasicPoi const>>::const_iterator poi = poi_candidates.begin(); poi != poi_candidates.end(); poi++) {
				double distance = map->shortest_distance(Graph::MapNodeIndicator(current_poi->get_id()), Graph::MapNodeIndicator((*poi)->get_id()));
				if ((*poi)->get_id() == current_poi->get_id() || distance > reachable_distance) continue;
				
				//ARSizeの計算
				std::vector<std::shared_ptr<Geography::LatLng const>> positions = entities->get_all_fixed_positions_of_phase(phase);
				positions.push_back(std::make_shared<Geography::LatLng>((*poi)->get_point()));
				double ar_size = positions.size() > 2 ? Geography::GeoCalculation::calc_convex_hull_size(positions) : Geography::GeoCalculation::lambert_distance(*positions.at(0), *positions.at(1));
				
				double score = std::abs(ar_size - setting_anonymous_area);
				if (phase > 0) {
					//そのPOIに到達可能なエンティティ数
					int reachable_entity_count = 0;
					for (Entity::entity_id id = 0; id < current_dummy_id; id++) {
						Graph::MapNodeIndicator previous_check_poi = entities->read_entity_by_id(id)->read_node_pos_info_of_phase(phase - 1).first;
						if (map->shortest_distance(previous_check_poi, Graph::MapNodeIndicator((*poi)->get_id(), Graph::NodeType::POI)) <= reachable_distance) {
							reachable_entity_count++;
						}
					}
					score *= (1 + reachable_entity_count);
				}
				poi_scores.push_back(std::make_pair(*poi, score));
			}
			if (poi_scores.size() == 0) return nullptr;

			std::vector<poi_score_pair>::const_iterator temp = std::min_element(poi_scores.begin(), poi_scores.end(), [](const poi_score_pair& left, const poi_score_pair& right) {
				return left.second < right.second;
			});
			current_poi = temp->first;
			ret->at(phase) = Graph::MapNodeIndicator(current_poi->get_id(), Graph::NodeType::POI);
		}

		return ret;
	}

}

