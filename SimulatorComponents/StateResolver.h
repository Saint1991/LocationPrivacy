#pragma once
#include "ObservedTrajectoryStructure.h"
#include "BasicDbMap.h"

namespace Observer
{

	///<summary>
	/// ノード間の状態から遷移確率を解く
	///</summary>
	class StateResolver
	{
	protected:
		std::shared_ptr<Map::BasicDbMap const> map;
	public:
		StateResolver(std::shared_ptr<Map::BasicDbMap> map);
		~StateResolver();
		void resolve(std::vector<std::shared_ptr<ObservedTrajectoryNode>>& current_nodes, std::vector<std::shared_ptr<ObservedTrajectoryNode>>& next_nodes);
	};
}


