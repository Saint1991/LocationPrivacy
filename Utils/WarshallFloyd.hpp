#pragma once


///<summary>
/// コンストラクタ
///</summary>
template <typename NODE_DATA, typename EDGE>
Graph::WarshallFloyd<NODE_DATA, EDGE>::WarshallFloyd()
{
}


///<summary>
/// デストラクタ
///</summary>
template <typename NODE_DATA, typename EDGE>
Graph::WarshallFloyd<NODE_DATA, EDGE>::~WarshallFloyd()
{
}


///<summary>
/// ワーシャルフロイド法によるルーティングテーブルの作成
/// 参照 (http://dai1741.github.io/maximum-algo-2012/docs/shortest-path/)
///</summary>
template <typename NODE_DATA, typename EDGE>
std::unique_ptr<const std::vector<std::vector<Graph::node_id>>> 
Graph::WarshallFloyd<NODE_DATA, EDGE>::create_routing_table(std::shared_ptr<const Collection::IdentifiableCollection<std::shared_ptr<const Node<NODE_DATA, EDGE>>>> node_collection) const
{
	if (node_collection == nullptr) return nullptr;
	std::unique_ptr<const std::vector<std::vector<Graph::node_id>>> routing_table = std::make_unique<const std::vector<std::vector<Graph::node_id>>>();
}
