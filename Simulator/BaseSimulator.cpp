#include "stdafx.h"
#include "BaseSimulator.h"

namespace Simulation
{

	///<summary>
	/// コンストラクタ
	///</summary>
	BaseSimulator::BaseSimulator(unsigned int user_id, const std::string& db_name) 
		: ISimulator<Map::BasicDbMap, User::BasicUser<Geography::LatLng>, Entity::Dummy<Geography::LatLng>, Requirement::PreferenceRequirement, Geography::LatLng, Graph::SemanticTrajectory<Geography::LatLng>>(),
		USER_ID(user_id), DB_NAME(db_name),
		user_preference_tree(std::make_shared<User::PreferenceTree>()),
		observed_preference_tree(std::make_shared<User::PreferenceTree>())
	{
	}


	///<summary>
	/// デストラクタ
	///</summary>
	BaseSimulator::~BaseSimulator()
	{
	}


	///<summary>
	/// Mapの構成
	/// map_tokyoのDBからデータを読み出して，WarshallFloyd法でルーティングテーブルを作成する
	///</summary>
	void BaseSimulator::build_map(const Graph::Rectangle<Geography::LatLng>& boundary)
	{
		map = std::make_shared<Map::BasicDbMap>(std::make_shared<Graph::Dijkstra<Map::BasicMapNode, Map::BasicRoad>>(), "../settings/mydbsettings.xml", DB_NAME);
		map->load(boundary);
	}


	///<summary>
	/// トラジェクトリ分割のルール
	/// 経過時刻が2時間半以上のところでトラジェクトリを分割する
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

	///<summary>
	/// DBからユーザのトラジェクトリリストを作成
	///</summary>
	void BaseSimulator::create_trajectories()
	{
		User::DbTrajectoryLoader<Graph::SemanticTrajectory<Geography::LatLng>> loader(trajectory_division_rule, "../settings/mydbsettings.xml", DB_NAME, "checkins_converted", "pois");
		user_trajectories = loader.load_trajectories(USER_ID, TRAJECTORY_LENGTH_THRESHOLD, MIN_SERVICE_INTERVAL);

		size_t trajectory_size = user_trajectories->size();
		std::cout << "Create " << trajectory_size << " Trajectories." << std::endl;
	}

	///<summary
	/// 嗜好の木の作成
	/// 全トラジェクトリのデータを使ってユーザの嗜好の木を作成する
	///</summary>
	void BaseSimulator::build_user_preference_tree()
	{
		for (unsigned int trajectory_id = 0; trajectory_id < user_trajectories->size(); trajectory_id++) {
			std::shared_ptr<Graph::SemanticTrajectory<Geography::LatLng>> trajectory = user_trajectories->at(trajectory_id);
			int trajectory_length = trajectory->phase_count();
			Collection::Sequence<std::string> category_sequence = trajectory->get_category_sequence(0, trajectory_length - 1);
			user_preference_tree->add_sequence_counter(category_sequence);
		}
	}


	///<summary>
	/// 前処理部分
	///</summary>
	void BaseSimulator::prepare()
	{
		make_requirement_list();
		build_map();
		create_trajectories();
		build_user_preference_tree();
	}
}

