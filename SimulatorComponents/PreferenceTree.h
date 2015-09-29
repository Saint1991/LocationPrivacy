#ifdef SIMULATORCOMPONENTS_EXPORTS
#define PREFERENCE_TREE_API __declspec(dllexport)
#else
#define PREFERENCE_TREE_API __declspec(dllimport)
#endif

#pragma once
#include "ProbabilityEdge.h"
#include "PreferenceTreeNode.h"
#include "PrefixTree.h"
#include "Sequence.h"

namespace User
{

	///<summary>
	/// ユーザの嗜好を表す木
	///</summary>
	class PreferenceTree : public Graph::PrefixTree<PreferenceTreeNode, std::string, ProbabilityEdge>
	{
	protected:
		void increase_prefix_count(std::vector<User::category_id>::iterator head, std::vector<User::category_id>::iterator tail);
		void increase_prefix_count(std::vector<User::category_id> prefix);

	public:
		PreferenceTree();
		~PreferenceTree();

		base_iterator get_iter_by_sequence(std::vector<category_id> category_sequence);
		void update_tree(std::vector<category_id> category_sequence);

		friend double edit_distance(const User::PreferenceTree& t1, const User::PreferenceTree& t2);
	};
}


