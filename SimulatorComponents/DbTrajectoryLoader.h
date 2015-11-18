#ifdef SIMULATORCOMPONENTS_EXPORTS
#define DB_TRAJECTORY_LOADER_API __declspec(dllexport)
#else
#define DB_TRAJECTORY_LOADER_API __declspec(dllimport)
#endif

#pragma once
#include "BasicUser.h"
#include "MySQLDb.h"
#include "DbSettingsFileLoader.h"
#include "SemanticTrajectory.h"
#include "Trajectory.h"
#include "LatLng.h"

namespace User
{

	///<summary>
	/// データベースからユーザデータを読み出してトラジェクトリリストを作成するユーティリティクラス
	///</summary>
	template <typename TRAJECTORY_TYPE = Graph::SemanticTrajectory<>>
	class DB_TRAJECTORY_LOADER_API DbTrajectoryLoader
	{
	private:
		std::shared_ptr<std::vector<std::shared_ptr<TRAJECTORY_TYPE>>> execute_with_query_result(const std::string& query, std::function<std::shared_ptr<std::vector<std::shared_ptr<TRAJECTORY_TYPE>>>(sql::ResultSet*)> execute_function);

	protected:
		std::function<bool(const std::string&, const std::string&)> division_rule;
		std::unique_ptr<Db::MySQLDb> db;
		std::string db_name;
		std::string user_table_name;
		std::string venue_table_name;
	public:
		DbTrajectoryLoader(std::function<bool(const std::string&, const std::string&)> division_rule, const std::string& setting_file_path, const std::string& db_name = "map_tokyo", const std::string& user_table_name = "checkins", const std::string& venue_table_name = "pois");
		~DbTrajectoryLoader();
		std::shared_ptr<std::vector<std::shared_ptr<TRAJECTORY_TYPE>>> load_trajectories(unsigned int user_id, int trajectory_length_threshold = 0);
	};

	template class DbTrajectoryLoader<Graph::SemanticTrajectory<Geography::LatLng>>;
	template class DbTrajectoryLoader<Graph::Trajectory<Geography::LatLng>>;
}


