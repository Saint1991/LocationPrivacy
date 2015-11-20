#include "stdafx.h"
#include "PreferenceTreeNode.h"

namespace User
{

	///<summary>
	/// �R���X�g���N�^
	///</summary>
	PreferenceTreeNode::PreferenceTreeNode(Graph::node_id id, Graph::node_id parent, const User::category_id& category) 
		: Graph::Node<User::category_id, Graph::BasicEdge>(id, std::make_shared<User::category_id>(category)), counter(0.0), parent(parent)
	{

	}


	///<summary>
	/// �R���X�g���N�^
	///</summary>
	PreferenceTreeNode::PreferenceTreeNode(Graph::node_id id, Graph::node_id parent, std::shared_ptr<User::category_id> category) 
		: Graph::Node<User::category_id, Graph::BasicEdge>(id, category), counter(0.0), parent(parent)
	{
	}


	///<summary>
	/// �R���X�g���N�^
	///</summary>
	PreferenceTreeNode::PreferenceTreeNode(const PreferenceTreeNode& node) 
		: Graph::Node<User::category_id, Graph::BasicEdge>(node.get_id(), std::make_shared<User::category_id>(*node.data)),  
		counter(node.count()), parent(node.get_parent())
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
	int PreferenceTreeNode::count() const
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


	///<summary>
	/// �t�m�[�h������
	///</summary>
	bool PreferenceTreeNode::is_leef() const
	{
		return edge_list->size() == 0;
	}


	///<summary>
	/// �e�m�[�h���擾
	///</summary>
	Graph::node_id PreferenceTreeNode::get_parent() const
	{
		return parent;
	}

	///<summary>
	/// �q�m�[�h��ID���X�g���擾����
	///</summary>
	std::vector<Graph::node_id> PreferenceTreeNode::get_children() const
	{
		return get_connecting_node_list();
	}
}

