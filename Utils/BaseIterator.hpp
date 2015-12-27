
namespace Graph
{

	///<summary>
	/// コンストラクタ
	///</summary>
	template <typename NODE, typename NODE_DATA, typename EDGE>
	BaseIterator<NODE, NODE_DATA, EDGE>::BaseIterator(node_id id, std::shared_ptr<Collection::IdentifiableCollection<node_id, NODE>> node_collection) 
		: BaseConstIterator<NODE, NODE_DATA, EDGE>(id, node_collection)
	{

	}

	///<summary>
	/// デストラクタ
	///</summary>
	template <typename NODE, typename NODE_DATA, typename EDGE>
	BaseIterator<NODE, NODE_DATA, EDGE>::~BaseIterator()
	{

	}

	///<summary>
	/// 参照外し
	///</summary>
	template <typename NODE, typename NODE_DATA, typename EDGE>
	std::shared_ptr<NODE> BaseIterator<NODE, NODE_DATA, EDGE>::operator*()
	{
		return node;
	}

	///<summary>
	/// 参照外し
	///</summary>
	template <typename NODE, typename NODE_DATA, typename EDGE>
	std::shared_ptr<NODE> BaseIterator<NODE, NODE_DATA, EDGE>::operator->()
	{
		return node;
	}

	///<summary>
	/// 条件に合う子要素を検索してイテレータを取得する
	/// 条件に合う要素がない場合はnullptrを指すイテレータを返します
	///</summary>
	template <typename NODE, typename NODE_DATA, typename EDGE>
	BaseIterator<NODE, NODE_DATA, EDGE> BaseIterator<NODE, NODE_DATA, EDGE>::find_child_if(const std::function<bool(std::shared_ptr<NODE const>)>& compare)
	{
		BaseIterator<NODE, NODE_DATA, EDGE> ret(-1, nullptr);
		if (node == nullptr) return ret;
		node->for_each_edge([&](std::shared_ptr<EDGE const> edge) {
			node_id to = edge->get_to();
			std::shared_ptr<NODE const> child_node = node_collection->read_by_id(to);
			if (compare(child_node)) ret = BaseIterator<NODE, NODE_DATA, EDGE>(to, node_collection);
		});
		return ret;
	}


	///<summary>
	/// イテレータが指している要素の子要素に対して，childを追加する
	///</summary>
	template <typename NODE, typename NODE_DATA, typename EDGE>
	void BaseIterator<NODE, NODE_DATA, EDGE>::add_child(std::shared_ptr<NODE> child)
	{
		Graph::node_id child_id = child->get_id();
		if (!node_collection->contains(child_id)) {
			node_collection->add(child);
		}
		if (!node->is_connecting_to(child_id)) {
			node->connect_to(std::make_shared<EDGE>(child->get_id()));
		}
	}
}

