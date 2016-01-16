
namespace Graph
{

	template <typename NODE, typename EDGE>
	Dijkstra<NODE, EDGE>::Dijkstra()
	{
	}

	template <typename NODE, typename EDGE>
	Dijkstra<NODE, EDGE>::~Dijkstra()
	{
	}


	///<summary>
	/// Dijkstra法により2点間の最短路を求める
	/// 不正な値の場合は(nullptr, DBL_MAX)が返る
	/// 参考(https://ja.wikipedia.org/wiki/ダイクストラ法)
	/// distance_thresholdを設定すると距離がそれ以上になった場合の探索を打ち切ります
	/// その場合はfromからtoへの経路が完成していなければ(nullptr, DBL_MAX)が返ります
	///</summary>
	template <typename NODE, typename EDGE>
	RouteInfo<EDGE> Dijkstra<NODE, EDGE>::shortest_path(std::shared_ptr<const Collection::IdentifiableCollection<Graph::node_id, NODE>> node_collection, Graph::node_id from, Graph::node_id to, double distance_threshold)
	{

		if (from == to) return RouteInfo<EDGE>(std::make_shared<std::vector<node_id>>(), 0.0);

		//初期化
		size_t node_count = node_collection->size();

		//優先度つきキューの宣言
		std::unique_ptr<std::priority_queue<distance_info, std::vector<distance_info>, std::greater<distance_info>>> queue
			= std::make_unique<std::priority_queue<distance_info, std::vector<distance_info>, std::greater<distance_info>>>();

		//始点からインデックスiのノードに到達する最短経路におけるiの前のノードのインデックス
		std::unique_ptr<std::vector<int>> previous = std::make_unique<std::vector<int>>(node_count, NOWHERE);
		std::shared_ptr<std::vector<double>> distance_map = std::make_shared<std::vector<double>>(node_count, DBL_MAX);
		std::unique_ptr<std::unordered_map<node_id, int>> index_map = std::make_unique<std::unordered_map<node_id, int>>();
		std::unique_ptr<std::vector<node_id>> reverse_index_map = std::make_unique<std::vector<node_id>>(node_count);

		int index = 0;
		node_collection->foreach([&](std::shared_ptr<NODE const> node) {
			node_id id = node->get_id();
			index_map->insert(std::make_pair(id, index));
			reverse_index_map->at(index++) = id;
		});
		int index_from = index_map->at(from);
		distance_map->at(index_from) = 0.0;
		queue->push(std::make_pair(distance_map->at(index_from), from));

		//ここからが本計算
		while (!queue->empty()) {

			//コストが最小のノードを取り出す
			distance_info target = queue->top();
			double d = target.first;
			node_id v = target.second;
			queue->pop();

			//もし最小コストがdistance_thresholdを超えている場合は探索を打ち切る
			if (d > distance_threshold || v == to) break;
			
			std::shared_ptr<NODE const> node = node_collection->read_by_id(target.second);
			node->for_each_edge([&](std::shared_ptr<EDGE const> edge) {
				node_id u = edge->get_to();
				double alt = d + edge->get_static_data()->get_distance();
				int index_u = index_map->at(u);
				double distance_u = distance_map->at(index_u);
				if (alt < distance_u) {
					distance_map->at(index_u) = alt;
					queue->push(std::make_pair(alt, u));
					previous->at(index_u) = index_map->at(v);
				}
			});
		}

		//previousをたどって経路を作成
		int index_to = index_map->at(to);
		double total_distance = distance_map->at(index_to);
		if (total_distance > distance_threshold) return RouteInfo<EDGE>(nullptr, DBL_MAX);

		std::shared_ptr<std::vector<node_id>> path = std::make_shared<std::vector<node_id>>();
		int current_index = index_to;
		while (current_index != NOWHERE) {
			path->push_back(reverse_index_map->at(current_index));
			current_index = previous->at(current_index);
		}
		if (path->back() != from || path->front() != to) return RouteInfo<EDGE>(nullptr, DBL_MAX);
		path->pop_back();
		std::reverse(path->begin(), path->end());
		
		return RouteInfo<EDGE>(path, total_distance);
	}
}


