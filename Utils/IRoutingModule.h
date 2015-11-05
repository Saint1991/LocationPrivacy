#pragma once
#include "RouteInfo.h"
#include "BasicEdge.h"

namespace Graph
{
	template <typename NODE, typename EDGE>
	class IRoutingModule
	{
	public :
		virtual RouteInfo<EDGE> shortest_path(std::shared_ptr<const Collection::IdentifiableCollection<Graph::node_id, NODE>> node_collection, Graph::node_id from, Graph::node_id to) = 0;
	};
}
