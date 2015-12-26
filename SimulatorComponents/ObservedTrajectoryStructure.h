#pragma once
#include "Tree.h"
#include "BasicEdge.h"
#include "MapNodeIndicator.h"
#include "ObservedTrajectoryNode.h"

namespace Observer
{
	class ObservedTrajectoryStructure : public Graph::Tree<ObservedTrajectoryNode, Graph::MapNodeIndicator, Graph::BasicEdge>
	{
	public:
		ObservedTrajectoryStructure();
		~ObservedTrajectoryStructure();

		void initialize(std::shared_ptr<ObservedTrajectoryNode> root_node);
		base_iterator find_node(const Graph::MapNodeIndicator& id, int phase);
		Graph::node_id find_node_id(const Graph::MapNodeIndicator& id, int phase) const;
	};
}



