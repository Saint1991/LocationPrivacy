#include "stdafx.h"
#include "BaseSimulator.h"

namespace Simulation
{

	///<summary>
	/// コンストラクタ
	///</summary>
	BaseSimulator::BaseSimulator(double testset_proportion) 
		: ISimulator<Map::BasicDbMap, User::BasicUser<Geography::LatLng>, Entity::Dummy<Geography::LatLng>, Requirement::BasicRequirement, Geography::LatLng, Graph::SemanticTrajectory<Geography::LatLng>>(),
		testset_proportion(testset_proportion)
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
	void BaseSimulator::build_map()
	{
		map = std::make_shared<Map::BasicDbMap>("../settings/mydbsettings.xml", "map_tokyo");
		map->initialize(std::move(std::make_unique<Graph::WarshallFloyd<Map::BasicMapNode, Map::BasicRoad>>()));
	}


	///<summary>
	/// DBからユーザのトラジェクトリリストを作成
	///</summary>
	void BaseSimulator::create_trajectories(unsigned int user_id)
	{
		User::DbTrajectoryLoader<Graph::SemanticTrajectory<Geography::LatLng>> loader("../settings/mydbsettings.xml", "map_tokyo", "checkins", "pois");
		user_trajectories = loader.load_trajectories(user_id);
		first_trajectory_sequence = (int)(user_trajectories->size() * testset_proportion) + 1;
	}

	///<summary>
	/// 嗜好の木の作成
	///</summary>
	void BaseSimulator::build_user_preference_tree()
	{
		for (int trajectory_sequence = 0; trajectory_sequence < first_trajectory_sequence; trajectory_sequence++) {
			std::shared_ptr<Graph::SemanticTrajectory<>> trajectory = user_trajectories->at(trajectory_sequence);
		}
	}
}

