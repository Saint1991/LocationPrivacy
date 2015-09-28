namespace Graph
{

	///<summary>
	/// �R���X�g���N�^
	///</summary>
	template <typename NODE, typename NODE_DATA, typename EDGE>
	DepthFirstConstIterator<NODE, NODE_DATA, EDGE>::DepthFirstConstIterator() : BaseConstIterator<NODE, NODE_DATA, EDGE>(root_node)
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
	/// �R���X�g���N�^
	///</summary>
	template <typename NODE, typename NODE_DATA, typename EDGE>
	DepthFirstConstIterator<NODE, NODE_DATA, EDGE>::DepthFirstConstIterator(std::shared_ptr<NODE const> node) : BaseConstIterator<NODE, NODE_DATA, EDGE>(node)
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
	/// �C���N�������g
	///</summary>
	template <typename NODE, typename NODE_DATA, typename EDGE>
	DepthFirstConstIterator<NODE, NODE_DATA, EDGE> DepthFirstConstIterator<NODE, NODE_DATA, EDGE>::operator++()
	{
		if (stack.size() == 0) return DepthFirstIterator<NODE, NODE_DATA, EDGE>(nullptr);
		node_id id = stack.top();
		stack.pop();

		std::shared_ptr<NODE const> node = node_collection->read_by_id(id);
		DepthFirstIterator<NODE, NODE_DATA, EDGE> ret(node);

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
