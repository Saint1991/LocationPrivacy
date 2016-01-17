#include "stdafx.h"
#include "UserTrajectoryConverter.h"


namespace User
{

	///<summary>
	/// コンストラクタ
	///</summary>
	UserTrajectoryConverter::UserTrajectoryConverter(std::shared_ptr<Map::BasicDbMap const> map) : map(map)
	{
	}


	///<summary>
	/// デストラクタ
	///</summary>
	UserTrajectoryConverter::~UserTrajectoryConverter()
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
	/// 歩行部分が最長となる切り方での開始Phase，終了Phaseを算出する
	///</summary>
	std::pair<int, int> UserTrajectoryConverter::get_phase_range_of_longest_walking_trajectory(std::shared_ptr<Graph::Trajectory<Geography::LatLng> const> trajectory, double max_speed) const
	{
		int max_length_start = 0;
		int max_length_end = 0;

		int start_phase = 0;
		int end_phase = 0;
		Graph::MapNodeIndicator previous_node;
		trajectory->foreach_state([&](int phase, long interval, const Graph::MapNodeIndicator& visited_node, std::shared_ptr<Graph::TrajectoryState<Geography::LatLng> const> state) {
			if (phase != 0) {
				if (!map->is_reachable(previous_node, visited_node, max_speed, interval)) {
					end_phase = phase - 1;
					int length = end_phase - start_phase + 1;
					if (length > max_length_end - max_length_start) {
						max_length_start = start_phase;
						max_length_end = end_phase;
					}
					start_phase = phase;
				}
			}
			previous_node = visited_node;
		});

		return std::make_pair(max_length_start, max_length_end);
	}


	///<summary>
	/// 歩行で到達可能な移動をしている部分のみ抽出する
	/// 該当部分の長さがmin_trajectory_length未満の場合はnullptrを返す
	///</summary>
	std::shared_ptr<Graph::Trajectory<Geography::LatLng>> UserTrajectoryConverter::extract_walking_trajectory(std::shared_ptr<Graph::Trajectory<Geography::LatLng> const> trajectory, size_t min_trajectory_length, double max_speed) const
	{
		std::shared_ptr<Graph::Trajectory<Geography::LatLng>> ret = nullptr;

		std::pair<int, int> trajectory_phase_range = get_phase_range_of_longest_walking_trajectory(trajectory, max_speed);
		int max_length_start = trajectory_phase_range.first;
		int max_length_end = trajectory_phase_range.second;

		size_t length = max_length_end - max_length_start + 1;
		if (length < min_trajectory_length) return nullptr;

		std::unique_ptr<std::vector<time_t>> timeslots = std::make_unique<std::vector<time_t>>(length);
		std::shared_ptr<std::vector<Graph::MapNodeIndicator>> visited_nodes = std::make_shared<std::vector<Graph::MapNodeIndicator>>(length);
		std::shared_ptr<std::vector<std::shared_ptr<Geography::LatLng const>>> positions = std::make_shared<std::vector<std::shared_ptr<Geography::LatLng const>>>(length);
		std::shared_ptr<std::vector<std::string>> venue_names = std::make_shared<std::vector<std::string>>(length);
		for (int phase = max_length_start; phase <= max_length_end; phase++) {
			std::shared_ptr<Graph::TrajectoryState<Geography::LatLng> const> state = trajectory->read_state_of_phase(phase);
			int index = phase - max_length_start;
			timeslots->at(index) = state->time;
			positions->at(index) = state->position;
			venue_names->at(index) = state->venue_name;
			visited_nodes->at(index) = trajectory->read_node_pos_info_of_phase(phase).first;
		}
		ret = std::make_shared<Graph::Trajectory<Geography::LatLng>>(std::make_shared<Time::TimeSlotManager>(std::move(timeslots)), visited_nodes, positions, venue_names);
		return ret;
	}


