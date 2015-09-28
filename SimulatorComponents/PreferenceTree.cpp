#include "stdafx.h"
#include "PreferenceTree.h"

namespace User
{

	PreferenceTree::PreferenceTree()
	{
	}


	PreferenceTree::~PreferenceTree()
	{
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

}
