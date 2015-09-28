#include "stdafx.h"
#include "PreferenceTreeNode.h"

namespace User
{

	///<summary>
	/// �R���X�g���N�^
	///</summary>
	PreferenceTreeNode::PreferenceTreeNode(Graph::node_id id, std::string category) : Graph::Node<std::string, ProbabilityEdge>(id, std::make_shared<std::string>(category)), counter(0)
	{
	}


	///<summary>
	/// �R���X�g���N�^
	///</summary>
	PreferenceTreeNode::PreferenceTreeNode(const PreferenceTreeNode& node) : Graph::Node<std::string, ProbabilityEdge>(node),  counter(0)
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
	const std::string PreferenceTreeNode::category_id() const
	{
		return *data;
	}
}

