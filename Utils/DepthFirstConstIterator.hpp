namespace Graph
{

	///<summary>
	/// コンストラクタ
	///</summary>
	template <typename NODE, typename NODE_DATA, typename EDGE>
	DepthFirstConstIterator<NODE, NODE_DATA, EDGE>::DepthFirstConstIterator(node_id id, std::shared_ptr<Collection::IdentifiableCollection<node_id, NODE>> node_collection) 
		: BaseConstIterator<NODE, NODE_DATA, EDGE>(id, node_collection)
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
	DepthFirstConstIterator<NODE, NODE_DATA, EDGE>& DepthFirstConstIterator<NODE, NODE_DATA, EDGE>::operator++(int)
	{
		if (stack.size() == 0) return DepthFirstConstIterator<NODE, NODE_DATA, EDGE>(-1, nullptr);
		node_id id = stack.top();
		stack.pop();

		node = node_collection->get_by_id(id);
		node->rfor_each_edge([&](std::shared_ptr<EDGE const> edge) {
			node_id to = edge->get_to();
			if (visited_node_ids.find(to) == visited_node_ids.end()) {
				visited_node_ids.insert(to);
				stack.push(to);
			}
		});

		return *this;
	}
}
