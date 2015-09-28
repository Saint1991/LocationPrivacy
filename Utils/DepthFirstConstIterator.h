#pragma once
#include "stdafx.h"
#include "PrefixTree.h"
#include "BaseConstIterator.h"

namespace Graph
{

	///<summary>
	/// 深さ優先探索用イテレータ
	///</summary>
	template <typename NODE, typename NODE_DATA, typename EDGE>
	class DepthFirstConstIterator : public BaseConstIterator<NODE, NODE_DATA, EDGE>
	{
	friend class PrefixTree<NODE, NODE_DATA, EDGE>;
	protected:
		DepthFirstConstIterator();
		DepthFirstConstIterator(std::shared_ptr<NODE const> node);

		std::set<node_id> visited_node_ids;
		std::stack<node_id> stack;

	public:
		DepthFirstConstIterator<NODE, NODE_DATA, EDGE> operator++();
	};
}

#include "DepthFirstConstIterator.hpp"