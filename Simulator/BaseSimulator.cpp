#include "stdafx.h"
#include "BaseSimulator.h"

namespace Simulation
{

	///<summary>
	/// コンストラクタ
	///</summary>
	BaseSimulator::BaseSimulator() 
		: ISimulator<Map::BasicDbMap, User::BasicUser<Geography::LatLng>, Entity::Dummy<Geography::LatLng>, Requirement::BasicRequirement>()
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
	/// ユーザの作成
	///</summary>
	void BaseSimulator::create_user(unsigned int user_id)
	{
		User::DbBasicUserLoader loader("../settings/mydbsettings.xml", "map_tokyo", "checkins", "pois");
		user = std::move(loader.load_user(user_id));
	}
}

