#pragma once
#include "stdafx.h"
#include "Node.h"
#include "BasicEdge.h"

namespace Graph
{

	///<summary>
	/// SequentialTreeに用いるノードのテンプレート
	///</summary>
	template <typename DATA>
	class SequentialTreeNode : public Node<DATA, Graph::BasicEdge>
	{
	protected:
		Graph::node_id parent;
		int depth;

	public:	
		SequentialTreeNode(node_id id, node_id parent, int depth, std::shared_ptr<DATA> data) : Graph::Node<DATA, BasicEdge>(id, data), parent(parent), depth(depth) {}
		SequentialTreeNode(const SequentialTreeNode& node) : Graph::Node<DATA, BasicEdge>(node), parent(node.get_parent()), depth(node.get_depth()) {}
		virtual ~SequentialTreeNode() {}
		Graph::node_id get_parent() const { return parent; }
		int get_depth() const { return depth; }
		bool is_leaf() const { return edge_list->size() == 0; }
		std::vector<Graph::node_id> get_children() const { return get_connecting_node_list(); }
	};
}


