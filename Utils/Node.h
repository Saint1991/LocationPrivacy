#ifdef UTILS_EXPORTS
#define NODE_API __declspec(dllexport)
#else
#define NODE_API __declspec(dllimport)
#endif

#pragma once
#include "Edge.h"
#include "Identifiable.h"

namespace Graph {


	///<summary>
	/// グラフにおけるノードを表すテンプレートクラス
	/// コンパイラの仕様上ここに実装も書くしかない...
	///</summary>
	template <typename NODE_DATA, typename EDGE_DATA>
	class Node : public Identifiable
	{
	protected:
		std::unique_ptr<std::list<std::shared_ptr<Edge<EDGE_DATA>>>> edge_list;
	public:
		std::shared_ptr<NODE_DATA> data;
		
		Node(node_id id, NODE_DATA data);
		Node(const Node& node);
		virtual ~Node();

		void connect_to(Graph::Edge<EDGE_DATA> link_to);
		void connect_to(node_id to, EDGE_DATA data);
		bool disconnect_from(node_id from);
		std::shared_ptr<Graph::Edge<EDGE_DATA>> get_edge_to(node_id id);
		bool is_connecting_to(node_id id);
		std::list<node_id> get_connecting_node_list();

		bool operator==(Graph::Node<NODE_DATA, EDGE_DATA> node);
		bool operator!=(Graph::Node<NODE_DATA, EDGE_DATA> node);
	};
}







///<summary>
/// コンストラクタ
///</summary>
///<param name='id'>ノードID</param>
///<param name='data'>ノードに持たせたいデータ</param>
template <typename NODE_DATA, typename EDGE_DATA>
Graph::Node<NODE_DATA, EDGE_DATA>::Node(Graph::node_id id, NODE_DATA data) : Identifiable(id), data(std::make_shared<NODE_DATA>(data)), edge_list(std::make_unique<std::list<std::shared_ptr<Graph::Edge<EDGE_DATA>>>>())
{
}

///<summary>
/// コピーコンストラクタ (不要かも) 
/// テストで要確認
///</summary>
template <typename NODE_DATA, typename EDGE_DATA>
Graph::Node<NODE_DATA, EDGE_DATA>::Node(const Graph::Node<NODE_DATA, EDGE_DATA> &node) : Identifiable(node), data(node.data), edge_list(std::make_unique<std::list<std::shared_ptr<Graph::Edge<EDGE_DATA>>>>())
{
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
		std::shared_ptr<Graph::Edge<EDGE_DATA>> connect = std::make_shared<Graph::Edge<EDGE_DATA>>(link_to);
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
	for (std::list<std::shared_ptr<Graph::Edge<EDGE_DATA>>>::iterator iter = edge_list->begin(); iter != edge_list->end(); iter++) {
		if (iter->get()->to == id) {
			return *iter;
		}
	}
	return nullptr;
}

///<summary>
/// 指定したノードと接続しているかどうかを判定する
///</summary>
template <typename NODE_DATA, typename EDGE_DATA>
bool Graph::Node<NODE_DATA, EDGE_DATA>::is_connecting_to(Graph::node_id link_to)
{
	return get_edge_to(link_to) != nullptr;
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
		Graph::node_id id = (*iter)->to;
		connect_node_list.push_back(id);
	}
	connect_node_list.sort();
	return connect_node_list;
}

template <typename NODE_DATA, typename EDGE_DATA>
bool Graph::Node<NODE_DATA, EDGE_DATA>::operator==(Graph::Node<NODE_DATA, EDGE_DATA> node)
{
	return id == node.id;
}

template <typename NODE_DATA, typename EDGE_DATA>
bool Graph::Node<NODE_DATA, EDGE_DATA>::operator!=(Graph::Node<NODE_DATA, EDGE_DATA> node)
{
	return id != node.id;
}
