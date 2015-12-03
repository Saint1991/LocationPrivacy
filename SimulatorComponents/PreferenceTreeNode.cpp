#include "stdafx.h"
#include "PreferenceTreeNode.h"

namespace User
{

	///<summary>
	/// �R���X�g���N�^
	///</summary>
	PreferenceTreeNode::PreferenceTreeNode(Graph::node_id id, Graph::node_id parent, unsigned int depth, const User::category_id& category) 
		: Graph::SequentialTreeNode<User::category_id>(id, parent, depth, std::make_shared<User::category_id>(category)), counter(0.0)
	{

	}


	///<summary>
	/// �R���X�g���N�^
	///</summary>
	PreferenceTreeNode::PreferenceTreeNode(Graph::node_id id, Graph::node_id parent, unsigned int depth, std::shared_ptr<User::category_id> category) 
		: Graph::SequentialTreeNode<User::category_id>(id, parent, depth, category), counter(0.0)
	{
	}


	///<summary>
	/// �R���X�g���N�^
	///</summary>
	PreferenceTreeNode::PreferenceTreeNode(const PreferenceTreeNode& node) 
		: Graph::SequentialTreeNode<User::category_id>(node), counter(node.count())
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
	/// null�̏ꍇ�͋󕶎����Ԃ�
	///</summary>
	const User::category_id PreferenceTreeNode::category_id() const
	{
		if (data == nullptr) return "";
		return *data;
	}


	///<summary>
	/// �K��񐔂��擾����
	///</summary>
	double PreferenceTreeNode::count() const
	{
		return counter;
	}


	///<summary>
	/// �K��񐔂��C���N�������g����
	///</summary>
	void PreferenceTreeNode::count_up(double add_num)
	{
		counter += add_num;
	}

}

