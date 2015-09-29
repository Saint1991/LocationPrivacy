#include "stdafx.h"
#include "PreferenceTree.h"

namespace User
{


	///<summary>
	/// コンストラクタ
	///</summary>
	PreferenceTree::PreferenceTree() : Graph::PrefixTree<PreferenceTreeNode, User::category_id, ProbabilityEdge>()
	{
		initialize();
	}


	///<summary>
	/// デストラクタ
	///</summary>
	PreferenceTree::~PreferenceTree()
	{
	}


	///<summary>
	/// headからtailまで実行されるので注意(よって最後まで渡したい場合はtailをend() - 1にする)
	///</summary>
	void PreferenceTree::increase_prefix_count(std::vector<User::category_id>::iterator head, std::vector<User::category_id>::iterator tail)
	{

		if (head >= tail) return;
		
		PreferenceTree::base_iterator current_node = root<PreferenceTree::base_iterator>();
		current_node->count_up();
		
		std::vector<User::category_id>::const_iterator iter = head;
		while (true) {
			PreferenceTree::base_iterator previous_node = current_node;
			current_node = current_node.find_child_if([iter](std::shared_ptr<User::PreferenceTreeNode const> node) {
				return *(node->data) == *iter;
			});

			//該当ノードがない場合は新しいノードを挿入しエッジを作成
			if (current_node == end<PreferenceTree::base_iterator>()) {
				current_node = insert(previous_node, ProbabilityEdge(node_collection->size(), 0.0), *iter);
			}
			current_node->count_up();

			//エッジが持つ遷移確率を更新
			previous_node->for_each_edge([&](std::shared_ptr<User::ProbabilityEdge> edge) {
				int parent_count = previous_node->visit_count();
				std::shared_ptr<PreferenceTreeNode const> target_node = node_collection->read_by_id(edge->get_to());
				int target_count = target_node->visit_count();
				edge->set_probability(parent_count / (double)target_count);
			});

			if (iter == tail) return;
			iter++;
		}
	}

	///<summary>
	/// prefix1つに含まれるノードのcounterをインクリメントする 
	///</summary>
	void PreferenceTree::increase_prefix_count(std::vector<User::category_id> prefix)
	{
		if (prefix.size() == 0) return;
		increase_prefix_count(prefix.begin(), prefix.end() - 1);
	}


	///<summary>
	/// 指定したシークエンスに該当するノードを取得する
	/// 合致するノードが存在しない場合はnullptrを返す
	///</summary>
	PreferenceTree::base_iterator PreferenceTree::get_iter_by_sequence(std::vector<User::category_id> category_sequence)
	{
		PreferenceTree::base_iterator current_node = root<PreferenceTree::base_iterator>();
		for (std::vector<User::category_id>::const_iterator iter = category_sequence.begin(); iter != category_sequence.end(); iter++) {
			current_node = current_node.find_child_if([iter](std::shared_ptr<User::PreferenceTreeNode const> node) {
				return *(node->data) == *iter;
			});
			if (current_node == end<PreferenceTree::base_iterator>()) return end<PreferenceTree::base_iterator>();
		}
		return current_node;
	}



	///<summary>
	/// category_sequenceを訪問したとしてPreferenceTreeを更新する
	///</summary>
	void PreferenceTree::update_tree(std::vector<User::category_id> category_sequence)
	{
		Collection::Sequence<User::category_id> sequence(category_sequence);
		sequence.for_each_prefix([&](Collection::Sequence<User::category_id>::iterator head, Collection::Sequence<User::category_id>::iterator tail) {
			increase_prefix_count(head, tail);
		});
	}


	///<summary>
	/// 2つの木のEditDistanceを計算する (要実装)
	///</summary>
	double User::edit_distance(const User::PreferenceTree& t1, const User::PreferenceTree& t2) 
	{
		return 0.0;
	}
}
