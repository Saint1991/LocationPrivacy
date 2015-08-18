#ifdef UTILS_EXPORTS
#define MAP_API __declspec(dllexport)
#else
#define MAP_API __declspec(dllimport)
#endif

#pragma once
#include "NodeCollectionFactory.h"
#include "IdentifiableCollection.h"
#include "RoutingMethod.h"

namespace Geography 
{

	//該当箇所が見つからない場合の値(create時にこれに該当するノードはエスケープする必要ありなので注意!)
	const Graph::node_id NOWHERE = ULONG_MAX;

	///<summary>
	/// 地図に関する抽象クラス
	/// 具体的なマップはこれを継承して作る
	/// ID ULONG_MAXは予約済みで使えないので注意
	///</summary>
	/*template <typename GEONODEDATA, typename PATHDATA>
	class Map
	{
		
	protected:
		std::shared_ptr<const Collection::IdentifiableCollection<std::shared_ptr<const Geography::GeoNode<GEONODEDATA, PATHDATA>>>> node_collection;
		virtual void build_map() = 0;
		virtual void create_routing_table(std::unique_ptr<Graph::RoutingMethod<GEONODEDATA, PATHDATA>> routing_method);


		///<summary>
		/// 最短路を格納するルーティングテーブル
		/// SourceからDestinationへ行くのに次にどのノードに行く必要があるかを格納する．
		///</summary>
		std::unique_ptr<const std::vector<const std::vector<Graph::node_id>>> routing_table;

	public:
		Map(std::unique_ptr<Graph::RoutingMethod<GEONODEDATA, PATHDATA>> routing_method);
		virtual ~Map();

		const unsigned long calc_necessary_time(const Graph::node_id& from, const Graph::node_id& to, const double& avg_speed) const;
		const bool is_reachable(const Graph::node_id& from, const Graph::node_id& to, const double& avg_speed, const unsigned int& time_limit) const;
		const Graph::node_id get_next_node_of_shortest_path(const Graph::node_id& from, const Graph::node_id& to) const;
		const std::list<Graph::node_id> get_shortest_path(const Graph::node_id& from, const Graph::node_id& to) const;
		const std::list<Graph::node_id> get_connecting_nodes(const Graph::node_id& id) const;
		std::shared_ptr<const Geography::GeoNode<GEONODEDATA, PATHDATA>> get_static_node(const Graph::node_id& id) const;
		std::shared_ptr<Geography::GeoNode<GEONODEDATA, PATHDATA>> get_node(const Graph::node_id& id);
	};
	
	#include "Map.hpp"
	*/
}


