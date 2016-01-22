#include "stdafx.h"
#include "SemanticObservedTrajectoryNode.h"

namespace Observer
{
	

	///<summary>
	/// �R���X�g���N�^
	///</summary>
	SemanticObservedTrajectoryNode::SemanticObservedTrajectoryNode(Graph::node_id id, int depth, std::shared_ptr<Graph::MapNodeIndicator> map_node_id) 
		: ObservedTrajectoryNode(id, depth, map_node_id), flow_in_map(std::make_shared<std::unordered_map<Graph::node_id, double>>())
	{
	}


	///<summary>
	/// �f�X�g���N�^
	///</summary>
	SemanticObservedTrajectoryNode::~SemanticObservedTrajectoryNode()
	{
	}


	///<summary>
	/// from�m�[�h����̗�����ǉ�����
	///</summary>
	void SemanticObservedTrajectoryNode::flow_in_from(Graph::node_id from, double flow)
	{
		std::unordered_map<Graph::node_id, double>::iterator iter = flow_in_map->find(from);
		if (iter == flow_in_map->end()) 	flow_in_map->insert(std::make_pair(from, flow));
		else {
			double new_value = iter->second + flow;
			flow_in_map->at(from) = new_value;
		}
	}


	///<summary>
	/// from�����flow���擾����
	///</summary>
	double SemanticObservedTrajectoryNode::get_flow_from(Graph::node_id from) const
	{
		std::unordered_map<Graph::node_id, double>::iterator iter = flow_in_map->find(from);
		return iter == flow_in_map->end() ? 0.0 : iter->second;
	}


	///<summary>
	/// ���̃m�[�h�ɗ������Ă���flow�̍��v�l��Ԃ�
	///</summary>
	double SemanticObservedTrajectoryNode::get_total_flow() const
	{
		double ret = 0.0;
		for (std::unordered_map<Graph::node_id, double>::const_iterator iter = flow_in_map->begin(); iter != flow_in_map->end(); iter++) {
			ret += iter->second;
		}
		return ret;
	}

}
