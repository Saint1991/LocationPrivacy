
namespace Graph
{

	///<summary>
	/// コンストラクタ
	///</summary>
	template <typename NODE, typename NODE_DATA, typename EDGE>
	PrefixTree<NODE, NODE_DATA, EDGE>::BreadthFirstConstIterator::BreadthFirstConstIterator() : PrefixTree<NODE, NODE_DATA, EDGE>::BaseConstIterator(root_node)
	{
		if (root_node != nullptr) {
			visited_node_ids.insert(root_node->get_id());
			root_node->for_each_edge([&](std::shared_ptr<EDGE const> edge) {
				node_id to = edge->get_to();
				if (visited_node_ids.find(to) == visited_node_ids.end()) {
					visited_node_ids.insert(to);
					queue.push(to);
				}
			});
		}
	}

	///<summary>
	/// コンストラクタ
	///</summary>
	template <typename NODE, typename NODE_DATA, typename EDGE>
	PrefixTree<NODE, NODE_DATA, EDGE>::BreadthFirstConstIterator::BreadthFirstConstIterator(std::shared_ptr<NODE const> node) :PrefixTree<NODE, NODE_DATA, EDGE>::BaseConstIterator(node)
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
	PrefixTree<NODE, NODE_DATA, EDGE>::BreadthFirstConstIterator PrefixTree<NODE, NODE_DATA, EDGE>::BreadthFirstConstIterator::operator++()
	{
		if (queue.size() == 0) return PrefixTree<NODE, NODE_DATA, EDGE>::BreadthFirstIterator(nullptr);
		node_id id = queue.front();
		queue.pop();

		std::shared_ptr<NODE const> node = node_collection->read_by_id(id);
		PrefixTree<NODE, NODE_DATA, EDGE>::BreadthFirstIterator ret(node);

		node->for_each_edge([&](std::shared_ptr<EDGE const> edge) {
			node_id to = edge->get_to();
			if (visited_node_ids.find(to) == visited_node_ids.end()) {
				visited_node_ids.insert(to);
				queue.push(to);
			}
		});

		return ret;
	}
}
