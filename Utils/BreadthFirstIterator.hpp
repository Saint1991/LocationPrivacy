
namespace Graph
{

	///<summary>
	/// �R���X�g���N�^
	///</summary>
	template <typename NODE, typename NODE_DATA, typename EDGE>
	BreadthFirstIterator<NODE, NODE_DATA, EDGE>::BreadthFirstIterator() : BaseIterator<NODE, NODE_DATA, EDGE>(root_node)
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
	/// �R���X�g���N�^
	///</summary>
	template <typename NODE, typename NODE_DATA, typename EDGE>
	BreadthFirstIterator<NODE, NODE_DATA, EDGE>::BreadthFirstIterator(std::shared_ptr<NODE> node) : BaseIterator<NODE, NODE_DATA, EDGE>(node)
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
	/// �C���N�������g
	///</summary>
	template <typename NODE, typename NODE_DATA, typename EDGE>
	BreadthFirstIterator<NODE, NODE_DATA, EDGE> BreadthFirstIterator<NODE, NODE_DATA, EDGE>::operator++()
	{
		if (queue.size() == 0) return BreadthFirstIterator<NODE, NODE_DATA, EDGE>(nullptr);
		node_id id = queue.front();
		queue.pop();

		std::shared_ptr<NODE> node = node_collection->get_by_id(id);
		BreadthFirstIterator<NODE, NODE_DATA, EDGE> ret(node);

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

