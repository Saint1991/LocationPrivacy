#ifdef UTILS_EXPORTS
#define MAP_API __declspec(dllexport)
#else
#define MAP_API __declspec(dllimport)
#endif

#pragma once
#include "NodeCollectionFactory.h"
#include "IdentifiableCollection.h"
#include "RoutingMethod.h"

namespace Graph
{

	///<summary>
	/// 地図に関する抽象クラス
	/// 具体的なマップはこれを継承して作る
	/// NODE, PATHにはそれぞれNode, Edgeを継承したクラスが使える
	/// PATHにはBasicPathDataを継承しているデータを保持しているEdgeしか使えないので注意
	/// ID ULONG_MAXのノードが入らないように注意!!

	///</summary>
	template <typename NODE, typename PATH>
	class MAP_API Map
	{

	protected:
		std::shared_ptr<const Collection::IdentifiableCollection<std::shared_ptr<NODE const>>> node_collection;
		virtual void build_map() = 0;

		///<summary>
		/// 最短路を格納するルーティングテーブル
		/// SourceからDestinationへ行くのに次にどのノードに行く必要があるかを格納する．
		///</summary>
		std::unique_ptr<const std::vector<const std::vector<node_id>>> routing_table;

	public:
		Map(std::unique_ptr<RoutingMethod<NODE>> routing_method);
		virtual ~Map();

		node_id get_next_node_of_shortest_path(const node_id& from, const node_id& to) const;
		const std::list<node_id> get_shortest_path(const node_id& source, const node_id& destination) const;
		unsigned long calc_necessary_time(const node_id& from, const node_id& to, const double& avg_speed) const;
		bool is_reachable(const node_id& from, const node_id& to, const double& avg_speed, const unsigned int& time_limit) const;
		
		
		const std::list<node_id> get_connecting_nodes(const node_id& id) const;
		std::shared_ptr<const Geography::GeoNode<NODE, PATH>> get_static_node(const node_id& id) const;
		std::shared_ptr<Geography::GeoNode<NODE, PATH>> get_node(const node_id& id);
	};
}

#include "Map.hpp"
