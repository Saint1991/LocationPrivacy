#ifdef SIMULATORCOMPONENTS_EXPORTS
#define PREFERENCE_TREE_API __declspec(dllexport)
#else
#define PREFERENCE_TREE_API __declspec(dllimport)
#endif

#pragma once
#include "PreferenceTreeNode.h"
#include "SequentialTree.h"
#include "Sequence.h"

namespace User
{

	///<summary>
	/// ユーザの嗜好を表す木
	///</summary>
	class PREFERENCE_TREE_API PreferenceTree : public Graph::SequentialTree<PreferenceTreeNode, std::string>
	{
	private:
		Collection::Sequence<category_id> make_prefix_by_node(std::shared_ptr<PreferenceTreeNode const> node) const;
		std::shared_ptr<PreferenceTreeNode> get_node_by_prefix(const Collection::Sequence<category_id>& prefix);
	public:
		PreferenceTree();
		PreferenceTree(const PreferenceTree& t);
		~PreferenceTree();

		void for_each_prefix(unsigned int sequence_length, const std::function<void(const Collection::Sequence<category_id>&, double)>& execute_function) const;
		void add_sequence_counter(const std::vector<category_id>& sequence, double add_num = 1.0);
		double get_support_of(const std::vector<category_id>& sequence) const;
		
		PREFERENCE_TREE_API friend double distance(const User::PreferenceTree& t1, const User::PreferenceTree& t2);
		PREFERENCE_TREE_API friend double similarity(const User::PreferenceTree& t1, const User::PreferenceTree& t2);
	};
}


