#pragma once
#include "RoutingMethod.h"

namespace Graph
{
	template <typename NODE_DATA, typename EDGE>
	class WarshallFloyd : public RoutingMethod<NODE_DATA, EDGE> 
	{
	public:
		WarshallFloyd();
		virtual ~WarshallFloyd();
		std::unique_ptr<const std::vector<std::vector<Graph::node_id>>> create_routing_table(std::shared_ptr<const Collection::IdentifiableCollection<std::shared_ptr<const Node<NODE_DATA, EDGE>>>> node_collection) const;
	};
}

#include "WarshallFloyd.hpp"