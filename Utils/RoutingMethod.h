#pragma once
#include "stdafx.h"
#include "Edge.h"
#include "Node.h"
#include "IdentifiableCollection.h"

namespace Graph 
{

	//該当箇所が見つからない場合の値(create時にこれに該当するノードはエスケープする必要ありなので注意!)
	const node_id NOWHERE = ULONG_MAX;

	///<summary>
	/// ルーティングテーブル作成のアルゴリズム用インターフェース
	///</summary>
	template <typename NODE>
	class RoutingMethod
	{	
	public:
		virtual std::unique_ptr<const std::vector<const std::vector<node_id>>> 
			create_routing_table(std::shared_ptr<const Collection::IdentifiableCollection<std::shared_ptr<NODE const>>> node_collection) const = 0;
	};
}
