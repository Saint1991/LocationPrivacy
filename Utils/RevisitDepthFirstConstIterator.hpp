
namespace Graph
{

	///<summary>
	/// コンストラクタ
	///</summary>
	template <typename NODE, typename NODE_DATA, typename EDGE>
	RevisitDepthFirstConstIterator<NODE, NODE_DATA, EDGE>::RevisitDepthFirstConstIterator(node_id id, std::shared_ptr<Collection::IdentifiableCollection<node_id, NODE>> node_collection)
		: BaseConstIterator<NODE, NODE_DATA, EDGE>(id, node_collection)
	{
		if (node != nullptr) {
			node->rfor_each_edge([&](std::shared_ptr<EDGE const> edge) {
				stack.push(edge->get_to());
			});
		}
	}


	///<summary>
	/// デストラクタ
	///</summary>
	template <typename NODE, typename NODE_DATA, typename  EDGE>
	RevisitDepthFirstConstIterator<NODE, NODE_DATA, EDGE>::~RevisitDepthFirstConstIterator()
	{

	}


	template <typename NODE, typename NODE_DATA, typename EDGE>
	RevisitDepthFirstConstIterator<NODE, NODE_DATA, EDGE>& RevisitDepthFirstConstIterator<NODE, NODE_DATA, EDGE>::operator++(int)
	{
		if (stack.size() == 0) {
			node = nullptr;
			node_collection = nullptr;
			return *this;
		}

		node_id id = stack.top();
		stack.pop();

		node = node_collection->get_by_id(id);
		node->rfor_each_edge([&](std::shared_ptr<EDGE const> edge) {
			stack.push(edge->get_to());
		});

		return *this;
	}
}

