#ifdef SIMULATORCOMPONENTS_EXPORTS
#define OBSERVED_TRAJECTORY_NODE_API __declspec(dllexport)
#else
#define OBSERVED_TRAJECTORY_NODE_API __declspec(dllimport)
#endif

#pragma once
#include "Node.h"
#include "MapNodeIndicator.h"
#include "BasicEdge.h"

namespace Observer
{
	class OBSERVED_TRAJECTORY_NODE_API ObservedTrajectoryNode : public Graph::Node<Graph::MapNodeIndicator, Graph::BasicEdge>
	{
	protected:
		int depth;
		size_t entity_count;

	public:
		ObservedTrajectoryNode(Graph::node_id id, int depth, std::shared_ptr<Graph::MapNodeIndicator> map_node_id);
		~ObservedTrajectoryNode();

		void count_up();
		size_t get_count() const;
		int get_depth() const;

		bool is_leaf() const;
	};
}


