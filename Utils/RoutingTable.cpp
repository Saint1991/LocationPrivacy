#include "stdafx.h"
#include "RoutingTable.h"

namespace Graph 
{

	///<summary>
	/// コンストラクタ
	///</summary>
	RoutingTable::RoutingTable(std::unique_ptr<std::vector<std::vector<node_id>>> table, std::unique_ptr<std::unordered_map<node_id, int>> conversion_map) 
		: table(std::move(table)), conversion_map(std::move(conversion_map))
	{
	}


	///<summary>
	/// デストラクタ
	///</summary>
	RoutingTable::~RoutingTable()
	{
	}


	///<summary>
	/// from から toへ最短路で行くときに次に進むべきノードのIDを取得
	/// 接続していない場合はNOWHEREが返る
	///</summary>
	node_id RoutingTable::get_next_node_of_shortest_path(const node_id& from, const node_id& to) const
	{
		long index_from = conversion_map->at(from);
		long index_to = conversion_map->at(to);
		node_id next = table->at(from).at(to);
		return next;
	}

	///<summary>
	/// node_idの系列で経路を返す．
	/// 到達不可能な場合は最後尾がNOWHEREになる
	/// source == destinationの場合は空のリストが返される
	///</summary>
	const std::list<node_id> RoutingTable::get_shortest_path(const node_id& source, const node_id& destination) const
	{
		std::list<node_id> shortest_path;
		if (source == destination) return shortest_path;

		node_id iter = source;
		while (iter != destination && iter != NOWHERE) {
			iter = get_next_node_of_shortest_path(iter, destination);
			shortest_path.push_back(iter);
		}
		return std::move(shortest_path);
	}
}

