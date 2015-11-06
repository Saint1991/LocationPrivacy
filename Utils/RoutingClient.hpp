
namespace Graph
{

	///<summary>
	/// コンストラクタ
	///</summary>
	template <typename NODE, typename EDGE>
	RoutingClient<NODE, EDGE>::RoutingClient(std::shared_ptr<const Collection::IdentifiableCollection<Graph::node_id, NODE>> node_collection, std::shared_ptr<IRoutingModule<NODE, EDGE>> routing_method)
		: node_collection(node_collection), routing_method(routing_method)
	{
	}

	///<summary>
	/// デストラクタ
	///</summary>
	template <typename NODE, typename EDGE>
	RoutingClient<NODE, EDGE>::~RoutingClient()
	{
	}


	///<summary>
	/// 最短路におけるfromからtoまでの距離を返します
	/// distance_thresholdを設定した場合は最短路がdistance_thresholdを超えている場合DBL_MAXが返されます
	///</summary>
	template <typename NODE, typename EDGE>
	double RoutingClient<NODE, EDGE>::shortest_distance(Graph::node_id from, Graph::node_id to, double distance_threshold)
	{
		RouteInfo<EDGE> route_info = routing_method->shortest_path(node_collection, from, to, distance_threshold);
		return route_info.total_distance;
	}


	///<summary>
	/// 最短路の交差点IDを指すエッジのリストを返します
	/// distance_thresholdを設定した場合，最短路がはthreshold以上の場合にnullptrを返す
	///</summary>
	template <typename NODE, typename EDGE>
	std::shared_ptr<std::vector<node_id>> RoutingClient<NODE, EDGE>::shortest_path(Graph::node_id from, Graph::node_id to, double distance_threshold)
	{
		RouteInfo<EDGE> route_info = routing_method->shortest_path(node_collection, from, to, distance_threshold);
		return route_info.route;
	}


	///<summary>
	/// 最短路の交差点IDを指すエッジのリストと
	/// その距離をもつRouteInfoを返す
	/// distance_thresholdを設定した場合，最短路がはthreshold以上の場合に(nullptr, DBL_MAX)を返す
	///</summary>
	template <typename NODE, typename EDGE>
	RouteInfo<EDGE> RoutingClient<NODE, EDGE>::shortest_path_info(Graph::node_id from, Graph::node_id to, double distance_threshold)
	{
		return routing_method->shortest_path(node_collection, from, to, distance_threshold);
	}
}

