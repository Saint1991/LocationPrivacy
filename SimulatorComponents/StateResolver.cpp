#include "stdafx.h"
#include "StateResolver.h"

namespace Observer
{


	///<summary>
	/// コンストラクタ
	///</summary>
	StateResolver::StateResolver(std::shared_ptr<Map::BasicDbMap> map) : map(map)
	{

	}


	///<summary>
	/// デストラクタ
	///</summary>
	StateResolver::~StateResolver()
	{

	}

	///<summary>
	/// ノードと観測されたエンティティ数の変化情報から遷移確率を解いてエッジの値を更新
	///</summary>
	void StateResolver::resolve(std::vector<std::shared_ptr<ObservedTrajectoryNode>>& current_state_nodes, std::vector<std::shared_ptr<ObservedTrajectoryNode>>& next_state_nodes)
	{

	}
}

