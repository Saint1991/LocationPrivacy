#include "stdafx.h"
#include "Node.h"

///<summary>
/// コンストラクタ
///</summary>
///<param name='id'>ノードID</param>
///<param name='data'>ノードに持たせたいデータ</param>
template <typename NODE_DATA, typename EDGE_DATA>
Graph::Node<NODE_DATA, EDGE_DATA>::Node(Graph::node_id id, NODE_DATA data) : id(id), data(std::make_shared<NODE_DATA>(data))
{
}

///<summary>
/// デストラクタ
///</summary>
template <typename NODE_DATA, typename EDGE_DATA>
Graph::Node<NODE_DATA, EDGE_DATA>::~Node()
{
}

template <typename NODE_DATA, typename EDGE_DATA>
void Graph::Node<NODE_DATA, EDGE_DATA>::connect_to(Graph::Edge<EDGE_DATA> link_to)
{

}

template <typename NODE_DATA, typename EDGE_DATA>
void Graph::Node<NODE_DATA, EDGE_DATA>::connect_to(Graph::node_id to, EDGE_DATA data)
{

}

template <typename NODE_DATA, typename EDGE_DATA>
bool Graph::Node<NODE_DATA, EDGE_DATA>::disconnect_from(Graph::node_id from)
{

}

template <typename NODE_DATA, typename EDGE_DATA>
std::shared_ptr<Graph::Edge<EDGE_DATA>> Graph::Node<NODE_DATA, EDGE_DATA>::get_edge_to(Graph::node_id id)
{

}

template <typename NODE_DATA, typename EDGE_DATA>
bool Graph::Node<NODE_DATA, EDGE_DATA>::is_connecting_to(Graph::node_id link_to)
{

}

template <typename NODE_DATA, typename EDGE_DATA>
std::list<Graph::node_id> Graph::Node<NODE_DATA, EDGE_DATA>::get_connecting_node_list()
{

}