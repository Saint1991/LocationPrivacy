
namespace Graph
{
	
	///<summary>
	/// コンストラクタ
	///</summary>
	template <typename NODE, typename POI, typename PATH>
	Map<NODE, POI, PATH>::Map()
		: node_collection(std::make_shared<const Collection::IdentifiableCollection<Graph::node_id, NODE>>())
	{		
	}

	///<summary>
	/// デストラクタ
	///</summary>
	template <typename NODE, typename POI, typename PATH>
	Map<NODE, POI, PATH>::~Map()
	{
	}


	///<summary>
	/// Mapの初期化
	/// 最初に必ずこれを呼ぶ必要があることに注意
	///</summary>
	template <typename NODE, typename POI, typename PATH>
	void Map<NODE, POI, PATH>::initialize(std::unique_ptr<RoutingMethod<NODE, PATH>> routing_method)
	{
		build_map();
		routing_table = routing_method->create_routing_table(node_collection);
	}

	///<summary>
	/// from から toへ最短路で行くときに次に進むべきノードのIDを取得
	/// 接続していない場合はNOWHEREが返る
	///</summary>
	template <typename NODE, typename POI, typename PATH>
	node_id Map<NODE, POI, PATH>::get_next_node_of_shortest_path(const node_id& from, const node_id& to) const
	{
		return routing_table->get_next_node_of_shortest_path(from, to);
	}


	///<summary>
	/// node_idの系列で経路を返す．
	/// 到達不可能な場合はnullptrを返す
	/// source == destinationの場合は空のリストが返されます．
	///</summary>
	template <typename NODE, typename POI, typename PATH>
	const std::list<node_id> Map<NODE, POI, PATH>::get_shortest_path(const node_id& source, const node_id& destination) const
	{
		return routing_table->get_shortest_path(source, destination);
	}



	///<summary>
	/// fromからtoまで平均速度avg_speed[m/s]で移動した際の所要時間を計算します
	/// 到達不可能な場合はNO_CONNECTIONを返します．
	///</summary>
	template <typename NODE, typename POI, typename PATH>
	double Map<NODE, POI, PATH>::calc_necessary_time(const node_id& source, const node_id& destination, const double& avg_speed) const
	{
		double shortest_distance = routing_table->shortest_distance(source, destination);
		return shortest_distance / avg_speed;
	}

	///<summary>
	/// fromからtoへ平均速度speed[m/s]で移動したときの制限時間time_limit[s]の到達可能性を調べる
	///</summary>
	template <typename NODE, typename POI, typename PATH>
	bool Map<NODE, POI, PATH>::is_reachable(const node_id& from, const node_id& to, const double& speed, const double& time_limit) const
	{
		return calc_necessary_time(from, to, speed) <= time_limit;
	}


	///<summary>
	/// 指定したIDのノードに接続しているすべてのノードを返す
	///</summary>
	template <typename NODE, typename POI, typename PATH>
	const std::list<node_id> Map<NODE, POI, PATH>::get_connecting_nodes(const node_id& id) const
	{
		std::shared_ptr<NODE const> node = node_collection->read_by_id(id);
		return node->get_connecting_node_list();
	}


	///<summary>
	/// 指定したIDのノードを変更不可の状態で取得する
	///</summary>
	template <typename NODE, typename POI, typename PATH>
	std::shared_ptr<NODE const> Map<NODE, POI, PATH>::get_static_node(const node_id& id) const
	{
		return node_collection->read_by_id(id);
	}
}

