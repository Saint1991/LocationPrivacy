#pragma once
#include "stdafx.h"
#include "Tree.h"
#include "BaseIterator.h"

namespace Graph
{

	///<summary>
	/// �[���D��T���p�C�e���[�^
	///</summary>
	template <typename NODE, typename NODE_DATA, typename EDGE, typename EDGE_DATA>
	class DepthFirstIterator : public BaseIterator<NODE, NODE_DATA, EDGE, EDGE_DATA>
	{
	protected:
		std::set<node_id> visited_node_ids;
		std::stack<node_id> stack;
	
	public:
		DepthFirstIterator(node_id id, std::shared_ptr<Collection::IdentifiableCollection<node_id, NODE>> node_collection);
		DepthFirstIterator<NODE, NODE_DATA, EDGE, EDGE_DATA>& operator++(int);
	};
}

#include "DepthFirstIterator.hpp"
