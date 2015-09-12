#pragma once
#include "NodeCollectionFactory.h"
#include "IdentifiableCollection.h"
#include "RoutingMethod.h"
#include "RoutingTable.h"

namespace Graph
{

	///<summary>
	/// 地図に関する抽象クラス
	/// 具体的なマップはこれを継承して作る
	/// NODE, PATHにはそれぞれNode, Edgeを継承したクラスが使える
	/// PATHにはBasicPathDataを継承しているデータを保持しているEdgeしか使えないので注意
	///</summary>
	template <typename NODE, typename POI, typename PATH>
	class Map
	{

	protected:
		std::shared_ptr<const Collection::IdentifiableCollection<Graph::node_id, NODE>> node_collection;
		std::shared_ptr <const Collection::IdentifiableCollection<Graph::node_id, POI>> poi_collection;
		virtual void build_map() = 0;

		///<summary>
		/// 最短路を格納するルーティングテーブル
		/// SourceからDestinationへ行くのに次にどのノードに行く必要があるかを格納する．
		/// Mapの構成はinitializeを呼び出した際に行われる
		///</summary>
		std::unique_ptr<RoutingTable const> routing_table;

	public:
		Map();
		virtual ~Map();

		void initialize(std::unique_ptr<RoutingMethod<NODE, PATH>> routing_method);

		node_id get_next_node_of_shortest_path(const node_id& from, const node_id& to) const;
		const std::list<node_id> get_shortest_path(const node_id& source, const node_id& destination) const;
		double calc_necessary_time(const node_id& from, const node_id& to, const double& avg_speed) const;
		bool is_reachable(const node_id& from, const node_id& to, const double& avg_speed, const double& time_limit) const;
		
		const std::list<node_id> get_connecting_nodes(const node_id& id) const;
		std::shared_ptr<NODE const> get_static_node(const node_id& id) const;
	};
}

#include "Map.hpp"
