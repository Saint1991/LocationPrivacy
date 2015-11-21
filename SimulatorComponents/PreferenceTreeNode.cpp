#include "stdafx.h"
#include "PreferenceTreeNode.h"

namespace User
{

	///<summary>
	/// コンストラクタ
	///</summary>
	PreferenceTreeNode::PreferenceTreeNode(Graph::node_id id, Graph::node_id parent, unsigned int depth, const User::category_id& category) 
		: Graph::Node<User::category_id, Graph::BasicEdge>(id, std::make_shared<User::category_id>(category)), counter(0.0), parent(parent), depth(depth)
	{

	}


	///<summary>
	/// コンストラクタ
	///</summary>
	PreferenceTreeNode::PreferenceTreeNode(Graph::node_id id, Graph::node_id parent, unsigned int depth, std::shared_ptr<User::category_id> category) 
		: Graph::Node<User::category_id, Graph::BasicEdge>(id, category), counter(0.0), parent(parent), depth(depth)
	{
	}


	///<summary>
	/// コンストラクタ
	///</summary>
	PreferenceTreeNode::PreferenceTreeNode(const PreferenceTreeNode& node) 
		: Graph::Node<User::category_id, Graph::BasicEdge>(node.get_id(), std::make_shared<User::category_id>(*node.data)),  
		counter(node.count()), parent(node.get_parent()), depth(node.get_depth())
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
	/// nullの場合は空文字列を返す
	///</summary>
	const User::category_id PreferenceTreeNode::category_id() const
	{
		if (data == nullptr) return "";
		return *data;
	}


	///<summary>
	/// 訪問回数を取得する
	///</summary>
	double PreferenceTreeNode::count() const
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
	/// 深さを取得する (ルートノードは深さ0扱い)
	///</summary>
	int PreferenceTreeNode::get_depth() const
	{
		return depth;
	}

	///<summary>
	/// 子ノードのIDリストを取得する
	///</summary>
	std::vector<Graph::node_id> PreferenceTreeNode::get_children() const
	{
		return get_connecting_node_list();
	}
}

