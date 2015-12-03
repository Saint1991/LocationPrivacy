#include "stdafx.h"
#include "PreferenceTreeNode.h"

namespace User
{

	///<summary>
	/// コンストラクタ
	///</summary>
	PreferenceTreeNode::PreferenceTreeNode(Graph::node_id id, Graph::node_id parent, unsigned int depth, const User::category_id& category) 
		: Graph::SequentialTreeNode<User::category_id>(id, parent, depth, std::make_shared<User::category_id>(category)), counter(0.0)
	{

	}


	///<summary>
	/// コンストラクタ
	///</summary>
	PreferenceTreeNode::PreferenceTreeNode(Graph::node_id id, Graph::node_id parent, unsigned int depth, std::shared_ptr<User::category_id> category) 
		: Graph::SequentialTreeNode<User::category_id>(id, parent, depth, category), counter(0.0)
	{
	}


	///<summary>
	/// コンストラクタ
	///</summary>
	PreferenceTreeNode::PreferenceTreeNode(const PreferenceTreeNode& node) 
		: Graph::SequentialTreeNode<User::category_id>(node), counter(node.count())
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

}

