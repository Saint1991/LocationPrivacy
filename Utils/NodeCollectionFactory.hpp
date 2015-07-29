
///<summary>
/// create_nodes, set_connectivitiesの実装通りにコレクションを構成し，
/// 変更不可の状態にロックしたコレクションを取得する．
///</summary>
template <typename NODE_DATA, typename EDGE_DATA>
std::unique_ptr<const Collection::IdentifiableCollection<std::shared_ptr<Graph::Node<NODE_DATA, EDGE_DATA>>>> Graph::NodeCollectionFactory<NODE_DATA, EDGE_DATA>::create_static_node_collection() const
{
	std::cout << "Start Creating Node Collection." << std::endl;

	create_nodes();
	set_connectivities();
	std::unique_ptr<const Collection::IdentifiableCollection<std::shared_ptr<Graph::Node<NODE_DATA, EDGE_DATA>>>> temp = std::move(node_collection);
	node_collection = std::make_unique<Collection::IdentifiableCollection<std::shared_ptr<Graph::Node<NODE_DATA, EDGE_DATA>>>();

	std::cout << "Finish Creating Node Collection." << std::endl;
	return std::move(temp);
}

///<summary>
/// create_nodes, set_connectivitiesの実装通りにコレクションを構成し，
/// 更新可能な状態でコレクションを取得する．
///</summary>
template <typename NODE_DATA, typename EDGE_DATA>
std::unique_ptr<Collection::IdentifiableCollection<std::shared_ptr<Graph::Node<NODE_DATA, EDGE_DATA>>>> Graph::NodeCollectionFactory<NODE_DATA, EDGE_DATA>::create_updateable_node_collection()
{
	std::cout << "Start Creating Node Collection." << std::endl;

	create_nodes();
	set_connectivities();
	std::unique_ptr<Collection::IdentifiableCollection<std::shared_ptr<Graph::Node<NODE_DATA, EDGE_DATA>>>> temp = std::move(node_collection);
	node_collection = std::make_unique<Collection::IdentifiableCollection<std::shared_ptr<Graph::Node<NODE_DATA, EDGE_DATA>>>();

	std::cout << "Finish Creating Node Collection." << std::endl;
	return std::move(temp);
}


///<summary>
/// 与えられた引数でノードを新規作成する．
/// 既存IDのノードを作成しようとした場合はDuplicatedIdExceptionがスローされ，falseが返される．
///</summary>
template <typename NODE_DATA, typename EDGE_DATA>
bool Graph::NodeCollectionFactory<NODE_DATA, EDGE_DATA>::create_node(Graph::node_id id, NODE_DATA data)
{
	return node_collection->add(Graph::Node<NODE_DATA, EDGE_DATA>(id, data));
}


///<summary>
/// 指定したIDを持つノードを削除する
///</summary>
template <typename NODE_DATA, typename EDGE_DATA>
bool Graph::NodeCollectionFactory<NODE_DATA, EDGE_DATA>::remove_node(Graph::node_id id)
{
	return node_collection->remove_by_id(id);
}


///<summary>
/// fromからtoへdataを属性に持つリンクを作成します．
/// from, toいずれかのノードが存在しなかった場合や，既存のリンクを追加しようとした場合はfalseを返します．
///</summary>
template <typename NODE_DATA, typename EDGE_DATA>
bool Graph::NodeCollectionFactory<NODE_DATA, EDGE_DATA>::connect(Graph::node_id from, Graph::node_id to, EDGE_DATA data)
{
	std::shared_ptr<Graph::Node<NODE_DATA, EDGE_DATA> node = node_collection->get_by_id(from);
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
template <typename NODE_DATA, typename EDGE_DATA>
bool Graph::NodeCollectionFactory<NODE_DATA, EDGE_DATA>::connect_each_other(Graph::node_id id1, Graph::node_id id2, EDGE_DATA data)
{
	std::shared_ptr < Graph::Node<NODE_DATA, EDGE_DATA> node1 = node_collection->get_by_id(from);
	std::shared_ptr < Graph::Node<NODE_DATA, EDGE_DATA> node2 = node_collection->get_by_id(to);
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
template <typename NODE_DATA, typename EDGE_DATA>
bool Graph::NodeCollectionFactory<NODE_DATA, EDGE_DATA>::disconnect(Graph::node_id target, Graph::node_id from)
{
	std::shared_ptr<Graph::Node<NODE_DATA, EDGE_DATA>> node = node_collection->get_by_id(target);
	if (target == nullptr) {
		return false;
	}

	return node->disconnect_from(from);
}


///<summary>
/// 2つのノード間のリンクを双方向切断します．
///</summary>
template <typename NODE_DATA, typename EDGE_DATA>
void Graph::NodeCollectionFactory<NODE_DATA, EDGE_DATA>::disconnect_each_other(Graph::node_id id1, Graph::node_id id2)
{
	std::shared_ptr<Graph::Node<NODE_DATA, EDGE_DATA>> node1 = node_collection->get_by_id(id1);
	std::shared_ptr<Graph::Node<NODE_DATA, EDGE_DATA>> node2 = node_collection->get_by_id(id2);

	if (node1 != nullptr) {
		node1->disconnect_from(id2);
	}

	if (node2 != nullptr) {
		node2->disconnect_from(id1);
	}
}
