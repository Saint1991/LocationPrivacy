#include "stdafx.h"
#include "MizunoMethodModWithoutReachability.h"

namespace Method
{

	MizunoMethodModWithoutReachability::MizunoMethodModWithoutReachability(
		std::shared_ptr<Map::BasicDbMap const> map,
		std::shared_ptr<User::BasicUser<Geography::LatLng>> user,
		std::shared_ptr<User::PreferenceTree> observed_preference_tree,
		std::shared_ptr<Requirement::PreferenceRequirement const> requirement,
		std::shared_ptr<Time::TimeSlotManager const> time_manager) : MizunoMethodMod(map, user, observed_preference_tree, requirement, time_manager)
	{
	}


	MizunoMethodModWithoutReachability::~MizunoMethodModWithoutReachability()
	{
	}

	///<summary>
	/// POIのスコア計算
	///</summary>
	double MizunoMethodModWithoutReachability::calc_poi_score(double ar_size, double setting_anonymous_area, int reachable_entity_count, double already_achieved_anonymous_area_size)
	{
		double score = 1.0;
		double increament = ar_size - already_achieved_anonymous_area_size;
		score *= already_achieved_anonymous_area_size >= setting_anonymous_area ?
			(1.0 / std::abs(ar_size - setting_anonymous_area)) : increament;
		return score;
	}

	//基準の点を基に実際に到達可能な経路を生成する
	std::shared_ptr<std::vector<Graph::MapNodeIndicator>> MizunoMethodModWithoutReachability::create_trajectory(Entity::entity_id current_dummy_id, const std::pair<int, Graph::MapNodeIndicator>& basis, const Collection::Sequence<User::category_id>& category_sequence)
	{
		//ランダマイズ関連
		Math::Probability prob;
		std::random_device rd;
		std::mt19937_64 generator(rd());

		const double MAX_SPEED = (requirement->average_speed_of_dummy + requirement->speed_range_of_dummy) * 1000.0 / 3600.0;

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

			//最大到達可能距離
			double max_distance = phase == 0 ? 0 : MAX_SPEED * std::abs(time_manager->time_of_phase(phase) - time_manager->time_of_phase(phase - 1));

			//現時点で生成できている匿名領域のサイズ
			double current_anonymous_area_size = entities->calc_convex_hull_size_of_fixed_entities_of_phase(phase);

			//探索範囲
			Graph::Rectangle<Geography::LatLng> search_boundary = Map::BasicDbMap::create_reachable_rect(current_poi->get_point(), reachable_distance);
			std::vector<std::shared_ptr<Map::BasicPoi const>> poi_candidates = map->find_pois_of_category_within_boundary(search_boundary, category);
			if (poi_candidates.size() == 0) return nullptr;

			//スコアリングにより訪問場所を決定
			std::vector<poi_score_tuple> poi_scores;
			//訪問POIの探索
			for (std::vector<std::shared_ptr<Map::BasicPoi const>>::const_iterator poi = poi_candidates.begin(); poi != poi_candidates.end(); poi++) {
				if ((*poi)->get_id() == current_poi->get_id() || map->shortest_distance(Graph::MapNodeIndicator(current_poi->get_id()), Graph::MapNodeIndicator((*poi)->get_id())) > reachable_distance) continue;

				//そのPOIへの訪問で形成される匿名領域
				std::vector<std::shared_ptr<Geography::LatLng const>> positions = entities->get_all_fixed_positions_of_phase(phase);
				positions.push_back(std::make_shared<Geography::LatLng>((*poi)->get_point()));
				double ar_size = positions.size() > 2 ? Geography::GeoCalculation::calc_convex_hull_size(positions) : Geography::GeoCalculation::lambert_distance(*positions.at(0), *positions.at(1));

				int reachable_entity_count = 0;
				poi_scores.push_back(std::make_tuple(*poi, ar_size, reachable_entity_count));
			}
			if (poi_scores.size() == 0) return nullptr;

			std::vector<poi_score_tuple>::const_iterator best_poi = std::max_element(poi_scores.begin(), poi_scores.end(), [&](const poi_score_tuple& left, const poi_score_tuple& right) {
				double left_score = calc_poi_score(std::get<1>(left), setting_anonymous_area, std::get<2>(left), current_anonymous_area_size);
				double right_score = calc_poi_score(std::get<1>(right), setting_anonymous_area, std::get<2>(right), current_anonymous_area_size);
				return left_score < right_score;
			});

			current_poi = std::get<0>(*best_poi);
			ret->at(phase) = Graph::MapNodeIndicator(current_poi->get_id(), Graph::NodeType::POI);
		}



		////後半部分の決定
		reachable_distance_list = std::vector<double>(time_manager->phase_count() - basis.first - 1);
		for (int phase = basis.first + 1; phase < time_manager->phase_count(); phase++) {
			double speed = prob.uniform_distribution(requirement->average_speed_of_dummy, requirement->speed_range_of_dummy);
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

			//最大到達可能距離
			double max_distance = phase == 0 ? 0 : MAX_SPEED * std::abs(time_manager->time_of_phase(phase) - time_manager->time_of_phase(phase - 1));

			//現時点で生成できている匿名領域のサイズ
			double current_anonymous_area_size = entities->calc_convex_hull_size_of_fixed_entities_of_phase(phase);

			//探索範囲
			Graph::Rectangle<Geography::LatLng> search_boundary = Map::BasicDbMap::create_reachable_rect(current_poi->get_point(), reachable_distance);
			std::vector<std::shared_ptr<Map::BasicPoi const>> poi_candidates = map->find_pois_of_category_within_boundary(search_boundary, category);
			if (poi_candidates.size() == 0) return nullptr;

			std::vector<poi_score_tuple> poi_scores;

			//訪問POIの探索
			for (std::vector<std::shared_ptr<Map::BasicPoi const>>::const_iterator poi = poi_candidates.begin(); poi != poi_candidates.end(); poi++) {
				if ((*poi)->get_id() == current_poi->get_id() || map->shortest_distance(Graph::MapNodeIndicator(current_poi->get_id()), Graph::MapNodeIndicator((*poi)->get_id())) > reachable_distance) continue;

				//そのPOIへの訪問で形成される匿名領域
				std::vector<std::shared_ptr<Geography::LatLng const>> positions = entities->get_all_fixed_positions_of_phase(phase);
				positions.push_back(std::make_shared<Geography::LatLng>((*poi)->get_point()));
				double ar_size = positions.size() > 2 ? Geography::GeoCalculation::calc_convex_hull_size(positions) : Geography::GeoCalculation::lambert_distance(*positions.at(0), *positions.at(1));

				int reachable_entity_count = 0;
				poi_scores.push_back(std::make_tuple(*poi, ar_size, reachable_entity_count));
			}
			if (poi_scores.size() == 0) return nullptr;

			std::vector<poi_score_tuple>::const_iterator best_poi = std::max_element(poi_scores.begin(), poi_scores.end(), [&](const poi_score_tuple& left, const poi_score_tuple& right) {
				double left_score = calc_poi_score(std::get<1>(left), setting_anonymous_area, std::get<2>(left), current_anonymous_area_size);
				double right_score = calc_poi_score(std::get<1>(right), setting_anonymous_area, std::get<2>(right), current_anonymous_area_size);
				return left_score < right_score;
			});

			current_poi = std::get<0>(*best_poi);
			ret->at(phase) = Graph::MapNodeIndicator(current_poi->get_id(), Graph::NodeType::POI);
		}

		return ret;
	}

}

