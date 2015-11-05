
namespace Graph
{
	template <typename EDGE>
	RouteInfo<EDGE>::RouteInfo()
	{
	}

	template <typename EDGE>
	RouteInfo<EDGE>::RouteInfo(std::shared_ptr<std::vector<node_id>> route, double total_distance)
		: route(route), total_distance(total_distance)
	{

	}

	template <typename EDGE>
	RouteInfo<EDGE>::~RouteInfo()
	{
	}

}


