#pragma once
#include "stdafx.h"
#include "BaseIterator.h"

namespace Graph
{
	template <typename NODE, typename NODE_DATA, typename EDGE, typename EDGE_DATA>
	class RevisitDepthFirstIterator : public BaseIterator<NODE, NODE_DATA, EDGE, EDGE_DATA>
	{
	protected:
		std::stack<node_id> stack;

	public:
		RevisitDepthFirstIterator(node_id id, std::shared_ptr<Collection::IdentifiableCollection<node_id, NODE>> node_collection);
		~RevisitDepthFirstIterator();
		RevisitDepthFirstIterator<NODE, NODE_DATA, EDGE, EDGE_DATA>& operator++(int);
	};
}

#include "RevisitDepthFirstIterator.hpp"
