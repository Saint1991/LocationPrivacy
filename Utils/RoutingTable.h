#pragma once
#include "BasicEdge.h"

namespace Graph
{
	
	constexpr double NO_CONNECTION = DBL_MAX;

	///<summary>
	/// ノード間の最短路を表すルーティングテーブル
	///</summary>
	class RoutingTable
	{
	protected:
		std::unique_ptr<std::vector<std::vector<node_id>>> routing_table;
		std::unique_ptr < std::vector<std::vector<double>>> shortest_distance_table;
		std::unique_ptr<std::unordered_map<node_id, int>> conversion_map;
	
	public:
		RoutingTable(std::unique_ptr<std::vector<std::vector<node_id>>> table, std::unique_ptr < std::vector<std::vector<double>>> shortest_distance_table, std::unique_ptr<std::unordered_map<node_id, int>> conversion_map);
		virtual ~RoutingTable();

		node_id get_next_node_of_shortest_path(const node_id& from, const node_id& to) const;
		const std::list<node_id> get_shortest_path(const node_id& source, const node_id& destination) const;
		double shortest_distance(const node_id& from, const node_id& to) const;
	};
}


