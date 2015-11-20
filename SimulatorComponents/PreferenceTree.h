#ifdef SIMULATORCOMPONENTS_EXPORTS
#define PREFERENCE_TREE_API __declspec(dllexport)
#else
#define PREFERENCE_TREE_API __declspec(dllimport)
#endif

#pragma once
#include "PreferenceTreeNode.h"
#include "PrefixTree.h"
#include "Sequence.h"

namespace User
{

	///<summary>
	/// ユーザの嗜好を表す木
	///</summary>
	class PreferenceTree : public Graph::PrefixTree<PreferenceTreeNode, std::string, Graph::BasicEdge>
	{
	protected:
		std::vector<Graph::node_id> get_all_nodes_by_depth(int depth);
	public:
		PreferenceTree();
		~PreferenceTree();

		void for_each_prefix(unsigned int sequence_length, const std::function<void(const Collection::Sequence<category_id>&, double)>& execute_function);
		void add_sequence_counter(const std::vector<category_id>& sequence, double add_num = 1.0);
		double get_support_of(const std::vector<category_id>& sequence) const;
		friend double distance(const User::PreferenceTree& t1, const User::PreferenceTree& t2);
		friend double similarity(const User::PreferenceTree& t1, const User::PreferenceTree& t2);
	};
}


