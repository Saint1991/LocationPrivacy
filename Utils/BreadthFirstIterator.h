#pragma once
#include "stdafx.h"
#include "PrefixTree.h"
#include "BaseIterator.h"

namespace Graph
{

	///<summary>
	/// 幅優先探索用イテレータ
	///</summary>
	template <typename NODE, typename NODE_DATA, typename EDGE>
	class BreadthFirstIterator : public BaseIterator<NODE, NODE_DATA, EDGE>
	{
	//friend class PrefixTree<NODE, NODE_DATA, EDGE>;
	protected:
		BreadthFirstIterator();
		BreadthFirstIterator(std::shared_ptr<NODE> node);
	
		std::set<node_id> visited_node_ids;
		std::queue<node_id> queue;

	public:
		BreadthFirstIterator<NODE, NODE_DATA, EDGE> operator++();
	};
}

#include "BreadthFirstIterator.hpp"
