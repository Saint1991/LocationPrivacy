
namespace Graph
{

	///<summary>
	/// �R���X�g���N�^
	///</summary>
	template <typename NODE, typename EDGE>
	RoutingClient<NODE, EDGE>::RoutingClient(std::shared_ptr<const Collection::IdentifiableCollection<Graph::node_id, NODE>> node_collection, std::shared_ptr<IRoutingModule<NODE, EDGE>> routing_method)
		: node_collection(node_collection), routing_method(routing_method)
	{
	}

	///<summary>
	/// �f�X�g���N�^
	///</summary>
	template <typename NODE, typename EDGE>
	RoutingClient<NODE, EDGE>::~RoutingClient()
	{
	}


	///<summary>
	/// �ŒZ�H�ɂ�����from����to�܂ł̋�����Ԃ��܂�
	/// distance_threshold��ݒ肵���ꍇ�͍ŒZ�H��distance_threshold�𒴂��Ă���ꍇDBL_MAX���Ԃ���܂�
	///</summary>
	template <typename NODE, typename EDGE>
	double RoutingClient<NODE, EDGE>::shortest_distance(Graph::node_id from, Graph::node_id to, double distance_threshold)
	{
		RouteInfo<EDGE> route_info = routing_method->shortest_path(node_collection, from, to, distance_threshold);
		return route_info.total_distance;
	}


	///<summary>
	/// �ŒZ�H�̌����_ID���w���G�b�W�̃��X�g��Ԃ��܂�
	/// distance_threshold��ݒ肵���ꍇ�C�ŒZ�H����threshold�ȏ�̏ꍇ��nullptr��Ԃ�
	///</summary>
	template <typename NODE, typename EDGE>
	std::shared_ptr<std::vector<node_id>> RoutingClient<NODE, EDGE>::shortest_path(Graph::node_id from, Graph::node_id to, double distance_threshold)
	{
		RouteInfo<EDGE> route_info = routing_method->shortest_path(node_collection, from, to, distance_threshold);
		return route_info.route;
	}


	///<summary>
	/// �ŒZ�H�̌����_ID���w���G�b�W�̃��X�g��
	/// ���̋���������RouteInfo��Ԃ�
	/// distance_threshold��ݒ肵���ꍇ�C�ŒZ�H����threshold�ȏ�̏ꍇ��(nullptr, DBL_MAX)��Ԃ�
	///</summary>
	template <typename NODE, typename EDGE>
	RouteInfo<EDGE> RoutingClient<NODE, EDGE>::shortest_path_info(Graph::node_id from, Graph::node_id to, double distance_threshold)
	{
		return routing_method->shortest_path(node_collection, from, to, distance_threshold);
	}
}

