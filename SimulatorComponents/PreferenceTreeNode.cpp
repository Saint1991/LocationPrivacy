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
	PreferenceTreeNode::PreferenceTreeNode(Graph::node_id id, std::shared_ptr<User::category_id> category) : Graph::Node<User::category_id, ProbabilityEdge>(id, category), counter(0)
	{

	}


	///<summary>
	/// �R���X�g���N�^
	///</summary>
	PreferenceTreeNode::PreferenceTreeNode(const PreferenceTreeNode& node) : Graph::Node<User::category_id, ProbabilityEdge>(node.get_id(), std::make_shared<User::category_id>(*node.data)),  counter(0)
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


	///<summary>
	/// �K��񐔂��擾����
	///</summary>
	int PreferenceTreeNode::visit_count() const
	{
		return counter;
	}


	///<summary>
	/// �K��񐔂��C���N�������g����
	///</summary>
	void PreferenceTreeNode::count_up()
	{
		counter++;
	}
}

