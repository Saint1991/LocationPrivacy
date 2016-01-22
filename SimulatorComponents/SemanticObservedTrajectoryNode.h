#ifdef SIMULATORCOMPONENTS_EXPORTS
#define SEMANTIC_OBSERVED_TRAJECTORY_NODE_API __declspec(dllexport)
#else
#define SEMANTIC_OBSERVED_TRAJECTORY_NODE_API __declspec(dllimport)
#endif
#pragma once
#include "ObservedTrajectoryNode.h"

namespace Observer
{

	///<summary>
	///
	///</summary>
	class SEMANTIC_OBSERVED_TRAJECTORY_NODE_API SemanticObservedTrajectoryNode : public ObservedTrajectoryNode
	{
	protected:
		std::shared_ptr<std::unordered_map<Graph::node_id, double>> flow_in_map;
	
	public:
		SemanticObservedTrajectoryNode(Graph::node_id id, int depth, std::shared_ptr<Graph::MapNodeIndicator> map_node_id);
		~SemanticObservedTrajectoryNode();

		void flow_in_from(Graph::node_id from, double flow);
		double get_flow_from(Graph::node_id from) const;
		double get_total_flow() const;
	};
}


