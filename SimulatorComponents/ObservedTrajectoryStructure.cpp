#include "stdafx.h"
#include "ObservedTrajectoryStructure.h"

namespace Observer
{

	///<summary>
	/// �R���X�g���N�^
	///</summary>
	ObservedTrajectoryStructure::ObservedTrajectoryStructure() : Graph::Tree<ObservedTrajectoryNode, Graph::MapNodeIndicator, Graph::FlowEdge, double>()
	{
		initialize(std::make_shared<ObservedTrajectoryNode>(0, 0, nullptr));
	}

	///<summary>
	/// �R�s�[�R���X�g���N�^
	///</summary>
	ObservedTrajectoryStructure::ObservedTrajectoryStructure(const ObservedTrajectoryStructure& structure) 
		: Graph::Tree<ObservedTrajectoryNode, Graph::MapNodeIndicator, Graph::FlowEdge, double>(structure)
	{

	}

	///<summary>
	/// �f�X�g���N�^
	///</summary>
	ObservedTrajectoryStructure::~ObservedTrajectoryStructure()
	{
	}


	///<summary>
	/// �w�肵���l��root_node��������
	///</summary>
	void ObservedTrajectoryStructure::initialize(std::shared_ptr<ObservedTrajectoryNode> root_node)
	{
		if (root_node->get_depth() != 0) throw std::invalid_argument("Depth of root node must be 0");
		Graph::Tree<ObservedTrajectoryNode, Graph::MapNodeIndicator, Graph::FlowEdge, double>::initialize(root_node);
	}


