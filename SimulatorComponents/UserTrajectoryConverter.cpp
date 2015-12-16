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
}

