#pragma once
#include "RoutingMethod.h"

namespace Graph
{
	template <typename NODE_DATA, typename EDGE_DATA>
	class WarshallFloyd : public RoutingMethod<NODE_DATA, EDGE_DATA> 
	{
	private:
		std::shared_ptr<const Collection::IdentifiableCollection<std::shared_ptr<const Node<NODE_DATA, EDGE_DATA>>>> node_collection;

	public:
		WarshallFloyd(std::shared_ptr<const Collection::IdentifiableCollection<std::shared_ptr<const Node<NODE_DATA, EDGE_DATA>>>> node_collection);
		virtual ~WarshallFloyd();
		std::unique_ptr<const std::vector<std::vector<Graph::node_id>>> create_routing_table() const;
	};
}

#include "WarshallFloyd.hpp"