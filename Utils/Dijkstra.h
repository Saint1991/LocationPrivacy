#pragma once
#include "stdafx.h"
#include "IRoutingModule.h"
#include "RouteInfo.h"

namespace Graph
{
	template <typename NODE, typename EDGE>
	class Dijkstra : public IRoutingModule<NODE, EDGE>
	{
	private:
		typedef std::pair<double, node_id> distance_info;

	public:
		Dijkstra();
		~Dijkstra();
		RouteInfo<EDGE> shortest_path(std::shared_ptr<const Collection::IdentifiableCollection<Graph::node_id, NODE>> node_collection, Graph::node_id from, Graph::node_id to);
	};
}

#include "Dijkstra.hpp"

