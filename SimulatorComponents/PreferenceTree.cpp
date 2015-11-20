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
	/// デストラクタ
	///</summary>
	PreferenceTree::~PreferenceTree()
	{
	}


	///<summary>
	/// depthの階層にあるノードのIDを全て取得する
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
	/// 木の中に含まれる長さsequence_lengthの各プレフィックスについてexecute_functionを実行する．
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
	/// sequenceに該当するノードのカウンターをadd_num分増加させる
	///</summary>
	void PreferenceTree::add_sequence_counter(const std::vector<category_id>& sequence, double add_num)
	{
		base_iterator iter = root<base_iterator>();
		
	}


	///<summary>
	/// 指定されたシーケンスのサポート値を取得する
	///</summary>
	double PreferenceTree::get_support_of(const std::vector<category_id>& sequence) const
	{
		return 0.0;
	}

	///<summary>
	/// 2つの木の距離を求める
	/// 同プレフィックス同士の誤差の合計値を返す
	///</summary>
	double distance(const User::PreferenceTree& t1, const User::PreferenceTree& t2)
	{
		return 0.0;
	}


	///<summary>
	/// 2つの木の嗜好の保存度を計算する．
	///</summary>
	double similarity(const User::PreferenceTree& t1, const User::PreferenceTree& t2)
	{
		return 0.0;
	}
}
