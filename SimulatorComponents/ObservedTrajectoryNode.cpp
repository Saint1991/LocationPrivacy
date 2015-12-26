#include "stdafx.h"
#include "ObservedTrajectoryNode.h"

namespace Observer
{

	///<summary>
	/// コンストラクタ
	///</summary>
	ObservedTrajectoryNode::ObservedTrajectoryNode(Graph::node_id id, int depth, std::shared_ptr<Graph::MapNodeIndicator> map_node_id) 
		: Graph::Node<Graph::MapNodeIndicator, Graph::BasicEdge>(id, map_node_id), depth(depth), entity_count(1)
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
		entity_count++;
	}


	///<summary>
	/// countを取得する
	///</summary>
	size_t ObservedTrajectoryNode::get_count() const
	{
		return entity_count;
	}


	///<summary>
	/// depthを取得する
	///</summary>
	int ObservedTrajectoryNode::get_depth() const
	{
		return depth;
	}

	///<summary>
	/// 葉ノードかどうか判定する
	///</summary>
	bool ObservedTrajectoryNode::is_leaf() const
	{
		return get_connecting_node_list().size() == 0;
	}


}