	///<summary>
	/// 歩行で到達可能な移動をしている部分のみ抽出する
	/// 該当部分の長さがmin_trajectory_length未満の場合はnullptrを返す
	///</summary>
	std::shared_ptr<Graph::SemanticTrajectory<Geography::LatLng>>  UserTrajectoryConverter::extract_walking_semantic_trajectory(std::shared_ptr<Graph::SemanticTrajectory<Geography::LatLng> const> trajectory, size_t min_trajectory_length, double max_speed) const
	{
		std::shared_ptr<Graph::SemanticTrajectory<Geography::LatLng>> ret = nullptr;

		std::pair<int, int> trajectory_phase_range = get_phase_range_of_longest_walking_trajectory(trajectory, max_speed);
		int max_length_start = trajectory_phase_range.first;
		int max_length_end = trajectory_phase_range.second;

		size_t length = max_length_end - max_length_start + 1;
		if (length < min_trajectory_length) return nullptr;

		std::unique_ptr<std::vector<time_t>> timeslots = std::make_unique<std::vector<time_t>>(length);
		std::shared_ptr<std::vector<Graph::MapNodeIndicator>> visited_nodes = std::make_shared<std::vector<Graph::MapNodeIndicator>>(length);
		std::shared_ptr<std::vector<std::shared_ptr<Geography::LatLng const>>> positions = std::make_shared<std::vector<std::shared_ptr<Geography::LatLng const>>>(length);
		std::shared_ptr<std::vector<std::string>> venue_names = std::make_shared<std::vector<std::string>>(length);
		std::shared_ptr<Collection::Sequence<User::category_id>> category_sequence = std::make_shared<Collection::Sequence<User::category_id>>(length);
		std::shared_ptr<std::vector<std::string>> category_names = std::make_shared<std::vector<std::string>>(length);
		for (int phase = max_length_start; phase <= max_length_end; phase++) {
			std::shared_ptr<Graph::SemanticTrajectoryState<Geography::LatLng> const> state = trajectory->read_semantic_state_of_phase(phase);
			int index = phase - max_length_start;
			timeslots->at(index) = state->time;
			positions->at(index) = state->position;
			venue_names->at(index) = state->venue_name;
			visited_nodes->at(index) = trajectory->read_node_pos_info_of_phase(phase).first;
			category_sequence->at(index) = state->category;
			category_names->at(index) = state->category_name;
		}
		ret = std::make_shared<Graph::SemanticTrajectory<Geography::LatLng>>(std::make_shared<Time::TimeSlotManager>(std::move(timeslots)), visited_nodes, positions, category_sequence, venue_names, category_names);
		return ret;
	}


