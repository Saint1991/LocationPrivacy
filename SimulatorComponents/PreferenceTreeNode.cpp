#include "stdafx.h"
#include "PreferenceTreeNode.h"

namespace User
{

	///<summary>
	/// �R���X�g���N�^
	///</summary>
	PreferenceTreeNode::PreferenceTreeNode(Graph::node_id id, User::category_id category) : Graph::Node<User::category_id, ProbabilityEdge>(id, std::make_shared<User::category_id>(category)), counter(0)
	{
	}


	///<summary>
	/// �R���X�g���N�^
	///</summary>
	PreferenceTreeNode::PreferenceTreeNode(const PreferenceTreeNode& node) : Graph::Node<User::category_id, ProbabilityEdge>(node),  counter(0)
	{
	}


	///<summary>
	/// �f�X�g���N�^
	///</summary>
	PreferenceTreeNode::~PreferenceTreeNode()
	{
	}


	///<summary>
	/// CategoryID�̎擾
	///</summary>
	const User::category_id PreferenceTreeNode::category_id() const
	{
		return *data;
	}
}

