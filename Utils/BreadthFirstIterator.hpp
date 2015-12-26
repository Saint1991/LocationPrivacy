
namespace Graph
{


	///<summary>
	/// コンストラクタ
	///</summary>
	template <typename NODE, typename NODE_DATA, typename EDGE>
	BreadthFirstIterator<NODE, NODE_DATA, EDGE>::BreadthFirstIterator(node_id id, std::shared_ptr<Collection::IdentifiableCollection<node_id, NODE>> node_collection) 
		: BaseIterator<NODE, NODE_DATA, EDGE>(id, node_collection)
	{
		if (node != nullptr) {
			visited_node_ids.insert(node->get_id());
			node->for_each_edge([&](std::shared_ptr<EDGE const> edge) {
				node_id to = edge->get_to();
				if (visited_node_ids.find(to) == visited_node_ids.end()) {
					visited_node_ids.insert(to);
					queue.push(to);
				}
			});
		}
	}


	///<summary>
	/// インクリメント
	///</summary>
	template <typename NODE, typename NODE_DATA, typename EDGE>
	BreadthFirstIterator<NODE, NODE_DATA, EDGE>& BreadthFirstIterator<NODE, NODE_DATA, EDGE>::operator++()
	{
		if (queue.size() == 0) return BreadthFirstIterator<NODE, NODE_DATA, EDGE>(-1, nullptr);
		node_id id = queue.front();
		queue.pop();

		node = node_collection->get_by_id(id);
		node->for_each_edge([&](std::shared_ptr<EDGE const> edge) {
			node_id to = edge->get_to();
			if (visited_node_ids.find(to) == visited_node_ids.end()) {
				visited_node_ids.insert(to);
				queue.push(to);
			}
		});

		return *this;
	}
}

