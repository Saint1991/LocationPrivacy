#include "stdafx.h"
#include "Node.h"

///<summary>
/// コンストラクタ
///</summary>
///<param name='id'>ノードID</param>
///<param name='data'>ノードに持たせたいデータ</param>
template <typename NODE_DATA, typename EDGE_DATA>
Graph::Node<NODE_DATA, EDGE_DATA>::Node(Graph::node_id id, NODE_DATA data) : id(id), data(std::make_shared<NODE_DATA>(data)), edge_list(std::make_unique<std::list<std::shared_ptr<Graph::Edge<EDGE_DATA>>>>())
{
}

///<summary>
/// コピーコンストラクタ (不要かも) 
/// テストで要確認
///</summary>
template <typename NODE_DATA, typename EDGE_DATA>
Graph::Node<NODE_DATA, EDGE_DATA>::Node(const Graph::Node<NODE_DATA, EDGE_DATA> &node) : id(node->id)
{
	edge_list = std::make_unique < std::list<std::shared_ptr<Graph::Edge<EDGE_DATA>>>();
	for (std::list<std::shared_ptr<Graph::Edge<EDGE_DATA>>>::iterator iter = node.edge_list->begin(); iter != node.edge_list->end(); iter++) {
		std::shared_ptr<Graph::Edge<EDGE_DATA>> s_iter = *iter;
		std::shared_ptr<Graph::Edge<EDGE_DATA>> copy = std::make_shared<Graph::Edge<EDGE_DATA>>(s_iter->to, s_iter->data);
		edge_list->push_back(s_iter);
	}
}

///<summary>
/// デストラクタ
///</summary>
template <typename NODE_DATA, typename EDGE_DATA>
Graph::Node<NODE_DATA, EDGE_DATA>::~Node()
{
}

///<summary>
/// 指定したエッジを接続エッジリストに追加します．
/// 既存のリンクの場合は距離を更新します
///</summary>
///<param name='link_to'>接続リストに追加するノード</param>
template <typename NODE_DATA, typename EDGE_DATA>
void Graph::Node<NODE_DATA, EDGE_DATA>::connect_to(Graph::Edge<EDGE_DATA> link_to)
{
	std::shared_ptr<Graph::Edge<EDGE_DATA>> target = this->get_edge_to(link_to.to);
	if (target == nullptr) {
		std::shared_ptr<Graph::Edge<EDGE_DATA>> connect = std::make_shared<Graph::Edge>(link_to);
		edge_list->push_back(connect);
	}
}

///<summary>
/// 指定したパラメータを持つエッジを接続エッジリストに追加します．
/// 既存のリンクの場合は距離を更新します
///</summary>
///<param name='to'>接続先ノード</param>
///<param name='data'>エッジが持つ属性値データ</param>
template <typename NODE_DATA, typename EDGE_DATA>
void Graph::Node<NODE_DATA, EDGE_DATA>::connect_to(Graph::node_id to, EDGE_DATA data)
{
	Graph::Edge<EDGE_DATA> link_to(to, data);
	connect_to(link_to);
}

///<summary>
/// 指定したIDのエッジを削除し，切断します
/// 削除が発生した場合true，発生しなかった場合falseを返す．
///</summary>
///<param name='from'>切断するノードのID</param>
template <typename NODE_DATA, typename EDGE_DATA>
bool Graph::Node<NODE_DATA, EDGE_DATA>::disconnect_from(Graph::node_id from)
{
	std::shared_ptr<Graph::Edge<EDGE_DATA>> target = this->get_edge_to(from);
	if (target != nullptr) {
		this->edge_list->remove(target);
		return true;
	}
	return false;
}

///<summary>
/// 指定したノードへのエッジのポインタを取得する
/// 存在しない場合はnullptrを返す．
///</summary>
///<param name='id'>ノードID</param> 
template <typename NODE_DATA, typename EDGE_DATA>
std::shared_ptr<Graph::Edge<EDGE_DATA>> Graph::Node<NODE_DATA, EDGE_DATA>::get_edge_to(Graph::node_id id)
{
	std::shared_ptr <Graph::Edge<EDGE_DATA> ret = nullptr;
	for (std::list<std::shared_ptr<Graph::Edge<EDGE_DATA>>>::iterator iter = edge_list->begin(); iter != edge_list->end(); iter++) {
		if (iter->get()->to == id) {
			ret.reset(iter->get());
		}
	}
	return ret;
}

///<summary>
/// 指定したノードと接続しているかどうかを判定する
///</summary>
template <typename NODE_DATA, typename EDGE_DATA>
bool Graph::Node<NODE_DATA, EDGE_DATA>::is_connecting_to(Graph::node_id link_to)
{
	return get_edge_to(id) != nullptr;
}

///<summary>
/// 接続しているノードのIDリストを返す．
/// ノード番号は昇順にソート
///</summary>
template <typename NODE_DATA, typename EDGE_DATA>
std::list<Graph::node_id> Graph::Node<NODE_DATA, EDGE_DATA>::get_connecting_node_list()
{
	std::list<Graph::node_id> connect_node_list;
	for (std::list<std::shared_ptr<Graph::Edge<EDGE_DATA>>>::iterator iter = edge_list->begin(); iter != edge_list->end(); iter++) {
		connect_node_list.sort();
		return connect_node_list;
	}
}