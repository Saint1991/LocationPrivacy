#pragma once
#include "stdafx.h"
#include "Edge.h"
#include "Node.h"
#include "IdentifiableCollection.h"
#include "RoutingTable.h"

namespace Graph 
{

	///<summary>
	/// ルーティングテーブル作成のアルゴリズム用インターフェース
	/// NODEにはBasicPathDataの派生クラスをエッジに持つNodeクラスを継承したクラスしか持てないので注意
	///</summary>
	template <typename NODE, typename EDGE>
	class RoutingMethod
	{		
	public:
		virtual std::unique_ptr<RoutingTable const> 
			create_routing_table(std::shared_ptr<const Collection::IdentifiableCollection<Graph::node_id, NODE>> node_collection) = 0;
	};
}