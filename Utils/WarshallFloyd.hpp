#pragma once

namespace Graph 
{
	///<summary>
	/// コンストラクタ
	///</summary>
	template <typename NODE>
	WarshallFloyd<NODE>::WarshallFloyd() : RoutingMethod()
	{
	}


	///<summary>
	/// デストラクタ
	///</summary>
	template <typename NODE>
	WarshallFloyd<NODE>::~WarshallFloyd()
	{
	}


	///<summary>
	/// ワーシャルフロイド法によるルーティングテーブルの作成
	/// 参照 (http://dai1741.github.io/maximum-algo-2012/docs/shortest-path/)
	/// nodesが正しく渡されていないとnullptrが返る
	///</summary>
	template <typename NODE>
	std::unique_ptr<const std::vector<const std::vector<node_id>>> 
		WarshallFloyd<NODE>::create_routing_table(std::shared_ptr<const Collection::IdentifiableCollection<std::shared_ptr<NODE const>>> nodes) const
	{

		//フィールドにnode_collectionへの参照を持たせる (距離計算 distanceメソッドのため)
		if (nodes == nullptr) return nullptr;
		node_collection = nodes;

		//ノード数の取得
		const node_id NODE_COUNT = node_collection->size();

		//全値を最初はNOWHEREで初期化
		std::unique_ptr<std::vector<std::vector<node_id>>> routing_table = 
			std::make_unique<std::vector<std::vector<node_id>>>(NODE_COUNT, std::vector<node_id>(NODE_COUNT, NOWHERE));


		


		return std::move(routing_table);
	}
}
