#include "stdafx.h"
#include "SemanticObservedTrajectoryStructure.h"


namespace Observer
{

	///<summary>
	/// �R���X�g���N�^
	///</summary>
	SemanticObservedTrajectoryStructure::SemanticObservedTrajectoryStructure() : Graph::Tree<SemanticObservedTrajectoryNode, Graph::MapNodeIndicator, Graph::FlowEdge, double>()
	{
		initialize(std::make_shared<SemanticObservedTrajectoryNode>(0, 0, nullptr));
	}


	///<summary>
	/// �R�s�[�R���X�g���N�^
	///</summary>
	SemanticObservedTrajectoryStructure::SemanticObservedTrajectoryStructure(const SemanticObservedTrajectoryStructure& structure)
		: Graph::Tree<SemanticObservedTrajectoryNode, Graph::MapNodeIndicator, Graph::FlowEdge, double>(structure)
	{

	}

	SemanticObservedTrajectoryStructure::~SemanticObservedTrajectoryStructure()
	{
	}

	///<summary>
	/// �w�肵���l��root_node��������
	///</summary>
	void SemanticObservedTrajectoryStructure::initialize(std::shared_ptr<SemanticObservedTrajectoryNode> root_node)
	{
		if (root_node->get_depth() != 0) throw std::invalid_argument("Depth of root node must be 0");
		Graph::Tree<SemanticObservedTrajectoryNode, Graph::MapNodeIndicator, Graph::FlowEdge, double>::initialize(root_node);
	}


	///<summary>
	/// MapNodeIndicator��Phase����Ƀm�[�h��T�����C���������ꍇ�͂��̃m�[�h���w���C�e���[�^��Ԃ��D
	/// ������Ȃ��ꍇ��nullptr���w���C�e���[�^��Ԃ��D
	/// phase��-1���w�肵���ꍇ��root node��Ԃ�
	///</summary>
	SemanticObservedTrajectoryStructure::base_iterator SemanticObservedTrajectoryStructure::find_node(const Graph::MapNodeIndicator& id, int phase)
	{
		if (phase == -1) return root<SemanticObservedTrajectoryStructure::base_iterator>();

		int depth = phase + 1;
		Collection::IdentifiableCollection<Graph::node_id, SemanticObservedTrajectoryNode>::iterator iter = std::find_if(node_collection->begin(), node_collection->end(), [id, depth](std::shared_ptr<Identifiable<Graph::node_id>> element) {
			std::shared_ptr<SemanticObservedTrajectoryNode> node = std::dynamic_pointer_cast<SemanticObservedTrajectoryNode>(element);
			bool is_ret_node = node != nullptr && node->data != nullptr && *node->data == id && depth == node->get_depth();
			return is_ret_node;
		});
		return iter == node_collection->end() ? SemanticObservedTrajectoryStructure::base_iterator(-1, nullptr) : SemanticObservedTrajectoryStructure::base_iterator((*iter)->get_id(), node_collection);
	}
	
	///<summary>
	/// �}�b�v�̃m�[�hID�ƖK��t�F�[�Y����Ή�����TrajectoryNode��ID���擾����
	/// �Y������m�[�h���Ȃ��ꍇ-1��Ԃ�
	///</summary>
	Graph::node_id SemanticObservedTrajectoryStructure::find_node_id(const Graph::MapNodeIndicator& id, int phase) const
	{
		int depth = phase + 1;
		Collection::IdentifiableCollection<Graph::node_id, SemanticObservedTrajectoryNode>::iterator iter = std::find_if(node_collection->begin(), node_collection->end(), [id, depth](std::shared_ptr<Identifiable<Graph::node_id>> element) {
			std::shared_ptr<SemanticObservedTrajectoryNode> node = std::dynamic_pointer_cast<SemanticObservedTrajectoryNode>(element);
			bool is_ret_node = node != nullptr && node->data != nullptr && *node->data == id && depth == node->get_depth();
			return is_ret_node;
		});
		
		return iter == node_collection->end() ? -1 : (*iter)->get_id();
	}
}