	///<summary>
	/// MapNodeIndicator��Phase����Ƀm�[�h��T�����C���������ꍇ�͂��̃m�[�h���w���C�e���[�^��Ԃ��D
	/// ������Ȃ��ꍇ��nullptr���w���C�e���[�^��Ԃ��D
	/// phase��-1���w�肵���ꍇ��root node��Ԃ�
	///</summary>
	ObservedTrajectoryStructure::base_iterator ObservedTrajectoryStructure::find_node(const Graph::MapNodeIndicator& id, int phase)
	{
		if (phase == -1) return root<ObservedTrajectoryStructure::base_iterator>();

		int depth = phase + 1;
		Collection::IdentifiableCollection<Graph::node_id, Observer::ObservedTrajectoryNode>::iterator iter = std::find_if(node_collection->begin(), node_collection->end(), [id, depth](std::shared_ptr<Identifiable<Graph::node_id>> element) {
			std::shared_ptr<Observer::ObservedTrajectoryNode> node = std::dynamic_pointer_cast<Observer::ObservedTrajectoryNode>(element);
			bool is_ret_node = node != nullptr && node->data != nullptr && *node->data == id && depth == node->get_depth();
			return is_ret_node;
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
			bool is_ret_node = node != nullptr && node->data != nullptr && *node->data == id && depth == node->get_depth();
			return is_ret_node;
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
		ret->root_node->connect_to(std::make_shared<Graph::FlowEdge>(root_node_id));
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
	/// Phase�ɑΉ�����m�[�h��S�Ď擾����
	///</summary>
	std::vector<std::shared_ptr<ObservedTrajectoryNode const>> ObservedTrajectoryStructure::read_all_nodes_of_phase(int phase) const
	{
		int depth = phase + 1;
		std::vector<std::shared_ptr<ObservedTrajectoryNode const>> ret;
		node_collection->foreach([&ret, &depth](std::shared_ptr<ObservedTrajectoryNode const> node) {
			if (node->get_depth() == depth) ret.push_back(node);
		});
		return ret;
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
	void ObservedTrajectoryStructure::for_each_possible_trajectory(const std::function<void(const Collection::Sequence<Graph::MapNodeIndicator>&)>& execute_function) const
	{
		size_t length = trajectory_length();
		ObservedTrajectoryStructure::revisit_depth_first_const_iterator iter = const_root<ObservedTrajectoryStructure::revisit_depth_first_const_iterator>();
		iter++;
		
		Collection::Sequence<Graph::MapNodeIndicator> trajectory;
		while (*iter != nullptr) {
			int phase = iter->get_depth() - 1;
			if (phase == 0) trajectory.clear();
			else trajectory = trajectory.subsequence(0, phase - 1);
			trajectory.push_back(*iter->data);
			if (trajectory.size() == length) execute_function(trajectory);
			iter++;
		}
	}

	
	///<summary>
	/// ��肤��g���W�F�N�g����S�Ď擾����
	///</summary>
	std::vector<Collection::Sequence<Graph::MapNodeIndicator>> ObservedTrajectoryStructure::get_all_possible_trajectories() const
	{
		std::vector<Collection::Sequence<Graph::MapNodeIndicator>> trajectories;
		for_each_possible_trajectory([&trajectories](const Collection::Sequence<Graph::MapNodeIndicator>& trajectory) {
			trajectories.push_back(trajectory);
		});
		return trajectories;
	}


	///<summary>
	/// �w�肵��Phase�Ɋe�m�[�h�ɑ��݂���G���e�B�e�B�����擾����
	///</summary>
	std::unordered_map<Graph::MapNodeIndicator, size_t> ObservedTrajectoryStructure::get_entity_count_info_of_phase(int phase) const
	{
		std::unordered_map<Graph::MapNodeIndicator, size_t> entity_count_info;
		
		std::vector<std::shared_ptr<ObservedTrajectoryNode const>> first_nodes = read_all_nodes_of_phase(phase);
		for (std::vector<std::shared_ptr<ObservedTrajectoryNode const>>::const_iterator node = first_nodes.begin(); node != first_nodes.end(); node++) {
			entity_count_info.insert(std::make_pair(*(*node)->data, (*node)->get_count()));
		} 

		return entity_count_info;
	}

	///<summary>
	/// trajectory�ɊY������G���e�B�e�B�����̌o�H�ňړ������m�����Z�o����
	/// ����0�̃g���W�F�N�g���̏ꍇ��0.0�ɂȂ�̂Œ���
	///</summary>
	double ObservedTrajectoryStructure::calc_probability_of_trajectory(const std::vector<Graph::MapNodeIndicator>& trajectory) const
	{
		if (trajectory.size() == 0) return 0.0;
		double probability = 1.0;
		
		ObservedTrajectoryStructure::base_const_iterator iter = const_root<ObservedTrajectoryStructure::base_const_iterator>();

		for (int phase = 0; phase < trajectory.size(); phase++) {
			Graph::MapNodeIndicator node = trajectory.at(phase);
			iter = iter.find_child_if([&node](std::shared_ptr<ObservedTrajectoryNode const> child) {
				return *child->data == node;
			});
			if (*iter == nullptr) return 0.0;
			if (phase < trajectory.size() - 1) {
				Graph::MapNodeIndicator next_node = trajectory.at(phase + 1);
				Graph::node_id next_node_id = find_node_id(next_node, phase + 1);
				std::shared_ptr<Graph::FlowEdge const> edge = iter->get_static_edge_to(next_node_id);
				probability *= edge->get_flow();
			}
		}

		return probability;
	}


	///<summary>
	/// ����[�Ȃ��ꓖ����I�Ή�
	/// �o�Ă���G�b�W�̃t���[�̘a��1�ɐ��K������
	///</summary>
	void ObservedTrajectoryStructure::normalize_all_edges()
	{
		node_collection->foreach([](std::shared_ptr<ObservedTrajectoryNode> node) {
			double edge_flow_sum = 0.0;
			node->for_each_edge([&edge_flow_sum](std::shared_ptr<Graph::FlowEdge> edge) {
				edge_flow_sum += edge->get_flow();
			});
			if (!(0.999 <= edge_flow_sum && edge_flow_sum <= 1.001)) {
				double new_value = 1.0 / (double)node->get_connecting_node_list().size();
				node->for_each_edge([new_value](std::shared_ptr<Graph::FlowEdge> edge) {
					edge->set_flow(new_value);
				});
			}
		});
	}
}

