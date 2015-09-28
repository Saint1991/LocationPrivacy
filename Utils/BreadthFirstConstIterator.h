#pragma once
#include "stdafx.h"
#include "PrefixTree.h"
#include "BaseConstIterator.h"

namespace Graph
{

	///<summary>
	/// 幅優先探索用イテレータ
	///</summary>
	template <typename NODE, typename NODE_DATA, typename EDGE>
	class BreadthFirstConstIterator : public BaseConstIterator<NODE, NODE_DATA, EDGE> 
	{
	friend class PrefixTree<NODE, NODE_DATA, EDGE>;
	private:
		BreadthFirstConstIterator();
		BreadthFirstConstIterator(std::shared_ptr<NODE const> node);

		std::set<node_id> visited_node_ids;
		std::queue<node_id> queue;

	public:
		BreadthFirstConstIterator<NODE, NODE_DATA, EDGE> operator++();
	};
}

#include "BreadthFirstConstIterator.hpp"