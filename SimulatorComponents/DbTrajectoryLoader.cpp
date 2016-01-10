#include "stdafx.h"
#include "DbTrajectoryLoader.h"


namespace User
{


	///<summary>
	/// コンストラクタ
	///</summary>
	template <typename TRAJECTORY_TYPE>
	DbTrajectoryLoader<TRAJECTORY_TYPE>::DbTrajectoryLoader(std::function<bool(const std::string&, const std::string&)> division_rule, const std::string& setting_file_path, const std::string& db_name, const std::string& user_table_name, const std::string& venue_table_name)
		: db(std::make_unique<Db::MySQLDb>(std::move(std::make_unique<Db::DbSettingsFileLoader>(setting_file_path)))),
		division_rule(division_rule), db_name(db_name), user_table_name(user_table_name), venue_table_name(venue_table_name)
	{
	}


	///<summary>
	/// デストラクタ
	///</summary>
	template < typename TRAJECTORY_TYPE>
	DbTrajectoryLoader<TRAJECTORY_TYPE>::~DbTrajectoryLoader()
	{
	}

	template <typename TRAJECTORY_TYPE>
	std::shared_ptr<std::vector<std::shared_ptr<TRAJECTORY_TYPE>>> DbTrajectoryLoader<TRAJECTORY_TYPE>::execute_with_query_result(const std::string& query, std::function<std::shared_ptr<std::vector<std::shared_ptr<TRAJECTORY_TYPE>>>(sql::ResultSet*)> execute_function)
	{
		if (!db->use(db_name)) return nullptr;
		sql::ResultSet* result = db->raw_query(query);
		if (result == nullptr || result->rowsCount() == 0) return nullptr;
		return execute_function(result);
	}

	

	///<summary>
	/// データベースからSemanticTrajectoryを作成する
	/// 読出しに失敗した場合はnullptrが返る
	///</summary>
	template <>
	std::shared_ptr<std::vector<std::shared_ptr<Graph::SemanticTrajectory<Geography::LatLng>>>> DbTrajectoryLoader<Graph::SemanticTrajectory<Geography::LatLng>>::load_trajectories(unsigned int user_id, int trajectory_length_threshold, int min_interval, bool use_relative_time)
	{
		std::stringstream query;
		query << "SELECT venue_id, timestamp, category_id, latitude, longitude, venue_name, category_name  FROM " << user_table_name << " INNER JOIN " << venue_table_name << " ON " << user_table_name << ".venue_id = " << venue_table_name << ".id WHERE user_id = " << user_id << " ORDER BY timestamp ASC;";
		
		return execute_with_query_result(query.str(), [&](sql::ResultSet* result) {
			
			std::shared_ptr<std::vector<std::shared_ptr<Graph::SemanticTrajectory<Geography::LatLng>>>> ret = std::make_shared<std::vector<std::shared_ptr<Graph::SemanticTrajectory<Geography::LatLng>>>>();
			
			std::unique_ptr<std::vector<std::string>> times;
			std::shared_ptr<std::vector<Graph::MapNodeIndicator>>node_ids;
			std::shared_ptr<std::vector<std::shared_ptr<Geography::LatLng const>>> positions;
			std::shared_ptr<Collection::Sequence<std::string>>category_sequence;
			std::shared_ptr<std::vector<std::string>> venue_names;
			std::shared_ptr<std::vector<std::string>> category_names;
			
			result->beforeFirst();
			std::string previous_time = "";
			while (result->next()) {
				
				int venue_id = result->getInt("venue_id");
				std::string timestamp = result->getString("timestamp");
				std::string category_id = result->getString("category_id");
				double latitude = result->getDouble("latitude");
				double longitude = result->getDouble("longitude");
				std::string venue_name = result->getString("venue_name");
				std::string category_name = result->getString("category_name");

				//時刻がmin_intervalより短ければmin_interval分加算する
				int interval = Time::TimeUtility::interval(timestamp, previous_time);
				if (interval < min_interval) timestamp = Time::TimeUtility::forward(timestamp, min_interval);
			
				if (division_rule(timestamp , previous_time)) {
					if (times != nullptr && times->size() >= trajectory_length_threshold) {
						std::shared_ptr<Time::TimeSlotManager const> timeslot = std::make_shared<Time::TimeSlotManager const>(std::move(times), use_relative_time);
						std::shared_ptr<Graph::SemanticTrajectory<Geography::LatLng>> trajectory = std::make_shared<Graph::SemanticTrajectory<Geography::LatLng>>(timeslot, std::move(node_ids), std::move(positions), std::move(category_sequence), std::move(venue_names), std::move(category_names));
						ret->push_back(trajectory);
					}
					previous_time = timestamp;
					times = std::make_unique<std::vector<std::string>>();
					node_ids = std::make_shared<std::vector<Graph::MapNodeIndicator>>();
					positions = std::make_shared<std::vector<std::shared_ptr<Geography::LatLng const>>>();
					category_sequence = std::make_shared<Collection::Sequence<std::string>>();
					venue_names = std::make_shared<std::vector<std::string>>();
					category_names = std::make_shared<std::vector<std::string>>();
				}

				times->push_back(timestamp);
				node_ids->push_back(venue_id);
				positions->push_back(std::make_shared<Geography::LatLng const>(latitude, longitude));
				category_sequence->push_back(category_id);
				venue_names->push_back(venue_name);
				category_names->push_back(category_name);
			}
			
			return ret;
		});
	}


