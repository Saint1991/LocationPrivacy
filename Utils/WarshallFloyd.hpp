#pragma once

namespace Graph 
{
	///<summary>
	/// コンストラクタ
	///</summary>
	template <typename NODE, typename EDGE>
	WarshallFloyd<NODE, EDGE>::WarshallFloyd() : RoutingMethod()
	{
	}


	///<summary>
	/// デストラクタ
	///</summary>
	template <typename NODE, typename EDGE>
	WarshallFloyd<NODE, EDGE>::~WarshallFloyd()
	{
	}


	///<summary>
	/// 距離行列，ルーティング結果の初期化
	///</summary>
	template <typename NODE, typename EDGE>
	void WarshallFloyd<NODE, EDGE>::initialize(std::shared_ptr<const Collection::IdentifiableCollection<Graph::node_id, NODE>> node_collection)
	{
		//ノード数の取得
		node_count = node_collection->size();

		//インデックスとノードIDの変換マップの作成
		create_conversion_map(node_collection);

		//距離行列とルーティングテーブルの初期化
		//距離行列は全ての要素をNO_CONNECTION = DBL_MAXで初期化，ルーティングテーブルは全要素NOWHERE = -1で初期化
		distance_map = std::make_unique<std::vector<std::vector<double>>>(node_count, std::vector<double>(node_count, NO_CONNECTION));
		routing_table = std::make_unique<std::vector<std::vector<node_id>>>(node_count, std::vector<node_id>(node_count, NOWHERE));

		//node_collectionを参照してノード間の距離を格納，自身への距離は0にする
		//自身への遷移はSELF = -2とする，ルーティングテーブルはインデックスでなくノードのIDを格納する
		int node_index = 0;
		node_collection->foreach([&](std::shared_ptr<NODE const> node) {

			node->for_each_edge([&](std::shared_ptr<EDGE const> edge) {
				node_id id = edge->get_to();
				long index_to = conversion_map->at(id);
				double distance = edge->get_static_data()->get_distance();
				distance_map->at(node_index).at(index_to) = distance;
				routing_table->at(node_index).at(index_to) = id;
			});
			distance_map->at(node_index).at(node_index) = 0;
			routing_table->at(node_index).at(node_index) = SELF;
			node_index++;

		});
	}


	///<summary>
	/// インデックスとノードIDの変換マップを作成する (とりあえずここでは処理速度を優先して両方向の変換マップを作成しているが，
	/// メモリ効率を優先するなら片方向にしてインデックスを探索するように直す
	///</summary>
	template <typename NODE, typename EDGE>
	void WarshallFloyd<NODE, EDGE>::create_conversion_map(std::shared_ptr<const Collection::IdentifiableCollection<Graph::node_id, NODE>> node_collection)
	{
		conversion_map = std::make_unique<std::unordered_map<node_id, int>>(node_count);
		reverse_conversion_map = std::make_unique<std::vector<node_id>>(node_count);
		long index = 0;
		node_collection->foreach([&](std::shared_ptr<NODE const> node) {
			node_id id = node->get_id();
			conversion_map->insert(std::make_pair(id, index));
			reverse_conversion_map->at(index) = id;
			index++;
		});
	}


	///<summary>
	/// ワーシャルフロイド法によるルーティングテーブルの作成
	/// 参照 (http://dai1741.github.io/maximum-algo-2012/docs/shortest-path/)
	/// node_collectionが正しく渡されていないとnullptrが返る
	///</summary>
	template <typename NODE, typename EDGE>
	std::unique_ptr<RoutingTable const>	WarshallFloyd<NODE, EDGE>::create_routing_table(std::shared_ptr<const Collection::IdentifiableCollection<Graph::node_id, NODE>> node_collection)
	{
		//フィールドにnode_collectionへの参照を持たせる (距離計算 distanceメソッドのため)
		if (node_collection == nullptr) return nullptr;

		//各フィールドを初期化する
		initialize(node_collection);

		//ワーシャルフロイド法で最短路を確定させていく
		for (long source = 0; source < node_count; source++) {
			for (long destination = 0; destination < node_count; destination++) {
				for (long via = 0; via < node_count; via++) {
					
					double distance_src_dest = distance_map->at(source).at(destination);
					double distance_src_via_dest = distance_map->at(source).at(via) + distance_map->at(via).at(destination);
					if (distance_src_via_dest < distance_src_dest) {
						distance_map->at(source).at(destination) = distance_src_via_dest;
						routing_table->at(source).at(destination) = reverse_conversion_map->at(via);
					}

				}
			}
		}

		std::unique_ptr<RoutingTable const> ret = std::make_unique<RoutingTable const>(std::move(routing_table), std::move(distance_map), std::move(conversion_map));
		return std::move(ret);
	}

}
