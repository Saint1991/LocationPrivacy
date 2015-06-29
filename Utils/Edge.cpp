#include "stdafx.h"
#include "Edge.h"

///<summary>
/// コンストラクタ
///</summary>
///<param name='to'>接続先のノードID</param>
///<param name='data'>エッジの持つ属性値</param>
template <typename EDGE_DATA>
Graph::Edge<EDGE_DATA>::Edge(Graph::node_id to, EDGE_DATA data) : to(to), data(std::make_shared(data))
{
}


///<summary>
/// デストラクタ
///</summary>
template <typename EDGE_DATA>
Graph::Edge<EDGE_DATA>::~Edge()
{
}

template <typename EDGE_DATA>
bool operator==(Graph::Edge<EDGE_DATA> e1, Graph::Edge<EDGE_DATA> e2)
{
	return e1.to == e2.to;
}

template <typename EDGE_DATA>
bool operator!=(Graph::Edge<EDGE_DATA> e1, Graph::Edge<EDGE_DATA> e2)
{
	return !(e1 == e2)
}