
namespace Graph
{

	///<summary>
	/// コンストラクタ
	///</summary>
	template <typename NODE, typename NODE_DATA, typename EDGE, typename EDGE_DATA>
	BaseIterator<NODE, NODE_DATA, EDGE, EDGE_DATA>::BaseIterator(node_id id, std::shared_ptr<Collection::IdentifiableCollection<node_id, NODE>> node_collection) 
		: BaseConstIterator<NODE, NODE_DATA, EDGE>(id, node_collection)
	{

	}

	///<summary>
	/// デストラクタ
	///</summary>
	template <typename NODE, typename NODE_DATA, typename EDGE, typename EDGE_DATA>
	BaseIterator<NODE, NODE_DATA, EDGE, EDGE_DATA>::~BaseIterator()
	{

	}

	///<summary>
	/// 参照外し
	///</summary>
	template <typename NODE, typename NODE_DATA, typename EDGE, typename EDGE_DATA>
	std::shared_ptr<NODE> BaseIterator<NODE, NODE_DATA, EDGE, EDGE_DATA>::operator*()
	{
		return node;
	}

	///<summary>
	/// 参照外し
	///</summary>
	template <typename NODE, typename NODE_DATA, typename EDGE, typename EDGE_DATA>
	std::shared_ptr<NODE> BaseIterator<NODE, NODE_DATA, EDGE, EDGE_DATA>::operator->()
	{
		return node;
	}

	///<summary>
	/// 条件に合う子要素を検索してイテレータを取得する
	/// 条件に合う要素がない場合はnullptrを指すイテレータを返します
	///</summary>
	template <typename NODE, typename NODE_DATA, typename EDGE, typename EDGE_DATA>
	BaseIterator<NODE, NODE_DATA, EDGE, EDGE_DATA> BaseIterator<NODE, NODE_DATA, EDGE, EDGE_DATA>::find_child_if(const std::function<bool(std::shared_ptr<NODE const>)>& compare)
	{
		BaseIterator<NODE, NODE_DATA, EDGE, EDGE_DATA> ret(-1, nullptr);
		if (node == nullptr) return ret;
		node->for_each_edge([&](std::shared_ptr<EDGE const> edge) {
			node_id to = edge->get_to();
			std::shared_ptr<NODE const> child_node = node_collection->read_by_id(to);
			if (compare(child_node)) ret = BaseIterator<NODE, NODE_DATA, EDGE, EDGE_DATA>(to, node_collection);
		});
		return ret;
	}


	///<summary>
	/// イテレータが指している要素の子要素に対して，childを追加する
	/// edge_dataは追加するノードを指すエッジに保持される
	///</summary>
	template <typename NODE, typename NODE_DATA, typename EDGE, typename EDGE_DATA>
	void BaseIterator<NODE, NODE_DATA, EDGE, EDGE_DATA>::add_child(std::shared_ptr<NODE> child, std::shared_ptr<EDGE_DATA> edge_data)
	{
		Graph::node_id child_id = child->get_id();
		if (!node_collection->contains(child_id)) {
			node_collection->add(child);
		}
		if (!node->is_connecting_to(child_id)) {
			node->connect_to(std::make_shared<EDGE>(child->get_id(), edge_data));
		}
	}

	///<summary>
	/// イテレータが指している要素の子要素に対して，childを追加する
	/// edge_dataは追加するノードを指すエッジに保持される
	///</summary>
	template <typename NODE, typename NODE_DATA, typename EDGE, typename EDGE_DATA>
	void BaseIterator<NODE, NODE_DATA, EDGE, EDGE_DATA>::add_child(std::shared_ptr<NODE> child, const EDGE_DATA& edge_data)
	{
		add_child(child, std::make_shared<EDGE_DATA>(edge_data));
	}

	#pragma region BasicEdge
	///<summary>
	/// コンストラクタ
	///</summary>
	template <typename NODE, typename NODE_DATA>
	BaseIterator<NODE, NODE_DATA, Graph::BasicEdge, nullptr_t>::BaseIterator(node_id id, std::shared_ptr<Collection::IdentifiableCollection<node_id, NODE>> node_collection)
		: BaseConstIterator<NODE, NODE_DATA, Graph::BasicEdge>(id, node_collection)
	{

	}

	///<summary>
	/// デストラクタ
	///</summary>
	template <typename NODE, typename NODE_DATA>
	BaseIterator<NODE, NODE_DATA, Graph::BasicEdge, nullptr_t>::~BaseIterator()
	{

	}

	///<summary>
	/// 参照外し
	///</summary>
	template <typename NODE, typename NODE_DATA>
	std::shared_ptr<NODE> BaseIterator<NODE, NODE_DATA, Graph::BasicEdge, nullptr_t>::operator*()
	{
		return node;
	}

	///<summary>
	/// 参照外し
	///</summary>
	template <typename NODE, typename NODE_DATA>
	std::shared_ptr<NODE> BaseIterator<NODE, NODE_DATA, Graph::BasicEdge, nullptr_t>::operator->()
	{
		return node;
	}

	///<summary>
	/// 条件に合う子要素を検索してイテレータを取得する
	/// 条件に合う要素がない場合はnullptrを指すイテレータを返します
	///</summary>
	template <typename NODE, typename NODE_DATA>
	BaseIterator<NODE, NODE_DATA, Graph::BasicEdge, nullptr_t> BaseIterator<NODE, NODE_DATA, Graph::BasicEdge, nullptr_t>::find_child_if(const std::function<bool(std::shared_ptr<NODE const>)>& compare)
	{
		BaseIterator<NODE, NODE_DATA, Graph::BasicEdge> ret(-1, nullptr);
		if (node == nullptr) return ret;
		node->for_each_edge([&](std::shared_ptr<Graph::BasicEdge const> edge) {
			node_id to = edge->get_to();
			std::shared_ptr<NODE const> child_node = node_collection->read_by_id(to);
			if (compare(child_node)) ret = BaseIterator<NODE, NODE_DATA, Graph::BasicEdge>(to, node_collection);
		});
		return ret;
	}


	///<summary>
	/// イテレータが指している要素の子要素に対して，childを追加する
	///</summary>
	template <typename NODE, typename NODE_DATA>
	void BaseIterator<NODE, NODE_DATA, Graph::BasicEdge, nullptr_t>::add_child(std::shared_ptr<NODE> child)
	{
		Graph::node_id child_id = child->get_id();
		if (!node_collection->contains(child_id)) {
			node_collection->add(child);
		}
		if (!node->is_connecting_to(child_id)) {
			node->connect_to(std::make_shared<Graph::BasicEdge>(child->get_id()));
		}
	}

	#pragma endregion BasicEdgeについての部分特殊化
}

