#include "stdafx.h"
#include "ObservedTrajectoryNode.h"

namespace Observer
{

	///<summary>
	/// �R���X�g���N�^
	///</summary>
	ObservedTrajectoryNode::ObservedTrajectoryNode(Graph::node_id id, int depth, std::shared_ptr<Graph::MapNodeIndicator> map_node_id) 
		: Graph::Node<Graph::MapNodeIndicator, Graph::BasicEdge>(id, map_node_id), depth(depth), entity_count(1)
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
		entity_count++;
	}


	///<summary>
	/// count���擾����
	///</summary>
	size_t ObservedTrajectoryNode::get_count() const
	{
		return entity_count;
	}


	///<summary>
	/// depth���擾����
	///</summary>
	int ObservedTrajectoryNode::get_depth() const
	{
		return depth;
	}

	///<summary>
	/// �t�m�[�h���ǂ������肷��
	///</summary>
	bool ObservedTrajectoryNode::is_leaf() const
	{
		return get_connecting_node_list().size() == 0;
	}


}

