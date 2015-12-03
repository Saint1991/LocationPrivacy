
namespace Graph
{

	///<summary>
	/// コンストラクタ
	///</summary>
	template <typename NODE, typename NODE_DATA, typename EDGE>
	DepthFirstIterator<NODE, NODE_DATA, EDGE>::DepthFirstIterator(node_id id, std::shared_ptr<Collection::IdentifiableCollection<node_id, NODE>> node_collection)
		: BaseIterator<NODE, NODE_DATA, EDGE>(id, node_collection)
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
	DepthFirstIterator<NODE, NODE_DATA, EDGE> DepthFirstIterator<NODE, NODE_DATA, EDGE>::operator++()
	{
		if (stack.size() == 0) return Tree<NODE, NODE_DATA, EDGE>::DepthFirstIterator(-1, nullptr);
		node_id id = stack.top();
		stack.pop();

		Tree<NODE, NODE_DATA, EDGE>::DepthFirstIterator ret(id, node_collection);

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
