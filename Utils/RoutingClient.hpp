
namespace Graph
{

	///<summary>
	/// コンストラクタ
	///</summary>
	template <typename NODE, typename EDGE>
	RoutingClient<NODE, EDGE>::RoutingClient(std::shared_ptr<const Collection::IdentifiableCollection<Graph::node_id, NODE>> node_collection, std::shared_ptr<IRoutingModule<NODE, EDGE>> routing_method)
		: node_collection(node_collection)
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
	///</summary>
	template <typename NODE, typename EDGE>
	double RoutingClient<NODE, EDGE>::shortest_distance(Graph::node_id from, Graph::node_id to)
	{
		RouteInfo<EDGE> route_info = routing_method->shortest_path(node_collection, from, to);
		return route_info.total_distance;
	}


	///<summary>
	/// 最短路の交差点IDを指すエッジのリストを返します
	///</summary>
	template <typename NODE, typename EDGE>
	std::shared_ptr<std::vector<node_id>> RoutingClient<NODE, EDGE>::shortest_path(Graph::node_id from, Graph::node_id to)
	{
		RouteInfo<EDGE> route_info = routing_method->shortest_path(node_collection, from, to);
		return route_info.route;
	}
}

