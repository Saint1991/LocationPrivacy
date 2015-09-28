namespace Graph
{

	///<summary>
	/// コンストラクタ
	///</summary>
	template <typename NODE, typename NODE_DATA, typename EDGE>
	PrefixTree<NODE, NODE_DATA, EDGE>::DepthFirstConstIterator::DepthFirstConstIterator() : PrefixTree<NODE, NODE_DATA, EDGE>::BaseConstIterator(root_node)
	{
		if (root_node != nullptr) {
			visited_node_ids.insert(root_node->get_id());
			root_node->rfor_each_edge([&](std::shared_ptr<EDGE const> edge) {
				node_id to = edge->get_to();
				if (visited_node_ids.find(to) == visited_node_ids.end()) {
					visited_node_ids.insert(to);
					stack.push(to);
				}
			});
		}
	}


	///<summary>
	/// コンストラクタ
	///</summary>
	template <typename NODE, typename NODE_DATA, typename EDGE>
	PrefixTree<NODE, NODE_DATA, EDGE>::DepthFirstConstIterator::DepthFirstConstIterator(std::shared_ptr<NODE const> node) : PrefixTree<NODE, NODE_DATA, EDGE>::BaseConstIterator(node)
	{
		if (node != nullptr) {
			visited_node_ids.insert(node->get_id());
			node->rfor_each_edge([&](std::shared_ptr<EDGE const> edge) {
				node_id to = edge->get_to();
				if (visited_node_ids.find(to) == visited_node_ids.end()) {
					visited_node_ids.insert(to);
					stack.push(to);
				}
			});
		}
	}


	///<summary>
	/// インクリメント
	///</summary>
	template <typename NODE, typename NODE_DATA, typename EDGE>
	PrefixTree<NODE, NODE_DATA, EDGE>::DepthFirstConstIterator PrefixTree<NODE, NODE_DATA, EDGE>::DepthFirstConstIterator::operator++()
	{
		if (stack.size() == 0) return PrefixTree<NODE, NODE_DATA, EDGE>::DepthFirstIterator(nullptr);
		node_id id = stack.top();
		stack.pop();

		std::shared_ptr<NODE const> node = node_collection->read_by_id(id);
		PrefixTree<NODE, NODE_DATA, EDGE>::DepthFirstIterator ret(node);

		node->rfor_each_edge([&](std::shared_ptr<EDGE const> edge) {
			node_id to = edge->get_to();
			if (visited_node_ids.find(to) == visited_node_ids.end()) {
				visited_node_ids.insert(to);
				stack.push(to);
			}
		});

		return ret;
	}
}
