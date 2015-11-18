#include "stdafx.h"
#include "DbTrajectoryLoader.h"


namespace User
{


	///<summary>
	/// コンストラクタ
	///</summary>
	template <typename TRAJECTORY_TYPE>
	DbTrajectoryLoader<TRAJECTORY_TYPE>::DbTrajectoryLoader(const std::string& setting_file_path, const std::string& db_name, const std::string& user_table_name, const std::string& venue_table_name)
		: db(std::make_unique<Db::MySQLDb>(std::move(std::make_unique<Db::DbSettingsFileLoader>(setting_file_path)))),
		db_name(db_name), user_table_name(user_table_name), venue_table_name(venue_table_name)
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
	std::shared_ptr<std::vector<std::shared_ptr<Graph::SemanticTrajectory<Geography::LatLng>>>> DbTrajectoryLoader<Graph::SemanticTrajectory<Geography::LatLng>>::load_trajectories(unsigned int user_id, int trajectory_length_threshold)
	{
		std::stringstream query;
		query << "SELECT venue_id, timestamp, category_id, latitude, longitude FROM " << user_table_name << " INNER JOIN " << venue_table_name << " ON " << user_table_name << ".venue_id = " << venue_table_name << ".id WHERE user_id = " << user_id << " ORDER BY timestamp ASC;";
		
		std::cout << query.str() << std::endl;
		
		return execute_with_query_result(query.str(), [trajectory_length_threshold](sql::ResultSet* result) {
			
			std::shared_ptr<std::vector<std::shared_ptr<Graph::SemanticTrajectory<Geography::LatLng>>>> ret = std::make_shared<std::vector<std::shared_ptr<Graph::SemanticTrajectory<Geography::LatLng>>>>();
			
			std::unique_ptr<std::vector<std::string>> times;
			std::shared_ptr<std::vector<Graph::MapNodeIndicator>>node_ids;
			std::shared_ptr<std::vector<std::shared_ptr<Geography::LatLng>>> positions;
			std::shared_ptr<Collection::Sequence<std::string>>category_sequence;
			
			result->beforeFirst();
			std::string current_date = "";
			while (result->next()) {
				
				int venue_id = result->getInt("venue_id");
				std::string timestamp = result->getString("timestamp");
				std::string category_id = result->getString("category_id");
				double latitude = result->getDouble("latitude");
				double longitude = result->getDouble("longitude");

				std::string date = timestamp.substr(0, 10);
				if (current_date != date) {
					if (times != nullptr && times->size() > trajectory_length_threshold) {
						std::shared_ptr<Graph::SemanticTrajectory<Geography::LatLng>> trajectory = std::make_shared<Graph::SemanticTrajectory<Geography::LatLng>>(std::move(times), std::move(node_ids), std::move(positions), std::move(category_sequence));
						ret->push_back(trajectory);
					}
					current_date = date;
					times = std::make_unique<std::vector<std::string>>();
					node_ids = std::make_shared<std::vector<Graph::MapNodeIndicator>>();
					positions = std::make_shared<std::vector<std::shared_ptr<Geography::LatLng>>>();
					category_sequence = std::make_shared<Collection::Sequence<std::string>>();
				}

				times->push_back(timestamp);
				node_ids->push_back(venue_id);
				positions->push_back(std::make_shared<Geography::LatLng>(latitude, longitude));
				category_sequence->push_back(category_id);
			}
			
			return ret;
		});
	}


	///<summary>
	/// データベースからTrajectoryを読み出す
	///</summary>
	template <>
	std::shared_ptr<std::vector<std::shared_ptr<Graph::Trajectory<Geography::LatLng>>>> DbTrajectoryLoader<Graph::Trajectory<Geography::LatLng>>::load_trajectories(unsigned int user_id, int trajectory_length_threshold)
	{
		std::stringstream query;
		query << "SELECT venue_id, timestamp, latitude, longitude FROM " << user_table_name << " INNER JOIN " << venue_table_name << " ON " << user_table_name << ".venue_id = " << venue_table_name << ".id WHERE user_id = " << user_id << " ORDER BY timestamp ASC;";

		return execute_with_query_result(query.str(), [trajectory_length_threshold](sql::ResultSet* result) {

			std::shared_ptr<std::vector<std::shared_ptr<Graph::Trajectory<Geography::LatLng>>>> ret;

			std::unique_ptr<std::vector<std::string>> times;
			std::shared_ptr<std::vector<Graph::MapNodeIndicator>>node_ids;
			std::shared_ptr<std::vector<std::shared_ptr<Geography::LatLng>>> positions;

			result->beforeFirst();
			std::string current_date = "";
			while (result->next()) {

				int venue_id = result->getInt("venue_id");
				std::string timestamp = result->getString("timestamp");
				double latitude = result->getDouble("latitude");
				double longitude = result->getDouble("longitude");

				std::string date = timestamp.substr(0, 9);
				if (current_date != date) {
					if (times != nullptr && times->size() > trajectory_length_threshold) {
						std::shared_ptr<Graph::Trajectory<Geography::LatLng>> trajectory = std::make_shared<Graph::Trajectory<Geography::LatLng>>(std::move(times), std::move(node_ids), std::move(positions));
						ret->push_back(trajectory);
					}
					current_date = date;
					times = std::make_unique<std::vector<std::string>>();
					node_ids = std::make_shared<std::vector<Graph::MapNodeIndicator>>();
					positions = std::make_shared<std::vector<std::shared_ptr<Geography::LatLng>>>();
				}

				times->push_back(timestamp);
				node_ids->push_back(venue_id);
				positions->push_back(std::make_shared<Geography::LatLng>(latitude, longitude));
			}

			return ret;
		});
	}
	
}

