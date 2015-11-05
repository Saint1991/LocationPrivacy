#pragma once
#include "stdafx.h"

namespace Graph
{
	template <typename EDGE>
	struct RouteInfo
	{
		std::shared_ptr<std::vector<node_id>> route;
		double total_distance;
		
		RouteInfo();
		RouteInfo(std::shared_ptr < std::vector <node_id>> route, double total_distance = DBL_MAX);
		~RouteInfo();
	};
}

#include "RouteInfo.hpp"

