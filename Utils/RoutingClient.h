#pragma once
#include "IdentifiableCollection.h"
#include "IRoutingModule.h"
#include "RouteInfo.h"

namespace Graph
{

	constexpr double NO_CONNECTION = DBL_MAX;

	template <typename NODE, typename EDGE>
	class RoutingClient
	{
	protected:
		std::shared_ptr<const Collection::IdentifiableCollection<Graph::node_id, NODE>> node_collection;
		std::shared_ptr<IRoutingModule<NODE, EDGE>> routing_method;
	public:
		RoutingClient(std::shared_ptr<const Collection::IdentifiableCollection<Graph::node_id, NODE>> node_collection, std::shared_ptr<IRoutingModule<NODE, EDGE>> routing_method);
		~RoutingClient();

		double shortest_distance(Graph::node_id from, Graph::node_id to, double distance_threshold = DBL_MAX);
		std::shared_ptr<std::vector<node_id>> shortest_path(Graph::node_id from, Graph::node_id to, double distance_threshold = DBL_MAX);
		RouteInfo<EDGE> shortest_path_info(Graph::node_id from, Graph::node_id to, double distance_threshold = DBL_MAX);
	};
}

#include "RoutingClient.hpp"

