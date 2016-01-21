#ifdef SIMULATORCOMPONENTS_EXPORTS
#define OBSERVED_TRAJECTORY_NODE_API __declspec(dllexport)
#else
#define OBSERVED_TRAJECTORY_NODE_API __declspec(dllimport)
#endif

#pragma once
#include "Node.h"
#include "MapNodeIndicator.h"
#include "FlowEdge.h"

namespace Observer
{
	class OBSERVED_TRAJECTORY_NODE_API ObservedTrajectoryNode : public Graph::Node<Graph::MapNodeIndicator, Graph::FlowEdge>
	{
	protected:
		int depth;
		size_t entity_count;

	public:
		ObservedTrajectoryNode(Graph::node_id id, int depth, std::shared_ptr<Graph::MapNodeIndicator> map_node_id);
		~ObservedTrajectoryNode();

		void count_up();
		size_t get_count() const;

		void clear_count();
		int get_depth() const;
		void set_depth(int depth);

		void flow_out_to(const Graph::node_id to, double flow = 1.0);
		bool is_leaf() const;
	};
}


