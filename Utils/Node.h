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
		std::shared_ptr<Graph::Edge<EDGE_DATA>> get_edge_to(node_id id) const;

	public:
		std::shared_ptr<NODE_DATA> data;
		
		Node(node_id id, NODE_DATA data);
		Node(const Node& node);
		virtual ~Node();

		bool connect_to(node_id to, EDGE_DATA data);
		bool disconnect_from(node_id from);

		std::shared_ptr<EDGE_DATA const> get_edge_data(node_id id) const;
		bool update_edge_data(node_id id, EDGE_DATA data);
		bool connect_or_update(node_id id, EDGE_DATA data);
		
		bool is_connecting_to(node_id id) const;
		std::list<node_id> get_connecting_node_list() const;

		bool operator==(Graph::Node<NODE_DATA, EDGE_DATA> node) const;
		bool operator!=(Graph::Node<NODE_DATA, EDGE_DATA> node) const;
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
/// コピーコンストラクタ
///</summary>
template <typename NODE_DATA, typename EDGE_DATA>
Graph::Node<NODE_DATA, EDGE_DATA>::Node(const Graph::Node<NODE_DATA, EDGE_DATA> &node) : Identifiable(node), data(node.data), edge_list(std::make_unique<std::list<std::shared_ptr<Graph::Edge<EDGE_DATA>>>>())
{
	for (std::list<std::shared_ptr<Graph::Edge<EDGE_DATA>>>::iterator iter = node.edge_list->begin(); iter != node.edge_list->end(); iter++) {
		std::shared_ptr<Graph::Edge<EDGE_DATA>> s_iter = *iter;
		std::shared_ptr<Graph::Edge<EDGE_DATA>> copy = std::make_shared<Graph::Edge<EDGE_DATA>>(s_iter->get_to(), *(s_iter->data));
		edge_list->push_back(copy);
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
/// 指定したノードへのエッジのポインタを取得する
/// 存在しない場合はnullptrを返す．
///</summary>
///<param name='id'>ノードID</param> 
template <typename NODE_DATA, typename EDGE_DATA>
std::shared_ptr<Graph::Edge<EDGE_DATA>> Graph::Node<NODE_DATA, EDGE_DATA>::get_edge_to(Graph::node_id id) const
{
	for (std::list<std::shared_ptr<Graph::Edge<EDGE_DATA>>>::iterator iter = edge_list->begin(); iter != edge_list->end(); iter++) {
		if (iter->get()->get_to() == id) {
			return *iter;
		}
	}
	return nullptr;
}

///<summary>
/// 指定したパラメータを持つエッジを接続エッジリストに追加します．
/// 既存のエッジの場合は追加せずにfalseを返します．
///</summary>
///<param name='to'>接続先ノード</param>
///<param name='data'>エッジが持つ属性値データ</param>
template <typename NODE_DATA, typename EDGE_DATA>
bool Graph::Node<NODE_DATA, EDGE_DATA>::connect_to(Graph::node_id to, EDGE_DATA data)
{

	std::shared_ptr<Graph::Edge<EDGE_DATA>> target = get_edge_to(to);
	if (target != nullptr) {
		return false;
	}

	std::shared_ptr<Graph::Edge<EDGE_DATA>> connect = std::make_shared<Graph::Edge<EDGE_DATA>>(to, data);
	edge_list->push_back(connect);
	return true;
}


///<summary>
/// 指定したIDのエッジを削除し，切断します
/// 削除が発生した場合true，発生しなかった場合falseを返す．
///</summary>
///<param name='from'>切断するノードのID</param>
template <typename NODE_DATA, typename EDGE_DATA>
bool Graph::Node<NODE_DATA, EDGE_DATA>::disconnect_from(Graph::node_id from)
{
	std::shared_ptr<Graph::Edge<EDGE_DATA>> target = get_edge_to(from);
	if (target != nullptr) {
		edge_list->remove(target);
		return true;
	}
	return false;
}


///<summary>
/// 指定したIDのデータを返します．
/// 対応するIDへのエッジがない場合はnullptrを返します．
///</summary>
template <typename NODE_DATA, typename EDGE_DATA>
std::shared_ptr<EDGE_DATA const> Graph::Node<NODE_DATA, EDGE_DATA>::get_edge_data(Graph::node_id id) const
{
	std::shared_ptr<Graph::Edge<EDGE_DATA>> target = get_edge_to(id);
	if (target == nullptr) {
		return nullptr;
	}
	return target->data;
}


///<summary>
/// 指定したIDのエッジのデータを更新します．
/// 対応するエッジがない等，更新がうまくいかなかった場合はfalseを返します．
///</summary>
template <typename NODE_DATA, typename EDGE_DATA>
bool Graph::Node<NODE_DATA, EDGE_DATA>::update_edge_data(Graph::node_id id, EDGE_DATA data)
{
	std::shared_ptr<Graph::Edge<EDGE_DATA>> target = get_edge_to(id);
	if (target == nullptr) {
		return false;
	}

	target->data = std::make_shared<EDGE_DATA>(data);
	return true;
}


///<summary>
/// 指定したIDを持つノードへのリンクを作成します．
/// 既存のリンクの場合はデータを更新します．
/// 新規作成の場合はtrue，更新の場合はfalseを返します．
///</summary>
template <typename NODE_DATA, typename EDGE_DATA>
bool Graph::Node<NODE_DATA, EDGE_DATA>::connect_or_update(Graph::node_id id, EDGE_DATA data)
{
	if (!connect_to(id, data)) {
		update_edge_data(id, data);
		return false;
	}
	return true;
}

///<summary>
/// 指定したノードと接続しているかどうかを判定する
///</summary>
template <typename NODE_DATA, typename EDGE_DATA>
bool Graph::Node<NODE_DATA, EDGE_DATA>::is_connecting_to(Graph::node_id link_to) const
{
	return get_edge_to(link_to) != nullptr;
}

///<summary>
/// 接続しているノードのIDリストを返す．
/// ノード番号は昇順にソート
///</summary>
template <typename NODE_DATA, typename EDGE_DATA>
std::list<Graph::node_id> Graph::Node<NODE_DATA, EDGE_DATA>::get_connecting_node_list() const
{
	std::list<Graph::node_id> connect_node_list;
	for (std::list<std::shared_ptr<Graph::Edge<EDGE_DATA>>>::iterator iter = edge_list->begin(); iter != edge_list->end(); iter++) {
		Graph::node_id id = (*iter)->get_to();
		connect_node_list.push_back(id);
	}
	connect_node_list.sort();
	return connect_node_list;
}

template <typename NODE_DATA, typename EDGE_DATA>
bool Graph::Node<NODE_DATA, EDGE_DATA>::operator==(Graph::Node<NODE_DATA, EDGE_DATA> node) const
{
	return id == node.id;
}

template <typename NODE_DATA, typename EDGE_DATA>
bool Graph::Node<NODE_DATA, EDGE_DATA>::operator!=(Graph::Node<NODE_DATA, EDGE_DATA> node) const
{
	return id != node.id;
}
