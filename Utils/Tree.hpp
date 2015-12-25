
namespace Graph
{


	///<summary>
	/// コンストラクタ
	///</summary>
	template <typename NODE, typename NODE_DATA, typename EDGE>
	Tree<NODE, NODE_DATA, EDGE>::Tree() : node_collection(std::make_shared<Collection::IdentifiableCollection<node_id, NODE>>())
	{

	}

	///<summary>
	///  コピーコンストラクタ
	///</summary>
	template <typename NODE, typename NODE_DATA, typename EDGE>
	Tree<NODE, NODE_DATA, EDGE>::Tree(const Tree& t) : node_collection(std::make_shared<Collection::IdentifiableCollection<node_id, NODE>>())
	{
		t.node_collection->foreach([&](std::shared_ptr<NODE const> node) {
			node_collection->add(std::make_shared<NODE>(*node));
		});
		root_node = std::make_shared<NODE>(*t.root_node);
	}


	///<summary>
	/// デストラクタ
	///</summary>
	template <typename NODE, typename NODE_DATA, typename EDGE>
	Tree<NODE, NODE_DATA, EDGE>::~Tree()
	{

	}

	
	///<summary>
	/// ルートノードを設定して木を初期化します
	///</summary>
	template <typename NODE, typename NODE_DATA, typename EDGE>
	void Tree<NODE, NODE_DATA, EDGE>::initialize(std::shared_ptr<NODE> root)
	{
		node_collection->clear();
		root_node = root;
		node_collection->add(root);
	}
	

	///<summary>
	/// rootノードを指している状態でイテレータを取得する
	///</summary>
	template <typename NODE, typename NODE_DATA, typename EDGE>
	template <typename ITER_TYPE>
	ITER_TYPE Tree<NODE, NODE_DATA, EDGE>::root()
	{
		ITER_TYPE ret(root_node->get_id(), node_collection);
		return ret;
	}

	///<summary>
	/// rootノードを指している状態でイテレータを取得する
	///</summary>
	template <typename NODE, typename NODE_DATA, typename EDGE>
	template <typename CONST_ITER_TYPE>
	CONST_ITER_TYPE Tree<NODE, NODE_DATA, EDGE>::const_root() const
	{
		CONST_ITER_TYPE ret(root_node->get_id(), node_collection);
		return ret;
	}

	///<summary>
	/// 指定したIDを持つノードを指して状態でイテレータを取得する
	///</summary>
	template <typename NODE, typename NODE_DATA, typename EDGE>
	template <typename ITER_TYPE>
	ITER_TYPE Tree<NODE, NODE_DATA, EDGE>::get_iter_by_id(node_id id)
	{
		ITER_TYPE ret(id, node_collection);
		return ret;
	}
}