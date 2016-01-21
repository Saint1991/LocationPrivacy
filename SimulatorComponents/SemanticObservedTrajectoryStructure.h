#pragma once
#include "Tree.h"
#include "FlowEdge.h"
#include "SemanticObservedTrajectoryNode.h"
#include "Sequence.h"

namespace Observer
{

	///<summary>
	/// MTC2計算のための観測トラジェクトリモデル
	///</summary>
	class SemanticObservedTrajectoryStructure : public Graph::Tree<SemanticObservedTrajectoryNode, Graph::MapNodeIndicator, Graph::FlowEdge, double>
	{
	public:
		SemanticObservedTrajectoryStructure();
		SemanticObservedTrajectoryStructure(const SemanticObservedTrajectoryStructure& structure);
		~SemanticObservedTrajectoryStructure();

		void initialize(std::shared_ptr<SemanticObservedTrajectoryNode> root_node);
		base_iterator find_node(const Graph::MapNodeIndicator& id, int phase);
		Graph::node_id find_node_id(const Graph::MapNodeIndicator& id, int phase) const;
	};
}


