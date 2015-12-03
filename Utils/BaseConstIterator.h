#pragma once
#include "stdafx.h"
#include "Tree.h"

namespace Graph
{

	template <typename NODE, typename NODE_DATA, typename EDGE>
	class BaseConstIterator : public std::iterator<std::forward_iterator_tag, NODE>
	{
	protected:
		std::shared_ptr<NODE> node = nullptr;
		std::shared_ptr<Collection::IdentifiableCollection<node_id, NODE>> node_collection = nullptr;
		
	public:
		BaseConstIterator(node_id id, std::shared_ptr<Collection::IdentifiableCollection<node_id, NODE>> node_collection);
		virtual ~BaseConstIterator();
		std::shared_ptr<NODE const> operator*() const;
		std::shared_ptr<NODE const> operator->() const;
		BaseConstIterator<NODE, NODE_DATA, EDGE> find_child_if(const std::function<bool(std::shared_ptr<NODE const>)>& compare) const;
		bool operator==(const BaseConstIterator<NODE, NODE_DATA, EDGE>& iter) const;
		bool operator!=(const BaseConstIterator<NODE, NODE_DATA, EDGE>& iter) const;
	};
}

#include "BaseConstIterator.hpp"