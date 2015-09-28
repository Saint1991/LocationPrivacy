
namespace Graph
{

	///<summary>
	/// コンストラクタ 
	///</summary>
	template <typename NODE, typename NODE_DATA, typename EDGE>
	DepthFirstIterator<NODE, NODE_DATA, EDGE>::DepthFirstIterator() : BaseIterator<NODE, NODE_DATA, EDGE>(root_node)
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
	DepthFirstIterator<NODE, NODE_DATA, EDGE>::DepthFirstIterator(std::shared_ptr<NODE> node) : BaseIterator<NODE, NODE_DATA, EDGE>(node)
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
		if (stack.size() == 0) return PrefixTree<NODE, NODE_DATA, EDGE>::DepthFirstIterator(nullptr);
		node_id id = stack.top();
		stack.pop();

		std::shared_ptr<NODE> node = node_collection->get_by_id(id);
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
