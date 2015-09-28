#pragma once
#include "stdafx.h"
#include "PrefixTree.h"

namespace Graph
{

	template <typename NODE, typename NODE_DATA, typename EDGE>
	class BaseConstIterator : public std::iterator<std::forward_iterator_tag, NODE>
	{
	friend class PrefixTree<NODE, NODE_DATA, EDGE>;

	protected:
		std::shared_ptr<NODE const> node;
		BaseConstIterator();
		BaseConstIterator(std::shared_ptr<NODE const> node);
		virtual ~BaseConstIterator();
	public:
		std::shared_ptr<NODE const> operator*() const;
		BaseConstIterator<NODE, NODE_DATA, EDGE> find_child_if(const std::function<bool(std::shared_ptr<NODE const>)>& compare) const;
		bool operator==(const BaseConstIterator<NODE, NODE_DATA, EDGE>& iter) const;
		bool operator!=(const BaseConstIterator<NODE, NODE_DATA, EDGE>& iter) const;
	};
}

#include "BaseConstIterator.h"