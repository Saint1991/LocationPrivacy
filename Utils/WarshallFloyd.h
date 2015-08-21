#pragma once
#include "RoutingMethod.h"

namespace Graph
{
	template <typename NODE>
	class WarshallFloyd : public RoutingMethod<NODE> 
	{
	public:
		WarshallFloyd();
		virtual ~WarshallFloyd();
		std::unique_ptr<const std::vector<const std::vector<node_id>>>
			create_routing_table(std::shared_ptr<const Collection::IdentifiableCollection<std::shared_ptr<NODE const>>> node_collection) const;
	};
}

#include "WarshallFloyd.hpp"