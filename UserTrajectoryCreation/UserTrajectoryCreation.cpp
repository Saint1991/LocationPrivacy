// UserTrajectoryCreation.cpp : コンソール アプリケーションのエントリ ポイントを定義します。
//

#pragma once
#include "stdafx.h"
#include "BasicDbMap.h"
#include "LatLng.h"
#include "DbTrajectoryLoader.h"
#include "PreferenceTree.h"
#include "Dijkstra.h"
#include "TimeUtility.h"
#include "UserTrajectoryConverter.h"
#include "MizunoMethod.h"
#include "MatsunoMethod.h"
#include "PreferenceRequirement.h"
#include "UserTrajectoryConverter.h"
#include "OnTimeQueryDbMap.h"
#include "BasicDbPreprocessedMap.h"

///<summary>
/// トラジェクトリ分割のルール
/// 経過時刻が2時間以上のところでトラジェクトリを分割する
///</summary>
bool trajectory_division_rule(const std::string& timestamp, const std::string& previous_time)
{
	constexpr time_t DIVISION_THRESHOLD = 7200;

	time_t current_time_t = Time::TimeUtility::convert_to_unixtimestamp(timestamp);
	time_t previous_time_t = Time::TimeUtility::convert_to_unixtimestamp(previous_time);

	time_t passed_time = current_time_t - previous_time_t;
	if (passed_time < DIVISION_THRESHOLD) return false;
	return true;
}

std::vector<std::string> get_all_user_id(std::shared_ptr<Db::MySQLDb> db)
{
	std::vector<std::string> ret;
	sql::ResultSet* result = db->raw_query("SELECT DISTINCT(user_id) FROM checkins_cleaned WHERE user_id >= 2198;");
	result->beforeFirst();

	while (result->next()) {
		std::string user_id = result->getString("user_id");
		ret.push_back(user_id);
	}
	delete result;
	return ret;
}

void insert(int user_id, std::shared_ptr<Graph::SemanticTrajectory<Geography::LatLng>> trajectory, std::shared_ptr<Db::MySQLDb> db)
{
	std::stringstream query;
	query << "INSERT INTO checkins_compressed (user_id, venue_id, timestamp) VALUES(" << user_id << ", ?, ?);";
	sql::PreparedStatement* statement = db->prepare(query.str());

	std::shared_ptr<Time::TimeSlotManager const> timeslot = trajectory->read_timeslot();
	for (int phase = 0; phase < timeslot->phase_count(); phase++) {
		time_t time = timeslot->time_of_phase(phase);
		Graph::node_id id = trajectory->read_node_pos_info_of_phase(phase).first.id();
		std::string timestamp = Time::TimeUtility::mysql_timestamp(time);

		try {
			statement->setInt(1, id);
			statement->setString(2, timestamp);
			statement->execute();
		}
		catch (sql::SQLException e) {
			std::cerr << e.what() << std::endl;
		}
		
	}
}

///<summary>
/// 歩行で到達可能なトラジェクトリにユーザトラジェクトリを変換してDBに格納する
///</summary>
int main()
{
	constexpr char* DB_NAME = "map_tokyo_category_top_level";
	constexpr double MAX_INTERVAL = 720.0;
	constexpr double MAX_WALKING_SPEED = 5.0 * 1000 / 3600.0;

	
	std::shared_ptr<Db::MySQLDb> db = std::make_shared<Db::MySQLDb>(std::move(std::make_unique<Db::DbSettingsFileLoader>("../settings/mydbsettings.xml")));
	db->use(DB_NAME);
	
	//std::vector<std::string> user_ids = get_all_user_id(db);
	//std::vector<std::string> user_ids = { "822", "1092", "1143", "1240", "196", "557", "368" };
	std::vector<std::string> user_ids = {"196", "557", "368" };

	User::DbTrajectoryLoader<Graph::SemanticTrajectory<Geography::LatLng>> loader(trajectory_division_rule, "../settings/mydbsettings.xml", DB_NAME, "checkins_cleaned", "pois");
	
	//マップの読み込み
	std::shared_ptr<Map::BasicDbMap> map = std::make_shared<Map::OnTimeQueryDbMap>(std::make_shared<Graph::Dijkstra<Map::BasicMapNode, Map::BasicRoad>>(), "../settings/mydbsettings.xml", DB_NAME);
	map->load(Graph::Rectangle<Geography::LatLng>(35.9, 139.4, 35.5, 141.0));
	
	std::unique_ptr<User::UserTrajectoryConverter> converter = std::make_unique<User::UserTrajectoryConverter>(map);

	for (std::vector<std::string>::const_iterator user_id = user_ids.begin(); user_id != user_ids.end(); user_id++) {
		int failed_count = 0;
		std::cout << "User" << *user_id << "---" << std::endl;
		std::shared_ptr<std::vector<std::shared_ptr<Graph::SemanticTrajectory<Geography::LatLng>>>> trajectories = loader.load_trajectories(std::stoi(*user_id), 4, 0, false);
		size_t trajectory_size = trajectories->size();
		if (trajectory_size == 0) continue;
		
		int trajectory_count = 1;
		double avg_trajectory_length = 0.0;
		for (std::vector<std::shared_ptr<Graph::SemanticTrajectory<Geography::LatLng>>>::const_iterator trajectory = trajectories->begin(); trajectory != trajectories->end(); trajectory++, trajectory_count++) {
			std::shared_ptr<Graph::SemanticTrajectory<Geography::LatLng>> insert_trajectory = converter->convert_to_walking_compressed_semantic_trajectory(*trajectory, MAX_WALKING_SPEED, MAX_INTERVAL);
			if (insert_trajectory == nullptr) {
				failed_count++;
				continue;
			}
			insert(std::stoi(*user_id), insert_trajectory, db);
			avg_trajectory_length += insert_trajectory->phase_count();
			std::cout << trajectory_count << " / " << trajectory_size << std::endl;
		}
		avg_trajectory_length /= trajectory_size;
		std::cout << "Failed: " << failed_count << std::endl;
		//std::stringstream query;
		//query << "INSERT INTO user_trajectory_infos (user_id, t_count, avg_t_length) VALUES (" << *user_id << ", " << trajectory_size << ", " << avg_trajectory_length << ");";
		//std::cout << query.str() << std::endl;
		//db->execute(query.str());
	}
	return 0;
}

