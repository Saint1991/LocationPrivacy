#pragma once
#include "stdafx.h"
#include "PrefixTree.h"
#include "BaseIterator.h"

namespace Graph
{

	///<summary>
	/// 深さ優先探索用イテレータ
	///</summary>
	template <typename NODE, typename NODE_DATA, typename EDGE>
	class DepthFirstIterator : public BaseIterator<NODE, NODE_DATA, EDGE>
	{
	//friend class PrefixTree<NODE, NODE_DATA, EDGE>;

	protected:
		DepthFirstIterator();
		DepthFirstIterator(std::shared_ptr<NODE> node);

		std::set<node_id> visited_node_ids;
		std::stack<node_id> stack;
	
	public:
		DepthFirstIterator<NODE, NODE_DATA, EDGE> operator++();
	};
}

#include "DepthFirstIterator.hpp"