	///<summary>
	/// トラジェクトリ長とカテゴリシーケンスを保存し，なるべく変更が少ない範囲で歩行可能なトラジェクトリに変換する
	/// 変換候補のPOIが見つからない場合はnullptrを返す
	/// チェックイン間隔が15秒以内の場合は入力間違いとみなして後者のみを採用する
	///</summary>
	std::shared_ptr<Graph::SemanticTrajectory<Geography::LatLng>> UserTrajectoryConverter::convert_to_walking_semantic_trajectory(std::shared_ptr<Graph::SemanticTrajectory<Geography::LatLng> const> original_trajectory, double max_speed) const
	{

		std::random_device rd;
		std::mt19937_64 generator(rd());

		std::shared_ptr<Time::TimeSlotManager const> timeslot = original_trajectory->read_timeslot();
		
		//まずは入力間違いらしきものを除去するため間隔が30秒未満の場合は前者を排除する
		std::vector<time_t> times;
		timeslot->for_each_time([&](time_t time, long interval, int phase) {
			if (phase == 0) {
				times.push_back(time);
				return;
			}
			else {
				if (interval < 15) times.at(times.size() - 1) = time;
				else 	times.push_back(time);
			}
		});
		
		std::shared_ptr<std::vector<Graph::MapNodeIndicator>> node_ids = std::make_shared<std::vector<Graph::MapNodeIndicator>>();
		std::shared_ptr<std::vector<std::shared_ptr<Geography::LatLng const>>> positions = std::make_shared<std::vector<std::shared_ptr<Geography::LatLng const>>>();
		std::shared_ptr<std::vector<std::string>> venue_names = std::make_shared<std::vector<std::string>>();
		std::shared_ptr<Collection::Sequence<User::category_id>> category_sequence = std::make_shared<Collection::Sequence<User::category_id>>();
		std::shared_ptr<std::vector<std::string>> category_names = std::make_shared<std::vector<std::string>>();
		for (std::vector<time_t>::const_iterator time = times.begin(); time != times.end(); time++) {
			
			auto info = original_trajectory->read_node_pos_info_at(*time);
			std::shared_ptr<Graph::SemanticTrajectoryState<Geography::LatLng> const> state = original_trajectory->read_semantic_state_of_phase(timeslot->find_phase_of_time(*time));
			
			Graph::MapNodeIndicator node = info.first;
			std::shared_ptr<Geography::LatLng const> pos = info.second;
			std::string venue_name = state->venue_name;
			std::string category_name = state->category_name;
			User::category_id category = state->category;

			node_ids->push_back(node);
			positions->push_back(pos);
			venue_names->push_back(venue_name);
			category_sequence->push_back(category);
			category_names->push_back(category_name);
		}

		std::shared_ptr<Time::TimeSlotManager const> cleaned_timeslot = std::make_shared<Time::TimeSlotManager const>(std::move(std::make_unique<std::vector<time_t>>(times)), false);
		std::shared_ptr<Graph::SemanticTrajectory<Geography::LatLng>> cleaned_trajectory 
			= std::make_shared<Graph::SemanticTrajectory<Geography::LatLng>>(cleaned_timeslot, node_ids, positions, category_sequence, venue_names, category_names);



		//ここから歩行最長部分を切り出し，前後を歩行で到達可能な場所に変換していく
		std::pair<int, int> range = get_phase_range_of_longest_walking_trajectory(cleaned_trajectory, max_speed);
		std::vector<time_t> new_times(cleaned_timeslot->phase_count());
		std::vector<Graph::MapNodeIndicator> new_trajectory(cleaned_timeslot->phase_count());
		for (int c_phase = range.first; c_phase <= range.second; c_phase++) {
			new_trajectory.at(c_phase) = cleaned_trajectory->read_node_pos_info_of_phase(c_phase).first;
			new_times.at(c_phase) = cleaned_timeslot->time_of_phase(c_phase);
		}

		//前方につなげていく
		std::shared_ptr<Map::BasicPoi const> current_poi = map->get_static_poi(original_trajectory->read_node_pos_info_of_phase(range.first).first.id());
		for (int phase = range.first - 1; 0 <= phase; phase--) {
			
			long interval = std::abs(cleaned_timeslot->time_of_phase(phase + 1) - cleaned_timeslot->time_of_phase(phase));
			User::category_id category = category_sequence->at(phase);
			std::shared_ptr<Map::BasicPoi const> next_poi = nullptr;
			
			//該当するPOIが見つからない場合はintervalを2分ずつ広げる
			while (next_poi == nullptr) {

				double reachable_distance = max_speed * interval;

				//マップから変換候補のPOIを探索
				Graph::Rectangle<Geography::LatLng> boundary = create_reachable_rect(current_poi->get_point(), reachable_distance);
				std::vector<std::shared_ptr<Map::BasicPoi const>> poi_candidates = map->find_pois_of_category_within_boundary(boundary, category);
				if (poi_candidates.size() == 0) {
					interval += 120;
					continue;
				}
			
				std::shuffle(poi_candidates.begin(), poi_candidates.end(), generator);
				for (std::vector<std::shared_ptr<Map::BasicPoi const>>::const_iterator poi = poi_candidates.begin(); poi != poi_candidates.end(); poi++) {
					double distance = map->shortest_distance(Graph::MapNodeIndicator(current_poi->get_id(), Graph::NodeType::POI), Graph::MapNodeIndicator((*poi)->get_id(), Graph::NodeType::POI), reachable_distance);
					if (distance <= reachable_distance) {
						next_poi = *poi;
						break;
					}
				}
				if (next_poi == nullptr) interval += 120;
			}
			
			new_trajectory.at(phase) = Graph::MapNodeIndicator(next_poi->get_id(), Graph::NodeType::POI);
			new_times.at(phase) = cleaned_timeslot->time_of_phase(phase + 1) - interval;
			current_poi = next_poi;
		}

		//後方につなげていく
		current_poi = map->get_static_poi(cleaned_trajectory->read_node_pos_info_of_phase(range.second).first.id());
		for (int phase = range.second + 1; phase < cleaned_timeslot->phase_count(); phase++) {
			
			long interval = std::abs(cleaned_timeslot->time_of_phase(phase) - cleaned_timeslot->time_of_phase(phase - 1));
			User::category_id category = category_sequence->at(phase);
			std::shared_ptr<Map::BasicPoi const> next_poi = nullptr;

			while (next_poi == nullptr) {
				double reachable_distance = max_speed * interval;

				//マップから変換候補のPOIを探索
				Graph::Rectangle<Geography::LatLng> boundary = create_reachable_rect(current_poi->get_point(), reachable_distance);
				std::vector<std::shared_ptr<Map::BasicPoi const>> poi_candidates = map->find_pois_of_category_within_boundary(boundary, category);
				if (poi_candidates.size() == 0) {
					interval += 120;
					continue;
				}

				std::shuffle(poi_candidates.begin(), poi_candidates.end(), generator);
				for (std::vector<std::shared_ptr<Map::BasicPoi const>>::const_iterator poi = poi_candidates.begin(); poi != poi_candidates.end(); poi++) {
					double distance = map->shortest_distance(Graph::MapNodeIndicator(current_poi->get_id(), Graph::NodeType::POI), Graph::MapNodeIndicator((*poi)->get_id(), Graph::NodeType::POI), reachable_distance);
					if (distance <= reachable_distance) {
						next_poi = *poi;
						break;
					}
				}
				if (next_poi == nullptr) interval += 120;
			}
		
			new_trajectory.at(phase) = Graph::MapNodeIndicator(next_poi->get_id(), Graph::NodeType::POI);
			new_times.at(phase) = cleaned_timeslot->time_of_phase(phase - 1) + interval;
			current_poi = next_poi;
		}

		
		//マップから新しいTrajectoryを作成
		std::shared_ptr<Time::TimeSlotManager> new_timeslot = std::make_shared<Time::TimeSlotManager>(std::move(std::make_unique<std::vector<time_t>>(new_times)), false);
		std::shared_ptr<std::vector<Graph::MapNodeIndicator>> visited_nodes = std::make_shared<std::vector<Graph::MapNodeIndicator>>(new_trajectory);

		size_t length = new_timeslot->phase_count();
		std::shared_ptr<std::vector<std::shared_ptr<Geography::LatLng const>>> new_positions = std::make_shared<std::vector<std::shared_ptr<Geography::LatLng const>>>(length);
		std::shared_ptr<std::vector<std::string>> new_venue_names = std::make_shared<std::vector<std::string>>(length);
		std::shared_ptr<std::vector<std::string>> new_category_names = std::make_shared<std::vector<std::string>>(length);
		for (int phase = 0; phase < length; phase++) {
			Graph::MapNodeIndicator node = visited_nodes->at(phase);
			if (node.type() != Graph::NodeType::POI) {
				throw std::invalid_argument("NodeType must be POI");
			}
			std::shared_ptr<Map::BasicPoi const> poi = map->get_static_poi(node.id());
			new_positions->at(phase) = std::make_shared<Geography::LatLng const>(poi->get_point());
			new_venue_names->at(phase) = poi->name();
			new_category_names->at(phase) = poi->category_name();
		}

		return std::make_shared<Graph::SemanticTrajectory<Geography::LatLng>>(new_timeslot, visited_nodes, new_positions, category_sequence, new_venue_names, new_category_names);
	}

