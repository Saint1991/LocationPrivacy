#include "stdafx.h"
#include "PreferenceTree.h"

namespace User
{


	///<summary>
	/// �R���X�g���N�^
	///</summary>
	PreferenceTree::PreferenceTree() : Graph::PrefixTree<PreferenceTreeNode, User::category_id, Graph::BasicEdge>()
	{
		initialize();
	}


	///<summary>
	/// �f�X�g���N�^
	///</summary>
	PreferenceTree::~PreferenceTree()
	{
	}


	///<summary>
	/// depth�̊K�w�ɂ���m�[�h��ID��S�Ď擾����
	///</summary>
	std::vector<Graph::node_id> PreferenceTree::get_all_nodes_by_depth(int depth)
	{
		unsigned int current_depth = 0;
		std::queue<std::shared_ptr<PreferenceTreeNode>> queue;
		base_iterator iter = root<base_iterator>();
		std::set<Graph::node_id> visited_node_ids;

		queue.push(*iter);


		return std::vector<Graph::node_id>();
	}


	///<summary>
	/// �؂̒��Ɋ܂܂�钷��sequence_length�̊e�v���t�B�b�N�X�ɂ���execute_function�����s����D
	///</summary>
	void PreferenceTree::for_each_prefix(unsigned int sequence_length, const std::function<void(const Collection::Sequence<category_id>&, double)>& execute_function)
	{
		int depth = 0;
		base_iterator iter = root<base_iterator>();
		std::queue<Graph::node_id> queue;
		while (iter != end<base_iterator>() && depth <= sequence_length) {
			Collection::Sequence<category_id> category_sequence;
			std::vector<Graph::node_id> children = iter->get_children();
			Graph::node_id last_child = children.back();
		}
	}


	///<summary>
	/// sequence�ɊY������m�[�h�̃J�E���^�[��add_num������������
	///</summary>
	void PreferenceTree::add_sequence_counter(const std::vector<category_id>& sequence, double add_num)
	{
		base_iterator iter = root<base_iterator>();
		
	}


	///<summary>
	/// �w�肳�ꂽ�V�[�P���X�̃T�|�[�g�l���擾����
	///</summary>
	double PreferenceTree::get_support_of(const std::vector<category_id>& sequence) const
	{
		return 0.0;
	}

	///<summary>
	/// 2�̖؂̋��������߂�
	/// ���v���t�B�b�N�X���m�̌덷�̍��v�l��Ԃ�
	///</summary>
	double distance(const User::PreferenceTree& t1, const User::PreferenceTree& t2)
	{
		return 0.0;
	}


	///<summary>
	/// 2�̖؂̚n�D�̕ۑ��x���v�Z����D
	///</summary>
	double similarity(const User::PreferenceTree& t1, const User::PreferenceTree& t2)
	{
		return 0.0;
	}
}
