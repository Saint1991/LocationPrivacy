
namespace Graph
{

	///<summary>
	/// コンストラクタ
	///</summary>
	template <typename NODE, typename NODE_DATA, typename EDGE>
	BaseConstIterator<NODE, NODE_DATA, EDGE>::BaseConstIterator(node_id id, std::shared_ptr<Collection::IdentifiableCollection<node_id, NODE>> node_collection) 
		: node_collection(node_collection)
	{
		if (node_collection != nullptr) node = node_collection->get_by_id(id);
	}


	///<summary>
	/// デストラクタ
	///</summary>
	template <typename NODE, typename NODE_DATA, typename EDGE>
	BaseConstIterator<NODE, NODE_DATA, EDGE>::~BaseConstIterator()
	{

	}

	///<summary>
	/// 参照外し
	///</summary>
	template <typename NODE, typename NODE_DATA, typename EDGE>
	std::shared_ptr<NODE const> BaseConstIterator<NODE, NODE_DATA, EDGE>::operator*() const
	{
		return node;
	}


	///<summary>
	/// 参照外し
	///</summary>
	template <typename NODE, typename NODE_DATA, typename EDGE>
	std::shared_ptr<NODE const> BaseConstIterator<NODE, NODE_DATA, EDGE>::operator->() const
	{
		return node;
	}

	///<summary>
	/// 条件に合う子要素を検索してイテレータを取得する
	/// 条件に合う要素がない場合はnullptrを指すイテレータを返します
	///</summary>
	template <typename NODE, typename NODE_DATA, typename EDGE>
	BaseConstIterator<NODE, NODE_DATA, EDGE> BaseConstIterator<NODE, NODE_DATA, EDGE>::find_child_if(const std::function<bool(std::shared_ptr<NODE const>)>& compare) const
	{
		BaseConstIterator<NODE, NODE_DATA, EDGE> ret(-1, nullptr);
		if (node == nullptr) return ret;
		node->for_each_edge([&](std::shared_ptr<EDGE const> edge) {
			node_id to = edge->get_to();
			std::shared_ptr<NODE const> node = node_collection->read_by_id(to);
			if (compare(node)) ret = BaseConstIterator<NODE, NODE_DATA, EDGE>(to, node_collection);
		});
		return ret;
	}

	///<summary>
	/// 比較演算子
	///</summary>
	template <typename NODE, typename NODE_DATA, typename EDGE>
	bool BaseConstIterator<NODE, NODE_DATA, EDGE>::operator==(const BaseConstIterator<NODE, NODE_DATA, EDGE>& iter) const
	{
		if (*iter == nullptr) return false;
		return **iter == *node;
	}

	///<summary>
	/// 比較演算子
	///</summary>
	template <typename NODE, typename NODE_DATA, typename EDGE>
	bool BaseConstIterator<NODE, NODE_DATA, EDGE>::operator!=(const BaseConstIterator<NODE, NODE_DATA, EDGE>& iter) const
	{
		if (*iter == nullptr) return false;
		return **iter != *node;
	}

}
