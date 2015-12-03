#pragma once
#include "stdafx.h"
#include "Tree.h"
#include "BaseConstIterator.h"

namespace Graph
{

	///<summary>
	/// ���D��T���p�C�e���[�^
	///</summary>
	template <typename NODE, typename NODE_DATA, typename EDGE>
	class BreadthFirstConstIterator : public BaseConstIterator<NODE, NODE_DATA, EDGE> 
	{
	protected:
		std::set<node_id> visited_node_ids;
		std::queue<node_id> queue;

	public:
		BreadthFirstConstIterator(node_id id, std::shared_ptr<Collection::IdentifiableCollection<node_id, NODE>> node_collection);
		BreadthFirstConstIterator<NODE, NODE_DATA, EDGE> operator++();
	};
}

#include "BreadthFirstConstIterator.hpp"