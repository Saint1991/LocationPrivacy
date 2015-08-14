#pragma once
#include "stdafx.h"
#include "Edge.h"

namespace Graph 
{

	///<summary>
	/// ルーティングテーブル作成のアルゴリズム用インターフェース
	///</summary>
	template <typename NODE_DATA, typename EDGE_DATA>
	class RoutingMethod
	{
	public:
		virtual std::unique_ptr<const std::vector<std::vector<Graph::node_id>>> create_routing_table(std::shared_ptr<const Collection::IdentifiableCollection<std::shared_ptr<const Node<NODE_DATA, EDGE_DATA>>>> node_collection) const = 0;
	};
}
