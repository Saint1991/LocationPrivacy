#include "stdafx.h"
#include "BaseSimulator.h"

namespace Simulation
{

	///<summary>
	/// コンストラクタ
	///</summary>
	BaseSimulator::BaseSimulator(unsigned int user_id, double trainingset_proportion) 
		: ISimulator<Map::BasicDbMap, User::BasicUser<Geography::LatLng>, Entity::Dummy<Geography::LatLng>, Requirement::BasicRequirement, Geography::LatLng, Graph::SemanticTrajectory<Geography::LatLng>>(),
		TRAININGSET_PROPORTION(trainingset_proportion), USER_ID(user_id)
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
		map = std::make_shared<Map::BasicDbMap>(std::make_shared<Graph::Dijkstra<Map::BasicMapNode, Map::BasicRoad>>(), "../settings/mydbsettings.xml", "map_tokyo");
		map->load(boundary);
	}


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

	///<summary>
	/// DBからユーザのトラジェクトリリストを作成
	///</summary>
	void BaseSimulator::create_trajectories()
	{
		User::DbTrajectoryLoader<Graph::SemanticTrajectory<Geography::LatLng>> loader(trajectory_division_rule, "../settings/mydbsettings.xml", "map_tokyo", "checkins", "pois");
		user_trajectories = loader.load_trajectories(USER_ID);
	}

	///<summary>
	/// 嗜好の木の作成 (未実装)
	///</summary>
	void BaseSimulator::build_user_preference_tree()
	{
	}


	///<summary>
	/// 前処理部分
	///</summary>
	void BaseSimulator::prepare()
	{
		make_requirement_list();
		create_trajectories();
		build_user_preference_tree();
	}
}

