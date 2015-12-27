#include "stdafx.h"
#include "ObservedTrajectoryStructure.h"

namespace Observer
{

	///<summary>
	/// �R���X�g���N�^
	///</summary>
	ObservedTrajectoryStructure::ObservedTrajectoryStructure() : Graph::Tree<ObservedTrajectoryNode, Graph::MapNodeIndicator, Graph::BasicEdge>()
	{
		initialize(std::make_shared<ObservedTrajectoryNode>(0, 0, nullptr));
	}


	///<summary>
	/// �f�X�g���N�^
	///</summary>
	ObservedTrajectoryStructure::~ObservedTrajectoryStructure()
	{
	}


	void ObservedTrajectoryStructure::depth_first_trajectory_search(
		size_t trajectory_length,
		Collection::Sequence<Graph::MapNodeIndicator> sequence,
		ObservedTrajectoryStructure::depth_first_const_iterator& iter,
		const std::function<void(const Collection::Sequence<Graph::MapNodeIndicator>& trajectory)>& execute_function) const
	{
		if (sequence.size() == trajectory_length) {
			execute_function(sequence);
			return;
		}

		sequence.push_back(*iter->data);
		iter++;
		depth_first_trajectory_search(trajectory_length, sequence, iter, execute_function);
	}

	///<summary>
	/// �w�肵���l��root_node���擾����
	///</summary>
	void ObservedTrajectoryStructure::initialize(std::shared_ptr<ObservedTrajectoryNode> root_node)
	{
		if (root_node->get_depth() != 0) throw std::invalid_argument("Depth of root node must be 0");
		Graph::Tree<ObservedTrajectoryNode, Graph::MapNodeIndicator, Graph::BasicEdge>::initialize(root_node);
	}


	///<summary>
	/// MapNodeIndicator��Phase����Ƀm�[�h��T�����C���������ꍇ�͂��̃m�[�h���w���C�e���[�^��Ԃ��D
	/// ������Ȃ��ꍇ��nullptr���w���C�e���[�^��Ԃ��D
	///</summary>
	ObservedTrajectoryStructure::base_iterator ObservedTrajectoryStructure::find_node(const Graph::MapNodeIndicator& id, int phase)
	{
		int depth = phase + 1;
		Collection::IdentifiableCollection<Graph::node_id, Observer::ObservedTrajectoryNode>::iterator iter = std::find_if(node_collection->begin(), node_collection->end(), [id, depth](std::shared_ptr<Identifiable<Graph::node_id>> element) {
			std::shared_ptr<Observer::ObservedTrajectoryNode> node = std::dynamic_pointer_cast<Observer::ObservedTrajectoryNode>(element);
			return node != nullptr && *node->data == id && depth == node->get_depth();
		});

		return iter == node_collection->end() ? ObservedTrajectoryStructure::base_iterator(-1, nullptr) : ObservedTrajectoryStructure::base_iterator((*iter)->get_id(), node_collection);
	}


	///<summary>
	/// �}�b�v�̃m�[�hID�ƖK��t�F�[�Y����Ή�����TrajectoryNode��ID���擾����
	/// �Y������m�[�h���Ȃ��ꍇ-1��Ԃ�
	///</summary>
	Graph::node_id ObservedTrajectoryStructure::find_node_id(const Graph::MapNodeIndicator& id, int phase) const
	{
		int depth = phase + 1;
		Collection::IdentifiableCollection<Graph::node_id, Observer::ObservedTrajectoryNode>::iterator iter = std::find_if(node_collection->begin(), node_collection->end(), [id, depth](std::shared_ptr<Identifiable<Graph::node_id>> element) {
			std::shared_ptr<Observer::ObservedTrajectoryNode> node = std::dynamic_pointer_cast<Observer::ObservedTrajectoryNode>(element);
			return node != nullptr && node->data != nullptr && *node->data == id && depth == node->get_depth();
		});

		return iter == node_collection->end() ? -1 : (*iter)->get_id();
	}


	///<summary>
	/// �g���W�F�N�g�������擾����
	/// �m�[�h�����݂��Ȃ��ꍇ��-1��Ԃ�
	///</summary>
	size_t ObservedTrajectoryStructure::trajectory_length() const
	{
		Collection::IdentifiableCollection<Graph::node_id, ObservedTrajectoryNode>::const_iterator iter = std::max_element(node_collection->begin(), node_collection->end(), [](std::shared_ptr<Identifiable<Graph::node_id>> largest, std::shared_ptr<Identifiable<Graph::node_id>> first) {
			std::shared_ptr<ObservedTrajectoryNode> largest_node = std::dynamic_pointer_cast<ObservedTrajectoryNode>(largest);
			std::shared_ptr<ObservedTrajectoryNode> target_node = std::dynamic_pointer_cast<ObservedTrajectoryNode>(first);
			return largest_node->get_depth() < target_node->get_depth();
		});

		std::shared_ptr<ObservedTrajectoryNode> node = std::dynamic_pointer_cast<ObservedTrajectoryNode>(*iter);
		return node == nullptr ? -1 : node->get_depth();
	}


