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
	template <typename NODE>
	class RoutingMethod
	{	
	private:
		double distance(const node_id& from, const node_id& to);

	protected:
		std::shared_ptr <const Collection::IdentifiableCollection<std::shared_ptr<NODE const>> node_collection;
		std::unique_ptr<std::vector<std::vector<double>>> distance_map;
		void create_distance_map();
	
	public:
		void set_node_collection(std::shared_ptr < const Collection::IdentifiableCollection<std::shared_ptr<NODE const>> node_collection);
		virtual std::unique_ptr<RoutingTable> create_routing_table() const = 0;
	};
}

#include "RoutingMethod.hpp"