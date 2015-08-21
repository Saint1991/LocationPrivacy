
namespace Graph
{
	
	///<summary>
	/// コンストラクタ
	///</summary>
	template <typename NODE, typename PATH>
	Map<NODE, PATH>::Map(std::unique_ptr<RoutingMethod<NODE>> routing_method)
		: node_collection(std::make_shared<const Collection::IdentifiableCollection<std::shared_ptr<NODE>>>())
	{
		build_map();
		routing_table = routing_method->create_routing_table(node_collection);
	}

	///<summary>
	/// デストラクタ
	///</summary>
	template <typename NODE, typename PATH>
	Map<NODE, PATH>::~Map()
	{
	}

	///<summary>
	/// from から toへ最短路で行くときに次に進むべきノードのIDを取得
	///</summary>
	template <typename NODE, typename PATH>
	const node_id Map<NODE, PATH>::get_next_node_of_shortest_path(const node_id& from, const node_id& to) const
	{
		return routing_table->at(from)->at(to);
	}


	///<summary>
	/// node_idの系列で経路を返す．
	/// 到達不可能な場合はnullptrを返す
	/// source == destinationの場合は空のリストが返されます．
	///</summary>
	template <typename NODE, typename PATH>
	const std::list<node_id> Map<NODE, PATH>::get_shortest_path(const node_id& source, const node_id& destination) const
	{
		std::list<node_id> path;
		if (source == destination) return path;

		node_id iter = source;
		while (iter != destination && iter != NOWHERE) {
			iter = get_next_node_of_shortest_path(iter, destination);
			path.push_back(iter);
		} 

		//pathの最後尾がdestinationになっていない場合は経路が見つかっていないのでnullptrを返す
		if (path.back() != destination) return nullptr;
		return path;
	}



	///<summary>
	/// fromからtoまで平均速度avg_speed[m/s]で移動した際の所要時間を計算します
	/// 到達不可能な場合はNOWHEREを返します．
	///</summary>
	template <typename NODE, typename PATH>
	const unsigned long Map<NODE, PATH>::calc_necessary_time(const node_id& source, const node_id& destination, const double& avg_speed) const
	{

		const std::list<node_id> shortest_path = get_shortest_path();
		if (shortest_path == nullptr) return NOWHERE;

		node_id from = source;
		node_id to;
		unsigned long necessary_time = 0UL;
		for (std::list<node_id>::const_iterator iter = shortest_path.begin(); iter != shortest_path.end(); iter++) {

			//ルーティングリストから次のノードのIDを取得
			to = *iter;

			//nodeコレクションからfromに該当するノードを取得
			std::shared_ptr<NODE const> target = node_collection->read_by_id(from);
			if (target == nullptr) return NOWHERE;

			//nodeが持つtoへのエッジを取得
			std::shared_ptr<PATH> path = target->get_edge_to(to);
			if (path == nullptr) return NOWHERE;

			//edgeの距離を調べる
			auto path_data = path->get_static_data();
			if (path_data == nullptr) return NOWHERE;
			double distance = path_data->get_distance();

			//所用時間に加算
			necessary_time += distance / avg_speed;

			//探索を進める
			from = to;
		}

		return necessary_time;
	}

	///<summary>
	/// fromからtoへ平均速度speed[m/s]で移動したときの制限時間time_limit[s]の到達可能性を調べる
	///</summary>
	template <typename NODE, typename PATH>
	const bool Map<NODE, PATH>::is_reachable(const node_id& from, const node_id& to, const double& speed, const unsigned int& time_limit) const
	{
		return calc_necessary_time(from, to, speed) <= time_limit;
	}


	///<summary>
	/// 指定したIDのノードに接続しているすべてのノードを返す
	///</summary>
	template <typename NODE, typename PATH>
	const std::list<node_id> Map<NODE, PATH>::get_connecting_nodes(const node_id& id) const
	{
		std::shared_ptr<NODE const> node = node_collection->read_by_id(id);
		return node->get_connecting_node_list();
	}


	///<summary>
	/// 指定したIDのノードを変更不可の状態で取得する
	///</summary>
	template <typename NODE, typename PATH>
	std::shared_ptr<NODE const> Map<NODE, PATH>::get_static_node(const node_id& id) const
	{
		return node_collection->read_by_id(id);
	}


	///<summary>
	/// 指定したIDのノードを取得する
	///</summary>
	template <typename NODE, typename PATH>
	std::shared_ptr<NODE> Map<NODE, PATH>::get_node(const node_id& id)
	{
		return node_collection->get_by_id(id);
	}
}

