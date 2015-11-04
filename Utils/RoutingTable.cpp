#include "stdafx.h"
#include "RoutingTable.h"

namespace Graph 
{

	///<summary>
	/// コンストラクタ
	/// routing_tableには[fromノードのインデックス][toノードのインデックス] => 次行くべきノードIDの行列
	/// shortest_distance_tableには[fromノードのインデックス][toノードのインデックス] = 最短ルートにおける距離
	/// conversionテーブルは[ノードID] => ノードのインデックス
	/// が格納されていることを想定している
	///</summary>
	RoutingTable::RoutingTable(std::unique_ptr<std::vector<std::shared_ptr<std::vector<node_id>>>> routing_table, std::unique_ptr<std::vector<std::shared_ptr<std::vector<double>>>> shortest_distance_table, std::unique_ptr<std::unordered_map<node_id, int>> conversion_map)
		: routing_table(std::move(routing_table)), shortest_distance_table(std::move(shortest_distance_table)), conversion_map(std::move(conversion_map))
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
		node_id next = routing_table->at(index_from)->at(index_to);
		return next;
	}

	///<summary>
	/// node_idの系列で経路を返す．
	/// 到達不可能な場合は最後尾がNOWHEREになる
	/// source == destinationの場合は空のリストが返される
	///</summary>
	const std::vector<node_id> RoutingTable::get_shortest_path(const node_id& source, const node_id& destination) const
	{
		std::vector<node_id> shortest_path;
		if (source == destination) return shortest_path;

		node_id iter = source;
		while (iter != destination && iter != NOWHERE) {
			iter = get_next_node_of_shortest_path(iter, destination);
			shortest_path.push_back(iter);
		}
		return std::move(shortest_path);
	}


	///<summary>
	/// fromからtoへ到達する最短経路の距離を返す
	///</summary>
	double RoutingTable::shortest_distance(const node_id& from, const node_id& to) const
	{
		long index_from = conversion_map->at(from);
		long index_to = conversion_map->at(to);
		double shortest_distance = shortest_distance_table->at(index_from)->at(index_to);
		return shortest_distance;
	}



	///<summary>
	/// candidatesのうちfromから最も近いものを返します
	/// candidatesが不正な場合はNO_CONNECTIONを返します
	///</summary>
	node_id RoutingTable::get_nearest_from(const node_id& from, const std::vector<node_id>& candidates) const
	{
		node_id ret = NOWHERE;
		double min_distance = NO_CONNECTION;
		for (std::vector<node_id>::const_iterator iter = candidates.begin(); iter != candidates.end(); iter++) {
			double distance = shortest_distance(from, *iter);
			if (distance < min_distance) {
				min_distance = distance;
				ret = *iter;
			}
		}
		return ret;
	}
}

