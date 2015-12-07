#include "stdafx.h"
#include "PreferenceTree.h"

namespace User
{


	///<summary>
	/// �R���X�g���N�^
	///</summary>
	PreferenceTree::PreferenceTree() : Graph::SequentialTree<PreferenceTreeNode, User::category_id>()
	{

	}


	///<summary>
	///  �R�s�[�R���X�g���N�^
	///</summary>
	PreferenceTree::PreferenceTree(const PreferenceTree& t) : Graph::SequentialTree<PreferenceTreeNode, User::category_id>(t)
	{
		
	}

	///<summary>
	/// �f�X�g���N�^
	///</summary>
	PreferenceTree::~PreferenceTree()
	{
	}


	///<summary>
	/// node�𖖒[�ɂ��v���t�B�b�N�X���擾����
	///</summary>
	Collection::Sequence<category_id> PreferenceTree::make_prefix_by_node(std::shared_ptr<PreferenceTreeNode const> node) const
	{
		Collection::Sequence<std::shared_ptr<PreferenceTreeNode const>> sequence = get_sequence_by_node(node);
		Collection::Sequence<category_id> ret(sequence.size());
		int index = 0;
		for (Collection::Sequence<std::shared_ptr<PreferenceTreeNode const>>::const_iterator iter = sequence.begin(); iter != sequence.end(); iter++, index++) {
			ret.at(index) = (*iter)->category_id();
		}
		return ret;
	}


	///<summary>
	/// Prefix�����ƂɑΉ�����m�[�h���擾����
	///</summary>
	std::shared_ptr<PreferenceTreeNode> PreferenceTree::get_node_by_prefix(const Collection::Sequence<category_id>& prefix)
	{
		base_iterator current_node = root<base_iterator>();
		int current_depth = 0;

		for (std::vector<category_id>::const_iterator iter = prefix.begin(); iter != prefix.end(); iter++) {
			base_iterator next_node = current_node.find_child_if([iter](std::shared_ptr<PreferenceTreeNode const> node) {
				return node->category_id() == *iter;
			});
			current_depth++;

			if (*next_node == nullptr) {
				Graph::node_id new_id = node_collection->size();
				current_node->connect_to(std::make_shared<Graph::BasicEdge>(new_id));
				node_collection->add(std::make_shared<PreferenceTreeNode>(new_id, current_node->get_id(), current_depth, *iter));
				next_node = base_iterator(new_id, node_collection);
			}
			current_node = next_node;
		}
		return *current_node;
	}

	///<summary>
	/// �؂̒��Ɋ܂܂�钷��sequence_length�̊e�v���t�B�b�N�X�ɂ���execute_function�����s����D
	///</summary>
	void PreferenceTree::for_each_prefix(unsigned int sequence_length, const std::function<void(const Collection::Sequence<category_id>&, double)>& execute_function) const
	{
		double trajectory_count = root_node->count();

		std::vector<std::shared_ptr<PreferenceTreeNode const>> nodes = get_all_nodes_in_depth(sequence_length);
		for (std::vector<std::shared_ptr<PreferenceTreeNode const>>::const_iterator target = nodes.begin(); target != nodes.end(); target++) {
			Collection::Sequence<category_id> sequence = make_prefix_by_node(*target);
			double visit_count = (*target)->count();
			double support = visit_count / trajectory_count;
			execute_function(sequence, support);
		}
	}


	///<summary>
	/// sequence�ɊY������m�[�h�̃J�E���^�[��add_num������������
	///</summary>
	void PreferenceTree::add_sequence_counter(const std::vector<category_id>& sequence, double add_num)
	{

		base_iterator current_node = root<base_iterator>();
		current_node->count_up(add_num);
		int current_depth = 0;

		for (std::vector<category_id>::const_iterator iter = sequence.begin(); iter != sequence.end(); iter++) {
			
			base_iterator next_node = current_node.find_child_if([iter](std::shared_ptr<PreferenceTreeNode const> node) {
				User::category_id category = node->category_id();
				return node->category_id() == *iter;
			});
			current_depth++;

			if (*next_node == nullptr) {
				Graph::node_id new_id = node_collection->size();
				current_node->connect_to(std::make_shared<Graph::BasicEdge>(new_id));
				std::shared_ptr<PreferenceTreeNode> new_node = std::make_shared<PreferenceTreeNode>(new_id, current_node->get_id(), current_depth, *iter);
				node_collection->add(new_node);
				next_node = base_iterator(new_id, node_collection);
			}
			current_node = next_node;
			current_node->count_up(add_num);
		}
	}


	///<summary>
	/// �w�肳�ꂽ�V�[�P���X�̃T�|�[�g�l���擾����
	///</summary>
	double PreferenceTree::get_support_of(const std::vector<category_id>& sequence) const
	{
		double support = 0.0;
		base_const_iterator current_node = const_root<base_const_iterator>();
		double trajectory_count = current_node->count();

		for (std::vector<category_id>::const_iterator iter = sequence.begin(); iter != sequence.end(); iter++) {
			current_node = current_node.find_child_if([iter](std::shared_ptr<PreferenceTreeNode const> node) {
				return node->category_id() == *iter;
			});
			if (*current_node == nullptr) break;
		}

		support = *current_node == nullptr ? 0.0 : current_node->count() / trajectory_count;
		return support;
	}


	///<summary>
	/// 2�̖؂̋��������߂�
	/// ���v���t�B�b�N�X���m�̌덷��Average��Ԃ�
	///</summary>
	double distance(const User::PreferenceTree& t1, const User::PreferenceTree& t2)
	{
		PreferenceTree t = t1;
		double trajectory_count = t.root_node->count();
		int max_depth = t2.max_depth();
		for (int depth = 1; depth <= max_depth; depth++) {
			t2.for_each_prefix(depth, [&t, trajectory_count](const Collection::Sequence<category_id>& prefix, double support) {
				std::shared_ptr<PreferenceTreeNode> target_node = t.get_node_by_prefix(prefix);
				target_node->count_up(-support * trajectory_count);
			});
		}

		int prefix_count = 0;
		double total_error = 0.0;
		max_depth = t.max_depth();
		for (int depth = 1; depth <= max_depth; depth++) {
			t.for_each_prefix(depth, [&prefix_count, &total_error](const Collection::Sequence<category_id>& prefix, double support) {
				prefix_count++;
				total_error += std::abs(support);
			});
		}
		return total_error / prefix_count;
	}


	///<summary>
	/// 2�̖؂̚n�D�̕ۑ��x���v�Z����D
	///</summary>
	double similarity(const User::PreferenceTree& t1, const User::PreferenceTree& t2)
	{
		return 1.0 - distance(t1, t2);
	}
}
