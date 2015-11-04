#pragma once
#include "stdafx.h"
#include "RoutingMethod.h"
#include "IdentifiableCollection.h"

namespace Graph
{

	///<summary>
	/// ワーシャルフロイド法によるルーティングテーブルの作成
	/// 使用後は各フィールドがnullptrになるので注意
	///</summary>
	template <typename NODE, typename EDGE>
	class WarshallFloyd : public RoutingMethod<NODE, EDGE> 
	{
	private:
		long node_count = -1L;
		std::unique_ptr<std::unordered_map<node_id, int>> conversion_map;
		std::unique_ptr<std::vector<node_id>> reverse_conversion_map;
		std::unique_ptr<std::vector<std::shared_ptr<std::vector<double>>>> distance_map;
		std::unique_ptr<std::vector<std::shared_ptr<std::vector<node_id>>>> routing_table;
		
		void initialize(std::shared_ptr<const Collection::IdentifiableCollection<Graph::node_id, NODE>> node_collection);
		void 	create_conversion_map(std::shared_ptr<const Collection::IdentifiableCollection<Graph::node_id, NODE>> node_collection);
		
	public:
		WarshallFloyd();
		virtual ~WarshallFloyd();

		std::unique_ptr<RoutingTable const> 
			create_routing_table(std::shared_ptr<const Collection::IdentifiableCollection<Graph::node_id, NODE>> node_collection);
	};
}

#include "WarshallFloyd.hpp"