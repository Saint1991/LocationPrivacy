
///<summary>
/// コンストラクタ
///</summary>
template <typename GEONODEDATA, typename PATHDATA>
Geography::Map<GEONODEDATA, PATHDATA>::Map(std::unique_ptr<Graph::RoutingMethod<GEONODEDATA, PATHDATA>> routing_method) 
	: node_collection(std::make_shared<const Collection::IdentifiableCollection<std::shared_ptr<const Geography::GeoNode<GEONODEDATA, PATHDATA>>>>())
{
	build_map();
	routing_table = create_routing_table(routing_method);
}

///<summary>
/// デストラクタ
///</summary>
template <typename GEONODEDATA, typename PATHDATA>
Geography::Map<GEONODEDATA, PATHDATA>::~Map()
{
}


///<summary>
/// 最短路のルーティングテーブルを構築します．
///</summary>
template <typename GEONODEDATA, typename PATHDATA>
void Geography::Map<GEONODEDATA, PATHDATA>::create_routing_table(std::unique_ptr<Graph::RoutingMethod<GEONODEDATA, PATHDATA>> routing_method)
{
	routing_table = std::move(routing_method->create_routing_table(node_collection));
}


///<summary>
/// fromからtoまで平均速度avg_speedで移動した際の所要時間を計算します
/// 到達不可能な場合はNOWHEREを返します．
///</summary>
template <typename GEONODEDATA, typename PATHDATA>
const unsigned long Geography::Map<GEONODEDATA, PATHDATA>::calc_necessary_time(const Graph::node_id& source, const Graph::node_id& destination, const double& avg_speed) const
{

	std::list<Graph::node_id> shortest_path = get_shortest_path();
	if (shortest_path == nullptr) return NOWHERE;

	Graph::node_id from = source;
	Graph::node_id to;
	unsigned long necessary_time = 0UL;
	for (std::list<Graph::node_id>::iterator iter = shortest_path.begin(); iter != shortest_path.end(); iter++) {

		//ルーティングリストから次のノードのIDを取得
		to = *iter;

		//nodeコレクションからfromに該当するノードを取得
		std::shared_ptr<const Geography::GeoNode<GEONODEDATA, PATHDATA>> target = node_collection->read_by_id(from);
		if (target == nullptr) return NOWHERE;

		//nodeが持つtoへのエッジを取得
		std::shared_ptr<Graph::Edge<PATHDATA>> path = target->get_edge_to(to);
		if (path == nullptr) return NOWHERE;

		//edgeの距離を調べる (GeoNodeはPATHDATAをBasicPathDataの派生クラスに絞っているので，このキャストは安全なはず)
		std::shared_ptr<const Geography::BasicPathData> path_data = std::static_pointer_cast<Geography::BasicPathData>(path);
		double distance = path_data->get_distance();

		//所用時間に加算
		necessary_time += distance / avg_speed;

		//探索を進める
		from = to;
	}

	return necessary_time;
}

///<summary>
/// fromからtoへ平均速度speedで移動したときの制限時間time_limit[分]の到達可能性を調べる
///</summary>
template <typename GEONODEDATA, typename PATHDATA>
const bool Geography::Map<GEONODEDATA, PATHDATA>::is_reachable(const Graph::node_id& from, const Graph::node_id& to, const double& speed, const unsigned int& time_limit) const
{
	return calc_necessary_time(from, to, speed) <= time_limit;
}


///<summary>
/// from から toへ最短路で行くときに次に進むべきノードのIDを取得
///</summary>
template <typename GEONODEDATA, typename PATHDATA>
const Graph::node_id Geography::Map<GEONODEDATA, PATHDATA>::get_next_node_of_shortest_path(const Graph::node_id& from, const Graph::node_id& to) const
{
	return routing_table->at(from)->at(to);
}


///<summary>
/// node_idの系列で経路を返す．
/// NOWHEREで探索が終了した場合はnullptrを返す
/// source == destinationの場合は空のリストが返されます．
///</summary>
template <typename GEONODEDATA, typename PATHDATA>
const std::list<Graph::node_id> Geography::Map<GEONODEDATA, PATHDATA>::get_shortest_path(const Graph::node_id& source, const Graph::node_id& destination) const
{
	std::list<Graph::node_id> path;
	if (source == destination) return path;

	Graph::node_id iter = source;
	do {
		iter = get_next_node_of_shortest_path(iter, destination);
		if (iter == NOWHERE) break;
		path.push_back(iter);
	} while (iter != destination && iter != NOWHERE)

		//NOWHEREで終了した場合は経路なしとしてnullptrを返す
		if (path.back() != destination) return nullptr;
	return path;
}


///<summary>
/// 指定したIDのノードに接続しているすべてのノードを返す
///</summary>
template <typename GEONODEDATA, typename PATHDATA>
const std::list<Graph::node_id> Geography::Map<GEONODEDATA, PATHDATA>::get_connecting_nodes(const Graph::node_id& id) const
{
	std::shared_ptr<Geography::GeoNode<GEONODEDATA, PATHDATA> const> node = node_collection->read_by_id(id);
	return node->get_connecting_node_list();
}


///<summary>
/// 指定したIDのノードを変更不可の状態で取得する
///</summary>
template <typename GEONODEDATA, typename PATHDATA>
std::shared_ptr<const Geography::GeoNode<GEONODEDATA, PATHDATA>> Geography::Map<GEONODEDATA, PATHDATA>::get_static_node(const Graph::node_id& id) const
{
	return node_collection->read_by_id(id);
}


///<summary>
/// 指定したIDのノードを取得する
///</summary>
template <typename GEONODEDATA, typename PATHDATA>
std::shared_ptr<Geography::GeoNode<GEONODEDATA, PATHDATA>> Geography::Map<GEONODEDATA, PATHDATA>::get_node(const Graph::node_id& id)
{
	return node_collection->get_by_id(id);
}
