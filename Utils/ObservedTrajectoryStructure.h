#ifdef UTILS_EXPORTS
#define OBSERVED_TRAJECTORY_STRUCTURE_API __declspec(dllexport)
#else
#define OBSERVED_TRAJECTORY_STRUCTURE_API __declspec(dllimport)
#endif

#pragma once
#include "Tree.h"
#include "BasicEdge.h"
#include "MapNodeIndicator.h"
#include "ObservedTrajectoryNode.h"

namespace Observer
{
	class OBSERVED_TRAJECTORY_STRUCTURE_API ObservedTrajectoryStructure : public Graph::Tree<ObservedTrajectoryNode, Graph::MapNodeIndicator, Graph::BasicEdge>
	{
	public:
		ObservedTrajectoryStructure();
		~ObservedTrajectoryStructure();

		void initialize(std::shared_ptr<ObservedTrajectoryNode> root_node);
		base_iterator find_node(const Graph::MapNodeIndicator& id, int phase);
		Graph::node_id find_node_id(const Graph::MapNodeIndicator& id, int phase) const;

		std::shared_ptr<ObservedTrajectoryStructure> sub_structure(Graph::node_id root_node_id) const;
	};
}



