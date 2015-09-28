
namespace Graph
{

	///<summary>
	/// コンストラクタ
	///</summary>
	template <typename NODE, typename NODE_DATA, typename EDGE>
	PrefixTree<NODE, NODE_DATA, EDGE>::BaseConstIterator::BaseConstIterator() : node(root)
	{

	}


	///<summary>
	/// コンストラクタ
	///</summary>
	template <typename NODE, typename NODE_DATA, typename EDGE>
	PrefixTree<NODE, NODE_DATA, EDGE>::BaseConstIterator::BaseConstIterator(std::shared_ptr<NODE const> node) : node(node)
	{

	}


	///<summary>
	/// デストラクタ
	///</summary>
	template <typename NODE, typename NODE_DATA, typename EDGE>
	PrefixTree<NODE, NODE_DATA, EDGE>::BaseConstIterator::~BaseConstIterator()
	{

	}

	///<summary>
	/// 参照外し
	///</summary>
	template <typename NODE, typename NODE_DATA, typename EDGE>
	std::shared_ptr<NODE const> PrefixTree<NODE, NODE_DATA, EDGE>::BaseConstIterator::operator*() const
	{
		return node;
	}

	///<summary>
	/// 条件に合う子要素を検索してイテレータを取得する
	/// 条件に合う要素がない場合はnullptrを指すイテレータを返します
	///</summary>
	template <typename NODE, typename NODE_DATA, typename EDGE>
	PrefixTree<NODE, NODE_DATA, EDGE>::BaseConstIterator PrefixTree<NODE, NODE_DATA, EDGE>::BaseConstIterator::find_child_if(const std::function<bool(std::shared_ptr<NODE const>)>& compare) const
	{
		node->for_each_edge([&](std::shared_ptr<EDGE const> edge) {
			node_id to = edge->get_to();
			std::shared_ptr<NODE const> node = node_collection->read_by_id(to);
			if (compare(node)) return PrefixTree<NODE, NODE_DATA, EDGE>::BaseIterator(node);
		});
		return PrefixTree<NODE, NODE_DATA, EDGE>::BaseIterator(nullptr);
	}

	///<summary>
	/// 比較演算子
	///</summary>
	template <typename NODE, typename NODE_DATA, typename EDGE>
	bool PrefixTree<NODE, NODE_DATA, EDGE>::BaseConstIterator::operator==(const PrefixTree<NODE, NODE_DATA, EDGE>::BaseConstIterator& iter) const
	{
		return **iter == *node;
	}

	///<summary>
	/// 比較演算子
	///</summary>
	template <typename NODE, typename NODE_DATA, typename EDGE>
	bool PrefixTree<NODE, NODE_DATA, EDGE>::BaseConstIterator::operator!=(const PrefixTree<NODE, NODE_DATA, EDGE>::BaseConstIterator& iter) const
	{
		return **iter != *node;
	}

}
