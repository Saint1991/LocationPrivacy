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
	protected:
		std::set<node_id> visited_node_ids;
		std::stack<node_id> stack;

	public:
		DepthFirstConstIterator(node_id id, std::shared_ptr<Collection::IdentifiableCollection<node_id, NODE>> node_collection);
		DepthFirstConstIterator<NODE, NODE_DATA, EDGE> operator++();
	};
}

#include "DepthFirstConstIterator.hpp"