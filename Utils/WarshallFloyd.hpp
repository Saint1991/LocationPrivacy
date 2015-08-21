#pragma once

namespace Graph 
{
	///<summary>
	/// コンストラクタ
	///</summary>
	template <typename NODE>
	WarshallFloyd<NODE>::WarshallFloyd()
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
	///</summary>
	template <typename NODE>
	std::unique_ptr<const std::vector<const std::vector<node_id>>> WarshallFloyd<NODE>::create_routing_table() const
	{
		if (node_collection == nullptr) return nullptr;
		std::unique_ptr<std::vector<std::vector<node_id>>> routing_table = std::make_unique<const std::vector<std::vector<node_id>>>();


		return std::move(routing_table);
	}
}
