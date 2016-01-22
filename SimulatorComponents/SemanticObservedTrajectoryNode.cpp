#include "stdafx.h"
#include "SemanticObservedTrajectoryNode.h"

namespace Observer
{
	

	///<summary>
	/// コンストラクタ
	///</summary>
	SemanticObservedTrajectoryNode::SemanticObservedTrajectoryNode(Graph::node_id id, int depth, std::shared_ptr<Graph::MapNodeIndicator> map_node_id) 
		: ObservedTrajectoryNode(id, depth, map_node_id), flow_in_map(std::make_shared<std::unordered_map<Graph::node_id, double>>())
	{
	}


	///<summary>
	/// デストラクタ
	///</summary>
	SemanticObservedTrajectoryNode::~SemanticObservedTrajectoryNode()
	{
	}


	///<summary>
	/// fromノードからの流入を追加する
	///</summary>
	void SemanticObservedTrajectoryNode::flow_in_from(Graph::node_id from, double flow)
	{
		std::unordered_map<Graph::node_id, double>::iterator iter = flow_in_map->find(from);
		if (iter == flow_in_map->end()) 	flow_in_map->insert(std::make_pair(from, flow));
		else {
			double new_value = iter->second + flow;
			flow_in_map->at(from) = new_value;
		}
	}


	///<summary>
	/// fromからのflowを取得する
	///</summary>
	double SemanticObservedTrajectoryNode::get_flow_from(Graph::node_id from) const
	{
		std::unordered_map<Graph::node_id, double>::iterator iter = flow_in_map->find(from);
		return iter == flow_in_map->end() ? 0.0 : iter->second;
	}


	///<summary>
	/// そのノードに流入しているflowの合計値を返す
	///</summary>
	double SemanticObservedTrajectoryNode::get_total_flow() const
	{
		double ret = 0.0;
		for (std::unordered_map<Graph::node_id, double>::const_iterator iter = flow_in_map->begin(); iter != flow_in_map->end(); iter++) {
			ret += iter->second;
		}
		return ret;
	}

}