	///<summary>
	/// データベースからTrajectoryを読み出す
	///</summary>
	template <>
	std::shared_ptr<std::vector<std::shared_ptr<Graph::Trajectory<Geography::LatLng>>>> DbTrajectoryLoader<Graph::Trajectory<Geography::LatLng>>::load_trajectories(unsigned int user_id, int trajectory_length_threshold, int min_interval, bool use_relative_time)
	{
		std::stringstream query;
		query << "SELECT venue_id, timestamp, latitude, longitude, venue_name FROM " << user_table_name << " INNER JOIN " << venue_table_name << " ON " << user_table_name << ".venue_id = " << venue_table_name << ".id WHERE user_id = " << user_id << " ORDER BY timestamp ASC;";

		return execute_with_query_result(query.str(), [&](sql::ResultSet* result) {

			std::shared_ptr<std::vector<std::shared_ptr<Graph::Trajectory<Geography::LatLng>>>> ret;

			std::shared_ptr<Time::TimeSlotManager const> timeslot;
			std::unique_ptr<std::vector<std::string>> times;
			std::shared_ptr<std::vector<Graph::MapNodeIndicator>>node_ids;
			std::shared_ptr<std::vector<std::shared_ptr<Geography::LatLng const>>> positions;
			std::shared_ptr<std::vector<std::string>> venue_names;

			result->beforeFirst();
			std::string previous_time = "";
			while (result->next()) {

				int venue_id = result->getInt("venue_id");
				std::string timestamp = result->getString("timestamp");
				double latitude = result->getDouble("latitude");
				double longitude = result->getDouble("longitude");
				std::string venue_name = result->getString("venue_name");

				//時刻がmin_intervalより短ければmin_interval分加算する
				int interval = Time::TimeUtility::interval(timestamp, previous_time);
				if (interval < min_interval) timestamp = Time::TimeUtility::forward(timestamp, min_interval);
				
				if (division_rule(timestamp, previous_time)) {
					if (times != nullptr && times->size() >= trajectory_length_threshold) {
						timeslot = std::make_shared<Time::TimeSlotManager const>(std::move(times), use_relative_time);
						std::shared_ptr<Graph::Trajectory<Geography::LatLng>> trajectory = std::make_shared<Graph::Trajectory<Geography::LatLng>>(std::move(timeslot), std::move(node_ids), std::move(positions), std::move(venue_names));
						ret->push_back(trajectory);
					}
					times = std::make_unique<std::vector<std::string>>();
					node_ids = std::make_shared<std::vector<Graph::MapNodeIndicator>>();
					positions = std::make_shared<std::vector<std::shared_ptr<Geography::LatLng const>>>();
					venue_names = std::make_shared<std::vector<std::string>>();
				}

				times->push_back(timestamp);
				node_ids->push_back(venue_id);
				positions->push_back(std::make_shared<Geography::LatLng const>(latitude, longitude));
				venue_names->push_back(venue_name);
			}

			return ret;
		});
	}
	
}

