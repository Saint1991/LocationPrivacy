#include "stdafx.h"
#include "PreferenceTreeNode.h"

namespace User
{

	///<summary>
	/// コンストラクタ
	///</summary>
	PreferenceTreeNode::PreferenceTreeNode(Graph::node_id id, User::category_id category) : Graph::Node<User::category_id, ProbabilityEdge>(id, std::make_shared<User::category_id>(category)), counter(0)
	{
	}


	///<summary>
	/// コンストラクタ
	///</summary>
	PreferenceTreeNode::PreferenceTreeNode(Graph::node_id id, std::shared_ptr<User::category_id> category) : Graph::Node<User::category_id, ProbabilityEdge>(id, category), counter(0)
	{

	}


	///<summary>
	/// コンストラクタ
	///</summary>
	PreferenceTreeNode::PreferenceTreeNode(const PreferenceTreeNode& node) : Graph::Node<User::category_id, ProbabilityEdge>(node.get_id(), std::make_shared<User::category_id>(*node.data)),  counter(0)
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
	int PreferenceTreeNode::visit_count() const
	{
		return counter;
	}


	///<summary>
	/// 訪問回数をインクリメントする
	///</summary>
	void PreferenceTreeNode::count_up()
	{
		counter++;
	}
}

