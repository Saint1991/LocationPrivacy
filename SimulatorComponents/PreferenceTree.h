#ifdef SIMULATORCOMPONENTS_EXPORTS
#define PREFERENCE_TREE_API __declspec(dllexport)
#else
#define PREFERENCE_TREE_API __declspec(dllimport)
#endif

#pragma once
#include "ProbabilityEdge.h"
#include "PreferenceTreeNode.h"
#include "PrefixTree.h"

namespace User
{

	///<summary>
	/// ユーザの嗜好を表す木
	///</summary>
	class PreferenceTree : public Graph::PrefixTree<PreferenceTreeNode, std::string, ProbabilityEdge>
	{
	public:
		PreferenceTree();
		~PreferenceTree();
	};
}


