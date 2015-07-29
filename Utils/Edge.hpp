
///<summary>
/// コンストラクタ
///</summary>
///<param name='to'>接続先のノードID</param>
///<param name='data'>エッジの持つ属性値</param>
template <typename EDGE_DATA>
Graph::Edge<EDGE_DATA>::Edge(Graph::node_id to, EDGE_DATA data) : to(to), data(std::make_shared<EDGE_DATA>(data))
{
}


///<summary>
/// デストラクタ
///</summary>
template <typename EDGE_DATA>
Graph::Edge<EDGE_DATA>::~Edge()
{
}


///<summary>
/// リンク先のIDを取得する．
///</summary>
template <typename EDGE_DATA>
Graph::node_id Graph::Edge<EDGE_DATA>::get_to() const
{
	return to;
}

template <typename EDGE_DATA>
bool Graph::Edge<EDGE_DATA>::operator==(Graph::Edge<EDGE_DATA> e) const
{
	return to == e.to;
}

template <typename EDGE_DATA>
bool Graph::Edge<EDGE_DATA>::operator!=(Graph::Edge<EDGE_DATA> e) const
{
	return to != e.to;
}