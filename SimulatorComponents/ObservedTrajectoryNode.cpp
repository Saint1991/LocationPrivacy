#include "stdafx.h"
#include "ObservedTrajectoryNode.h"

namespace Observer
{

	///<summary>
	/// �R���X�g���N�^
	///</summary>
	ObservedTrajectoryNode::ObservedTrajectoryNode(Graph::node_id id, int depth, std::shared_ptr<Graph::MapNodeIndicator> map_node_id)
		: Graph::Node<Graph::MapNodeIndicator, Graph::FlowEdge>(id, map_node_id), depth(depth), entity_count(0)
	{
	}


	///<summary>
	/// �f�X�g���N�^
	///</summary>
	ObservedTrajectoryNode::~ObservedTrajectoryNode()
	{
	}


	///<summary>
	/// entity_count��1�J�E���g�A�b�v����
	///</summary>
	void ObservedTrajectoryNode::count_up()
	{
		entity_count += 1.0;
	}


	///<summary>
	/// count���擾����
	///</summary>
	double ObservedTrajectoryNode::get_count() const
	{
		return entity_count;
	}

	void ObservedTrajectoryNode::add_count(double add)
	{
		entity_count += add;
	}

	///<summary>
	/// EntityCount��0�ɖ߂�
	///</summary>
	void ObservedTrajectoryNode::clear_count()
	{
		entity_count = 0;
	}


	///<summary>
	/// depth���擾����
	///</summary>
	int ObservedTrajectoryNode::get_depth() const
	{
		return depth;
	}


	///<summary>
	/// Depth���Đݒ肷��
	///</summary>
	void ObservedTrajectoryNode::set_depth(int depth)
	{
		this->depth = depth;
	}


	///<summary>
	/// to�̃m�[�h�֌�������Flow�̗ʂ�flow�̃G�b�W���쐬����
	/// �����̃G�b�W�̏ꍇ��flow�̗ʂ�flow��������������
	///</summary>
	void ObservedTrajectoryNode::flow_out_to(Graph::node_id to, double flow)
	{
		std::shared_ptr<Graph::FlowEdge> edge = get_edge_to(to);
		if (edge == nullptr) connect_to(std::make_shared<Graph::FlowEdge>(to, flow));
		else edge->flow_in(flow);
	}

	///<summary>
	/// �t�m�[�h���ǂ������肷��
	///</summary>
	bool ObservedTrajectoryNode::is_leaf() const
	{
		return get_connecting_node_list().size() == 0;
	}

}

