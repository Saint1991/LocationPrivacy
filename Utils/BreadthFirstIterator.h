#pragma once
#include "stdafx.h"
#include "PrefixTree.h"
#include "BaseIterator.h"

namespace Graph
{

	///<summary>
	/// ���D��T���p�C�e���[�^
	///</summary>
	template <typename NODE, typename NODE_DATA, typename EDGE>
	class BreadthFirstIterator : public BaseIterator<NODE, NODE_DATA, EDGE>
	{
	protected:
		std::set<node_id> visited_node_ids;
		std::queue<node_id> queue;

	public:
		BreadthFirstIterator(node_id id, std::shared_ptr<Collection::IdentifiableCollection<node_id, NODE>> node_collection);
		BreadthFirstIterator<NODE, NODE_DATA, EDGE> operator++();
	};
}

#include "BreadthFirstIterator.hpp"
