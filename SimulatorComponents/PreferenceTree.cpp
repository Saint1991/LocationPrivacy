#include "stdafx.h"
#include "PreferenceTree.h"

namespace User
{


	///<summary>
	/// コンストラクタ
	///</summary>
	PreferenceTree::PreferenceTree() : Graph::PrefixTree<PreferenceTreeNode, User::category_id, Graph::BasicEdge>()
	{
		initialize();
	}


	///<summary>
	///  コピーコンストラクタ
	///</summary>
	PreferenceTree::PreferenceTree(const PreferenceTree& t) : Graph::PrefixTree<PreferenceTreeNode, User::category_id, Graph::BasicEdge>(t)
	{
		
	}

	///<summary>
	/// デストラクタ
	///</summary>
	PreferenceTree::~PreferenceTree()
	{
	}


	///<summary>
	/// nodeを末端にもつプレフィックスを取得する
	///</summary>
	Collection::Sequence<category_id> PreferenceTree::make_prefix_by_node(std::shared_ptr<PreferenceTreeNode const> node) const
	{
		
		Graph::node_id current_node_id = node->get_id();
		std::shared_ptr<PreferenceTreeNode const> current_node = node;
		
		Collection::Sequence<category_id> sequence;
		while (current_node != nullptr && current_node_id != 0 && current_node_id != -1) {
			sequence.push_back(current_node->category_id());
			current_node_id = current_node->get_parent();
			current_node = node_collection->read_by_id(current_node_id);
		}

		std::reverse(sequence.begin(), sequence.end());
		return sequence;
	}


	///<summary>
	/// Prefixをもとに対応するノードを取得する
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
	/// depthの階層にあるノードのIDを全て取得する
	///</summary>
	std::vector<Graph::node_id> PreferenceTree::get_all_nodes_by_depth(int depth) const
	{
		std::vector<Graph::node_id> ret;
		node_collection->foreach([depth, &ret](std::shared_ptr<PreferenceTreeNode const> node) {
			if (node->get_depth() == depth) ret.push_back(node->get_id());
		});
		return ret;
	}


	///<summary>
	/// 木に含まれるプレフィックスの最大長を取得する
	///</summary>
	int PreferenceTree::max_depth() const
	{
		int max = 0;
		node_collection->foreach([&max](std::shared_ptr<PreferenceTreeNode const> node) {
			int depth = node->get_depth();
			if (max < depth) max = depth;
		});
		return max;
	}

	///<summary>
	/// 木の中に含まれる長さsequence_lengthの各プレフィックスについてexecute_functionを実行する．
	///</summary>
	void PreferenceTree::for_each_prefix(unsigned int sequence_length, const std::function<void(const Collection::Sequence<category_id>&, double)>& execute_function) const
	{
		double trajectory_count = root_node->count();

		std::vector<Graph::node_id> node_ids = get_all_nodes_by_depth(sequence_length);
		for (std::vector<Graph::node_id>::const_iterator target = node_ids.begin(); target != node_ids.end(); target++) {
			std::shared_ptr<PreferenceTreeNode const> node = node_collection->read_by_id(*target);
			Collection::Sequence<category_id> sequence = make_prefix_by_node(node);
			double visit_count = node->count();
			double support = visit_count / trajectory_count;
			execute_function(sequence, support);
		}
	}


	///<summary>
	/// sequenceに該当するノードのカウンターをadd_num分増加させる
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
	/// 指定されたシーケンスのサポート値を取得する
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
			if (current_node == const_end<base_const_iterator>()) break;
		}

		support = current_node == const_end<base_const_iterator>() ? 0.0 : current_node->count() / trajectory_count;
		return support;
	}


	///<summary>
	/// 2つの木の距離を求める
	/// 同プレフィックス同士の誤差のAverageを返す
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
	/// 2つの木の嗜好の保存度を計算する．
	///</summary>
	double similarity(const User::PreferenceTree& t1, const User::PreferenceTree& t2)
	{
		return 1.0 - distance(t1, t2);
	}
}
