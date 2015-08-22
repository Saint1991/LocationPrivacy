#pragma once
#include "stdafx.h"
#include "RoutingMethod.h"
#include "IdentifiableCollection.h"

namespace Graph
{
	template <typename NODE, typename EDGE>
	class WarshallFloyd : public RoutingMethod<NODE, EDGE> 
	{
	private:
		std::unique_ptr<std::unordered_map<node_id, int>> conversion_map;
		std::unique_ptr<std::vector<std::vector<double>>> distance_map;
		
		long node_count = -1L;
		void initialize();
		void 	create_conversion_map(std::shared_ptr<const Collection::IdentifiableCollection<NODE const>> node_collection);
		void initialize_distance_map(std::shared_ptr<const Collection::IdentifiableCollection<NODE const>> node_collection);
		
	public:
		WarshallFloyd();
		virtual ~WarshallFloyd();
		std::unique_ptr<RoutingTable const> 
			create_routing_table(std::shared_ptr<const Collection::IdentifiableCollection<NODE const>> node_collection) const;
	};
}

#include "WarshallFloyd.hpp"