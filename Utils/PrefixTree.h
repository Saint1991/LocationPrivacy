#pragma once
#include "stdafx.h"
#include "IdentifiableCollection.h"
#include "Node.h"
#include "BaseIterator.h"
#include "BaseConstIterator.h"
#include "DepthFirstIterator.h"
#include "DepthFirstConstIterator.h"
#include "BreadthFirstIterator.h"
#include "BreadthFirstConstIterator.h"

namespace Graph
{
	template <typename NODE, typename NODE_DATA, typename EDGE>
	class PrefixTree
	{
	static_assert(std::is_base_of<Node<NODE_DATA, EDGE>, NODE>::value, "template type NODE must be derived from Node<NODE_DATA, EDGE>");
	protected:
		std::unique_ptr<Collection::IdentifiableCollection<node_id, NODE>> node_collection;
		std::shared_ptr<NODE> root_node = nullptr;
		
	public:

		#pragma region Iterators
		typedef BaseIterator<NODE, NODE_DATA, EDGE> base_iterator;
		typedef BaseConstIterator<NODE, NODE_DATA, EDGE> base_const_iterator;
		typedef DepthFirstIterator<NODE, NODE_DATA, EDGE> depth_first_iterator;
		typedef DepthFirstConstIterator<NODE, NODE_DATA, EDGE> depth_first_const_iterator;
		typedef BreadthFirstIterator<NODE, NODE_DATA, EDGE> breadth_first_iterator;
		typedef BreadthFirstConstIterator<NODE, NODE_DATA, EDGE> breadth_first_const_iterator;
		#pragma endregion

		PrefixTree();
		virtual ~PrefixTree();

		void initialize(std::shared_ptr<NODE_DATA> root_data = nullptr);

		template <typename ITER_TYPE> ITER_TYPE root();
		template <typename ITER_TYPE> ITER_TYPE end();
		template <typename ITER_TYPE> ITER_TYPE get_iter_by_id(node_id id);
		bool insert(base_iterator iter, EDGE edge, NODE_DATA node_data);
	};
}

#include "PrefixTree.hpp"