	std::shared_ptr<Graph::SemanticTrajectory<Geography::LatLng>> UserTrajectoryConverter::convert_to_walking_compressed_semantic_trajectory(std::shared_ptr<Graph::SemanticTrajectory<Geography::LatLng> const> original_trajectory, double max_speed, int max_interval) const
	{

		std::random_device rd;
		std::mt19937_64 generator(rd());

		std::shared_ptr<Time::TimeSlotManager const> timeslot = original_trajectory->read_timeslot();
		Collection::Sequence<std::string> category_sequence = original_trajectory->get_category_sequence(0, -1);
		
		//サービス利用間隔は10～15分に変換
		time_t begin_time = timeslot->time_of_phase(0);
		std::vector<time_t> times = { begin_time };
		for (int phase = 1; phase < timeslot->phase_count(); phase++) {
			time_t previous_time = times.at(phase - 1);
			time_t current_time = timeslot->time_of_phase(phase);
			int interval = current_time - previous_time;
			if (interval < 600) interval = 600;
			time_t time = previous_time + min(interval, max_interval);
			times.push_back(time);
		}

		//歩行部分の最長一致等は考えずに最初の点を起点に順に変換していく
		std::shared_ptr<Map::BasicPoi const> current_poi = map->get_static_poi(original_trajectory->read_node_pos_info_of_phase(0).first.id());

		std::vector<time_t> new_times(times.size());
		new_times.at(0) = times.at(0);
		std::vector < Graph::MapNodeIndicator> new_trajectory(times.size());
		new_trajectory.at(0) = original_trajectory->read_node_pos_info_of_phase(0).first;

		//後方につなげていく
		for (int phase = 1; phase < times.size(); phase++) {

			//このPhaseにおける経過時刻とカテゴリ
			long interval = std::abs(times.at(phase) - times.at(phase - 1));
			User::category_id category = category_sequence.at(phase);
			
			std::shared_ptr<Map::BasicPoi const> next_poi = nullptr;
			while (next_poi == nullptr && interval <= max_interval) {
				double reachable_distance = max_speed * interval;

				//マップから変換候補のPOIを探索
				Graph::Rectangle<Geography::LatLng> boundary = create_reachable_rect(current_poi->get_point(), reachable_distance);
				std::vector<std::shared_ptr<Map::BasicPoi const>> poi_candidates = map->find_pois_of_category_within_boundary(boundary, category);
				if (poi_candidates.size() == 0) {
					interval += 120;
					continue;
				}

				std::shuffle(poi_candidates.begin(), poi_candidates.end(), generator);
				for (std::vector<std::shared_ptr<Map::BasicPoi const>>::const_iterator poi = poi_candidates.begin(); poi != poi_candidates.end(); poi++) {
					double distance = map->shortest_distance(Graph::MapNodeIndicator(current_poi->get_id(), Graph::NodeType::POI), Graph::MapNodeIndicator((*poi)->get_id(), Graph::NodeType::POI), reachable_distance);
					if (distance <= reachable_distance) {
						next_poi = *poi;
						break;
					}
				}
				if (next_poi == nullptr) interval += 120;
			}

			if (next_poi == nullptr) {
				throw std::exception("Trajectory Not found");
			} 
			else {
				new_trajectory.at(phase) = Graph::MapNodeIndicator(next_poi->get_id(), Graph::NodeType::POI);
				new_times.at(phase) = new_times.at(phase - 1) + interval;
				current_poi = next_poi;
			}
		}

		//マップから新しいTrajectoryを作成
		std::shared_ptr<Time::TimeSlotManager> new_timeslot = std::make_shared<Time::TimeSlotManager>(std::move(std::make_unique<std::vector<time_t>>(new_times)), false);
		std::shared_ptr<std::vector<Graph::MapNodeIndicator>> visited_nodes = std::make_shared<std::vector<Graph::MapNodeIndicator>>(new_trajectory);

		size_t length = new_timeslot->phase_count();
		std::shared_ptr<std::vector<std::shared_ptr<Geography::LatLng const>>> new_positions = std::make_shared<std::vector<std::shared_ptr<Geography::LatLng const>>>(length);
		std::shared_ptr<std::vector<std::string>> new_venue_names = std::make_shared<std::vector<std::string>>(length);
		std::shared_ptr<std::vector<std::string>> new_category_names = std::make_shared<std::vector<std::string>>(length);
		for (int phase = 0; phase < length; phase++) {
			Graph::MapNodeIndicator node = visited_nodes->at(phase);
			if (node.type() != Graph::NodeType::POI) {
				throw std::invalid_argument("NodeType must be POI");
			}
			std::shared_ptr<Map::BasicPoi const> poi = map->get_static_poi(node.id());
			new_positions->at(phase) = std::make_shared<Geography::LatLng const>(poi->get_point());
			new_venue_names->at(phase) = poi->name();
			new_category_names->at(phase) = poi->category_name();
		}

		return std::make_shared<Graph::SemanticTrajectory<Geography::LatLng>>(new_timeslot, visited_nodes, new_positions, std::make_shared<Collection::Sequence<std::string>>(category_sequence), new_venue_names, new_category_names);
	}
}

