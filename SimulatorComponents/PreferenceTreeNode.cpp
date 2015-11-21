#include "stdafx.h"
#include "PreferenceTreeNode.h"

namespace User
{

	///<summary>
	/// �R���X�g���N�^
	///</summary>
	PreferenceTreeNode::PreferenceTreeNode(Graph::node_id id, Graph::node_id parent, unsigned int depth, const User::category_id& category) 
		: Graph::Node<User::category_id, Graph::BasicEdge>(id, std::make_shared<User::category_id>(category)), counter(0.0), parent(parent), depth(depth)
	{

	}


	///<summary>
	/// �R���X�g���N�^
	///</summary>
	PreferenceTreeNode::PreferenceTreeNode(Graph::node_id id, Graph::node_id parent, unsigned int depth, std::shared_ptr<User::category_id> category) 
		: Graph::Node<User::category_id, Graph::BasicEdge>(id, category), counter(0.0), parent(parent), depth(depth)
	{
	}


	///<summary>
	/// �R���X�g���N�^
	///</summary>
	PreferenceTreeNode::PreferenceTreeNode(const PreferenceTreeNode& node) 
		: Graph::Node<User::category_id, Graph::BasicEdge>(node.get_id(), std::make_shared<User::category_id>(*node.data)),  
		counter(node.count()), parent(node.get_parent()), depth(node.get_depth())
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
	/// �[�����擾���� (���[�g�m�[�h�͐[��0����)
	///</summary>
	int PreferenceTreeNode::get_depth() const
	{
		return depth;
	}

	///<summary>
	/// �q�m�[�h��ID���X�g���擾����
	///</summary>
	std::vector<Graph::node_id> PreferenceTreeNode::get_children() const
	{
		return get_connecting_node_list();
	}
}

