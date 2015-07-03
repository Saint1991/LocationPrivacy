#include "stdafx.h"
#include "NodeManager.h"


///<summary>
/// コンストラクタ
///</summary>
template <typename NODE_DATA, typename EDGE_DATA>
Graph::NodeManager<NODE_DATA, EDGE_DATA>::NodeManager() : node_collection(std::make_unique<Collection::IdentifiableCollection>())
{
	node_collection->clear();
}

///<summary>
/// デストラクタ
///</summary>
template <typename NODE_DATA, typename EDGE_DATA>
Graph::NodeManager<NODE_DATA, EDGE_DATA>::~NodeManager()
{
	node_collection->clear();
}

///<summary>
/// 指定したIDのノードを返す．
/// 見つからない場合はnullptrを返す．
///</summary>
/// <param name='id'>取得したいノードのID</param>
template <typename NODE_DATA, typename EDGE_DATA>
std::shared_ptr<Graph::Node<NODE_DATA, EDGE_DATA>> Graph::NodeManager<NODE_DATA, EDGE_DATA>::get_node(Graph::node_id id)
{
	//要dynamic_cast?
	return node_collection->find_by_id(id);
}

///<summary>
/// ノードを新規作成し，作成したノードのIDを返します．
///</summary>
template <typename NODE_DATA, typename EDGE_DATA>
Graph::node_id Graph::NodeManager<NODE_DATA, EDGE_DATA>::create_node(NODE_DATA data)
{
	std::size_t new_id = node_collection->size();
	std::shared_ptr<Graph::Node<NODE_DATA, EDGE_DATA>> node = std::make_shared<Graph::Node<NODE_DATA, EDGE_DATA>>(new_id, data);
	node_collection->push_back(node);
}

///<summary>
/// fromからtoに指定したデータを属性としてもつリンクを張る
///</summary>
template <typename NODE_DATA, typename EDGE_DATA>
bool Graph::NodeManager<NODE_DATA, EDGE_DATA>::connect_to(Graph::node_id from, Graph::node_id to, EDGE_DATA data)
{
	std::shared_ptr<Graph::Node<NODE_DATA, EDGE_DATA>> from_node = get_node(from);
	std::shared_ptr<Graph::Node<NODE_DATA, EDGE_DATA>> to_node = get_node(to);
	if (from_node != nullptr && to_node != nullptr) {
		from_node->connect_to(to, data);
		return true;
	}
	return false;
}

///<summary>
/// id1とid2を相互にリンクする．
/// id1からid2へはdata1，id2からid1へはdata2の属性値を持ったリンクが張られる
///</summary>
template <typename NODE_DATA, typename EDGE_DATA>
bool Graph::NodeManager<NODE_DATA, EDGE_DATA>::connect_each_other(Graph::node_id id1, Graph::node_id id2, EDGE_DATA data1, EDGE_DATA data2)
{
	std::shared_ptr<Graph::Node<NODE_DATA, EDGE_DATA>> node1 = get_node(id1);
	std::shared_ptr<Graph::Node<NODE_DATA, EDGE_DATA>> node2 = get_node(id2);
	if (from_node != nullptr && to_node != nullptr) {
		from_node->connect_to(id2, data1);
		to_node->connect_to(id1, data2);
		return true;
	}
	return false;
}


///<summary>
/// targetがfromに対してもつリンクを削除する．
/// 削除が発生した場合はtrueを，リンクが見つからない場合はfalseを返す．
///</summary>
template <typename NODE_DATA, typename EDGE_DATA>
void Graph::NodeManager<NODE_DATA, EDGE_DATA>::disconnect_from(Graph::node_id target, Graph::node_id from)
{
	std::shared_ptr <Graph::Node<NODE_DATA, EDGE_DATA>> target_node = get_node(target);
	if (target_node != nullptr) {
		target_node->disconnect_from(from);
	}
}

///<summary>
/// 相互にリンクを切断する．
/// 両方向ともにリンクが存在し，切断した場合はtrue，それ以外はfalseを返す．
///</summary>
template <typename NODE_DATA, typename EDGE_DATA>
void Graph::NodeManager<NODE_DATA, EDGE_DATA>::disconnect_each_other(Graph::node_id id1, Graph::node_id id2)
{
	std::shared_ptr<Graph::Node<NODE_DATA, EDGE_DATA>> node1 = get_node(id1);
	if (node1 != nullptr) {
		node1->disconnect_from(id2);
	}

	std::shared_ptr< Graph::Node<NODE_DATA, EDGE_DATA>> node2 = get_node(id2);
	if (node2 != nullptr) {
		node2->disconnect_from(id1);
	}
}

///<summary>
/// ノードコレクションをソートする．
/// ソートするとノードにアクセスする際の探索の効率が上がる．
///</summary>
template <typename NODE_DATA, typename EDGE_DATA>
void Graph::NodeManager<NODE_DATA, EDGE_DATA>::sort_node_collection()
{
	node_collection->sort();
}