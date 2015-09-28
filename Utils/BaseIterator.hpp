
namespace Graph
{

	///<summary>
	/// コンストラクタ
	///</summary>
	template <typename NODE, typename NODE_DATA, typename EDGE>
	BaseIterator<NODE, NODE_DATA, EDGE>::BaseIterator() : node(nullptr)
	{

	}


	///<summary>
	/// コンストラクタ
	///</summary>
	template <typename NODE, typename NODE_DATA, typename EDGE>
	BaseIterator<NODE, NODE_DATA, EDGE>::BaseIterator(std::shared_ptr<NODE> node) : node(node)
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
	/// 条件に合う子要素を検索してイテレータを取得する
	/// 条件に合う要素がない場合はnullptrを指すイテレータを返します
	///</summary>
	template <typename NODE, typename NODE_DATA, typename EDGE>
	BaseIterator<NODE, NODE_DATA, EDGE> BaseIterator<NODE, NODE_DATA, EDGE>::find_child_if(const std::function<bool(std::shared_ptr<NODE const>)>& compare)
	{
		node->for_each_edge([&](std::shared_ptr<EDGE const> edge) {
			node_id to = edge->get_to();
			std::shared_ptr<NODE> node = node_collection->get_by_id(to);
			if (compare(node)) return base_iterator(node);
		});
		return base_iterator(nullptr);
	}

	
	///<summary>
	/// 比較演算子
	///</summary>
	template <typename NODE, typename NODE_DATA, typename EDGE>
	bool BaseIterator<NODE, NODE_DATA, EDGE>::operator==(const BaseIterator<NODE, NODE_DATA, EDGE>& iter) const
	{
		return **iter == *node;
	}
	

	///<summary>
	/// 比較演算子
	///</summary>
	template <typename NODE, typename NODE_DATA, typename EDGE>
	bool BaseIterator<NODE, NODE_DATA, EDGE>::operator!=(const BaseIterator<NODE, NODE_DATA, EDGE>& iter) const
	{
		return **iter != *node;
	}
	
}

