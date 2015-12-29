#ifdef SIMULATORCOMPONENTS_EXPORTS
#define OBSERVED_TRAJECTORY_STRUCTURE_API __declspec(dllexport)
#else
#define OBSERVED_TRAJECTORY_STRUCTURE_API __declspec(dllimport)
#endif

#pragma once
#include "Tree.h"
#include "FlowEdge.h"
#include "MapNodeIndicator.h"
#include "ObservedTrajectoryNode.h"
#include "Sequence.h"

namespace Observer
{

	///<summary>
	/// 観測トラジェクトリを表す階層構造
	///</summary>
	class OBSERVED_TRAJECTORY_STRUCTURE_API ObservedTrajectoryStructure : public Graph::Tree<ObservedTrajectoryNode, Graph::MapNodeIndicator, Graph::FlowEdge, double>
	{

	public:
		ObservedTrajectoryStructure();
		~ObservedTrajectoryStructure();

		void initialize(std::shared_ptr<ObservedTrajectoryNode> root_node);
		base_iterator find_node(const Graph::MapNodeIndicator& id, int phase);
		Graph::node_id find_node_id(const Graph::MapNodeIndicator& id, int phase) const;

		size_t trajectory_length() const;

		std::shared_ptr<ObservedTrajectoryStructure> sub_structure(Graph::node_id root_node_id) const;
		std::shared_ptr<ObservedTrajectoryStructure> sub_structure(const Graph::MapNodeIndicator& map_node_id, int phase) const;

		void depth_first_iteration(Graph::node_id start_node_id, const std::function<void(std::shared_ptr<ObservedTrajectoryNode>)>& execute_function);
		void breadth_first_iteration(Graph::node_id start_node_id, const std::function<void(std::shared_ptr<ObservedTrajectoryNode>)>& execute_function);

		void for_each_possible_trajectory(const std::function<void(const Collection::Sequence<Graph::MapNodeIndicator>&)>& execute_function) const;
		std::vector<Collection::Sequence<Graph::MapNodeIndicator>> get_all_possible_trajectories() const;

		double calc_probability_of_trajectory(const std::vector<Graph::MapNodeIndicator>& trajectory) const;
	};
}
