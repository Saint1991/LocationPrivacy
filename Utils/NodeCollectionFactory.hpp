
namespace Graph 
{


	///<summary>
	/// コンストラクタ
	///</summary>
	template <typename NODE_DATA, typename EDGE>
	NodeCollectionFactory<NODE_DATA, EDGE>::NodeCollectionFactory() 
		: node_collection(std::make_shared<Collection::IdentifiableCollection<std::shared_ptr<Node<NODE_DATA, EDGE>>>>())
	{
	}


	///<summary>
	/// コピー禁止のため
	///</summary>
	template <typename NODE_DATA, typename EDGE>
	NodeCollectionFactory<NODE_DATA, EDGE>::NodeCollectionFactory(const NodeCollectionFactory& factory)
	{
	}


	///<summary>
	/// デストラクタ
	///</summary>
	template <typename NODE_DATA, typename EDGE>
	NodeCollectionFactory<NODE_DATA, EDGE>::~NodeCollectionFactory()
	{
	}

	///<summary>
	/// create_nodes, set_connectivitiesの実装通りにコレクションを構成し，
	/// 変更不可の状態にロックしたコレクションを取得する．
	///</summary>
	template <typename NODE_DATA, typename EDGE>
	std::shared_ptr<const Collection::IdentifiableCollection<std::shared_ptr<const Graph::Node<NODE_DATA, EDGE>>>> Graph::NodeCollectionFactory<NODE_DATA, EDGE>::create_static_node_collection() const
	{
		create_nodes();
		set_connectivities();
		return std::move(node_collection);
	}

	///<summary>
	/// create_nodes, set_connectivitiesの実装通りにコレクションを構成し，
	/// 更新可能な状態でコレクションを取得する．
	///</summary>
	template <typename NODE_DATA, typename EDGE>
	std::shared_ptr<Collection::IdentifiableCollection<std::shared_ptr<Graph::Node<NODE_DATA, EDGE>>>> Graph::NodeCollectionFactory<NODE_DATA, EDGE>::create_updateable_node_collection()
	{
		create_nodes();
		set_connectivities();
		return std::move(node_collection);
	}


	///<summary>
	/// 与えられた引数でノードを新規作成する．
	/// 既存IDのノードを作成しようとした場合はDuplicatedIdExceptionがスローされ，falseが返される．
	///</summary>
	template <typename NODE_DATA, typename EDGE>
	bool Graph::NodeCollectionFactory<NODE_DATA, EDGE>::create_node(Graph::node_id id, NODE_DATA data)
	{
		return node_collection->add(Graph::Node<NODE_DATA, EDGE>(id, data));
	}


	///<summary>
	/// Nodeを継承しているクラスのshared_ptrを追加する
	/// 既存IDのノードを作成しようとした場合はDuplicatedIdExceptionがスローされ，falseが返される．
	///</summary>
	template <typename NODE_DATA, typename EDGE>
	bool Graph::NodeCollectionFactory<NODE_DATA, EDGE>::add_node(std::shared_ptr<Graph::Node<NODE_DATA, EDGE>> node)
	{
		return node_collection->add(node);
	}

	///<summary>
	/// 指定したIDを持つノードを削除する
	///</summary>
	template <typename NODE_DATA, typename EDGE>
	bool Graph::NodeCollectionFactory<NODE_DATA, EDGE>::remove_node(Graph::node_id id)
	{
		return node_collection->remove_by_id(id);
	}


	///<summary>
	/// fromからtoへdataを属性に持つリンクを作成します．
	/// from, toいずれかのノードが存在しなかった場合や，既存のリンクを追加しようとした場合はfalseを返します．
	///</summary>
	template <typename NODE_DATA, typename EDGE>
	bool Graph::NodeCollectionFactory<NODE_DATA, EDGE>::connect(Graph::node_id from, Graph::node_id to, EDGE data)
	{
		std::shared_ptr<Graph::Node<NODE_DATA, EDGE> node = node_collection->get_by_id(from);
		bool is_to_exists = node_collection->contains(to);
		if (node == nullptr || !is_to_exists) {
			return false;
		}
		return node->connect_to(to, data);
	}


	///<summary>
	/// 指定したID間に相互にリンクを作成します．
	/// 双方向にリンクを作成できなければ，両方向切断されている状態に戻します．
	/// 双方向ともに成功した場合trueを，失敗した場合falseを返します．
	///</summary>
	template <typename NODE_DATA, typename EDGE>
	bool Graph::NodeCollectionFactory<NODE_DATA, EDGE>::connect_each_other(Graph::node_id id1, Graph::node_id id2, EDGE data)
	{
		std::shared_ptr < Graph::Node<NODE_DATA, EDGE> node1 = node_collection->get_by_id(from);
		std::shared_ptr < Graph::Node<NODE_DATA, EDGE> node2 = node_collection->get_by_id(to);
		if (node1 == nullptr || node2 == nullptr) {
			return false;
		}

		if (!node1->connect_to(id2, data)) {
			return false;
		}

		if (!node2->connect_to(id1, data)) {
			node1->disconnect_from(id2);
			return false;
		}
		return true;
	}



	///<summary>
	/// targetノードからfromノードへのリンクを切断します．
	/// 切断に成功した場合はtrueを，リンクやノードが見つからなかった場合はfalseを返します．
	///</summary>
	template <typename NODE_DATA, typename EDGE>
	bool Graph::NodeCollectionFactory<NODE_DATA, EDGE>::disconnect(Graph::node_id target, Graph::node_id from)
	{
		std::shared_ptr<Graph::Node<NODE_DATA, EDGE>> node = node_collection->get_by_id(target);
		if (target == nullptr) {
			return false;
		}

		return node->disconnect_from(from);
	}


	///<summary>
	/// 2つのノード間のリンクを双方向切断します．
	///</summary>
	template <typename NODE_DATA, typename EDGE>
	void Graph::NodeCollectionFactory<NODE_DATA, EDGE>::disconnect_each_other(Graph::node_id id1, Graph::node_id id2)
	{
		std::shared_ptr<Graph::Node<NODE_DATA, EDGE>> node1 = node_collection->get_by_id(id1);
		std::shared_ptr<Graph::Node<NODE_DATA, EDGE>> node2 = node_collection->get_by_id(id2);

		if (node1 != nullptr) {
			node1->disconnect_from(id2);
		}

		if (node2 != nullptr) {
			node2->disconnect_from(id1);
		}
	}
}

