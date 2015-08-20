#pragma once
#include "stdafx.h"
#include "Edge.h"
#include "Node.h"
#include "IdentifiableCollection.h"
namespace Graph 
{

	///<summary>
	/// ルーティングテーブル作成のアルゴリズム用インターフェース
	///</summary>
	template <typename NODE>
	class RoutingMethod
	{
	public:
		virtual std::unique_ptr<const std::vector<const std::vector<node_id>>> 
			create_routing_table(std::shared_ptr<const Collection::IdentifiableCollection<std::shared_ptr<const NODE>>> node_collection) const = 0;
	};
}
