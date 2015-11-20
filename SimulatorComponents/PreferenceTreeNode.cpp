#include "stdafx.h"
#include "PreferenceTreeNode.h"

namespace User
{

	///<summary>
	/// コンストラクタ
	///</summary>
	PreferenceTreeNode::PreferenceTreeNode(Graph::node_id id, Graph::node_id parent, const User::category_id& category) 
		: Graph::Node<User::category_id, Graph::BasicEdge>(id, std::make_shared<User::category_id>(category)), counter(0.0), parent(parent)
	{

	}


	///<summary>
	/// コンストラクタ
	///</summary>
	PreferenceTreeNode::PreferenceTreeNode(Graph::node_id id, Graph::node_id parent, std::shared_ptr<User::category_id> category) 
		: Graph::Node<User::category_id, Graph::BasicEdge>(id, category), counter(0.0), parent(parent)
	{
	}


	///<summary>
	/// コンストラクタ
	///</summary>
	PreferenceTreeNode::PreferenceTreeNode(const PreferenceTreeNode& node) 
		: Graph::Node<User::category_id, Graph::BasicEdge>(node.get_id(), std::make_shared<User::category_id>(*node.data)),  
		counter(node.count()), parent(node.get_parent())
	{
	}


	///<summary>
	/// デストラクタ
	///</summary>
	PreferenceTreeNode::~PreferenceTreeNode()
	{
	}


	///<summary>
	/// CategoryIDの取得
	///</summary>
	const User::category_id PreferenceTreeNode::category_id() const
	{
		return *data;
	}


	///<summary>
	/// 訪問回数を取得する
	///</summary>
	int PreferenceTreeNode::count() const
	{
		return counter;
	}


	///<summary>
	/// 訪問回数をインクリメントする
	///</summary>
	void PreferenceTreeNode::count_up(double add_num)
	{
		counter += add_num;
	}


	///<summary>
	/// 葉ノードか判定
	///</summary>
	bool PreferenceTreeNode::is_leef() const
	{
		return edge_list->size() == 0;
	}


	///<summary>
	/// 親ノードを取得
	///</summary>
	Graph::node_id PreferenceTreeNode::get_parent() const
	{
		return parent;
	}

	///<summary>
	/// 子ノードのIDリストを取得する
	///</summary>
	std::vector<Graph::node_id> PreferenceTreeNode::get_children() const
	{
		return get_connecting_node_list();
	}
}