	///<summary>
	/// �w�肵��ID�����m�[�h�����[�g�Ƃ��镔���\���̃R�s�[���擾����
	/// �w�肵��ID�̃m�[�h�����݂��Ȃ��ꍇnullptr��Ԃ�
	///</summary>
	std::shared_ptr<ObservedTrajectoryStructure> ObservedTrajectoryStructure::sub_structure(Graph::node_id root_node_id) const
	{
		std::shared_ptr<ObservedTrajectoryStructure> ret = std::make_shared<ObservedTrajectoryStructure>();

		//�[���D��T����Structure��T�����C�R�s�[���ׂ��m�[�h��ID���X�g���쐬
		ret->root_node->connect_to(std::make_shared<Graph::BasicEdge>(root_node_id));
		ObservedTrajectoryStructure::depth_first_const_iterator iter = ObservedTrajectoryStructure::depth_first_const_iterator(root_node_id, node_collection);
		if (*iter == nullptr) return nullptr;
		int depth_offset = iter->get_depth() - 1;
		while (*iter != nullptr) {
			Graph::node_id new_node_id = ret->node_collection->size();
			std::shared_ptr<ObservedTrajectoryNode> node = std::make_shared<ObservedTrajectoryNode>(**iter);
			node->set_depth(iter->get_depth() - depth_offset);
			ret->node_collection->add(node);
			iter++;
		}
		return ret;
	}


	///<summary>
	/// �w�肵��Phase�Ɏw�肵��MapNodeIndicator�ɖK�₷��m�[�h�����[�g�Ƃ��镔���\���̃R�s�[���擾����
	/// �w�肵��ID�̃m�[�h�����݂��Ȃ��ꍇnullptr��Ԃ�
	///</summary>
	std::shared_ptr<ObservedTrajectoryStructure> ObservedTrajectoryStructure::sub_structure(const Graph::MapNodeIndicator& map_node_id, int phase) const
	{
		Graph::node_id id = find_node_id(map_node_id, phase);
		return id == -1 ? nullptr : sub_structure(id);
	}


	///<summary>
	/// �[���D��T���̏����ʂ�Ƀm�[�h���擾���C���ꂼ��̃m�[�h�ɂ���execute_function�����s����
	///</summary>
	void ObservedTrajectoryStructure::depth_first_iteration(Graph::node_id start_node_id, const std::function<void(std::shared_ptr<ObservedTrajectoryNode>)>& execute_function)
	{
		ObservedTrajectoryStructure::depth_first_iterator iter = get_iter_by_id<ObservedTrajectoryStructure::depth_first_iterator>(start_node_id);
		while (*iter != nullptr) {
			execute_function(*iter);
			iter++;
		}
	}


	///<summary>
	/// ���D��T���̏����ʂ�Ƀm�[�h���擾���C���ꂼ��̃m�[�h�ɂ���execute_function�����s����
	///</summary>
	void ObservedTrajectoryStructure::breadth_first_iteration(Graph::node_id start_node_id, const std::function<void(std::shared_ptr<ObservedTrajectoryNode>)>& execute_function)
	{
		ObservedTrajectoryStructure::breadth_first_iterator iter = get_iter_by_id<ObservedTrajectoryStructure::breadth_first_iterator>(start_node_id);
		while (*iter != nullptr) {
			execute_function(*iter);
			iter++;
		}
	}



	///<summary>
	/// ������t�܂ł̑S�Ẵp�X�ɂ���execute_function�����s����
	///</summary>
	void ObservedTrajectoryStructure::for_each_possible_trajectory(const std::function<void(const Collection::Sequence<Graph::MapNodeIndicator>& trajectory)>& execute_function) const
	{
		size_t depth = trajectory_length();
		ObservedTrajectoryStructure::depth_first_const_iterator iter = const_root<ObservedTrajectoryStructure::depth_first_const_iterator>();
		depth_first_trajectory_search(depth, Collection::Sequence<Graph::MapNodeIndicator>(), iter, execute_function);
	}
}

