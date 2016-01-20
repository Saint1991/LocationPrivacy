#include "stdafx.h"
#include "ObservedTrajectoryNode.h"

namespace Observer
{

	///<summary>
	/// コンストラクタ
	///</summary>
	ObservedTrajectoryNode::ObservedTrajectoryNode(Graph::node_id id, int depth, std::shared_ptr<Graph::MapNodeIndicator> map_node_id)
		: Graph::Node<Graph::MapNodeIndicator, Graph::FlowEdge>(id, map_node_id), depth(depth), entity_count(0)
	{
	}


	///<summary>
	/// デストラクタ
	///</summary>
	ObservedTrajectoryNode::~ObservedTrajectoryNode()
	{
	}


	///<summary>
	/// entity_countを1カウントアップする
	///</summary>
	void ObservedTrajectoryNode::count_up()
	{
		entity_count += 1.0;
	}


	///<summary>
	/// countを取得する
	///</summary>
	double ObservedTrajectoryNode::get_count() const
	{
		return entity_count;
	}

	void ObservedTrajectoryNode::add_count(double add)
	{
		entity_count += add;
	}

	///<summary>
	/// EntityCountを0に戻す
	///</summary>
	void ObservedTrajectoryNode::clear_count()
	{
		entity_count = 0;
	}


	///<summary>
	/// depthを取得する
	///</summary>
	int ObservedTrajectoryNode::get_depth() const
	{
		return depth;
	}


	///<summary>
	/// Depthを再設定する
	///</summary>
	void ObservedTrajectoryNode::set_depth(int depth)
	{
		this->depth = depth;
	}


	///<summary>
	/// toのノードへ向かってFlowの量がflowのエッジを作成する
	/// 既存のエッジの場合はflowの量をflowだけ増加させる
	///</summary>
	void ObservedTrajectoryNode::flow_out_to(Graph::node_id to, double flow)
	{
		std::shared_ptr<Graph::FlowEdge> edge = get_edge_to(to);
		if (edge == nullptr) connect_to(std::make_shared<Graph::FlowEdge>(to, flow));
		else edge->flow_in(flow);
	}

	///<summary>
	/// 葉ノードかどうか判定する
	///</summary>
	bool ObservedTrajectoryNode::is_leaf() const
	{
		return get_connecting_node_list().size() == 0;
	}

}

