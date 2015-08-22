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

	template <typename NODE, typename EDGE>
	void WarshallFloyd<NODE, EDGE>::initialize()
	{
		//ノード数の取得
		node_count = node_collection->size();

		//インデックスとノードIDの変換マップの作成
		create_conversion_map(node_collection);

		//距離行列の初期化
		initialize_distance_map(node_collection);
	}


	///<summary>
	/// インデックスとノードIDの変換マップを作成する
	///</summary>
	template <typename NODE, typename EDGE>
	void WarshallFloyd<NODE, EDGE>::create_conversion_map(std::shared_ptr<const Collection::IdentifiableCollection<NODE const>> node_collection)
	{
		conversion_map = std::make_unique<std::unordered_map<node_id, int>>(node_count);
		std::unique_ptr<std::vector<long>> id_list = node_collection->get_id_list();
		for (std::vector<node_id>::const_iterator iter = id_list->begin(), long index = 0; iter != id_list->end(); iter++, index++) {
			conversion_map->insert({*iter, index});
		}
	}


	///<summary>
	/// node_collectionを基に距離行列を初期化する
	/// 自身から自身への距離を0，他の要素をNO_CONNECTION = DBL_MAXで初期化し，
	/// その後接続しているノード間の距離を格納する
	/// EDGEが持つデータはBasicPathDataを継承したクラスであることを想定
	///</summary>
	template <typename NODE, typename EDGE>
	void	WarshallFloyd<NODE, EDGE>::initialize_distance_map(std::shared_ptr<const Collection::IdentifiableCollection<NODE const>> node_collection)
	{
		//最初は全ての要素をNO_CONNECTION = DBL_MAXで初期化する
		distance_map = std::make_unique<std::vector<std::vector<double>>>(node_count, std::vector<double>(node_count, NO_CONNECTION));

		//node_collectionを参照してノード間の距離を格納，さらに自身への距離は0にする
		int node_index = 0;
		node_collection->foreach([&](std::shared_ptr<NODE const> node) {

			distance_map->at(node_index)->at(node_index) = 0;
			
			node->for_each_edge([&](std::shared_ptr<EDGE const> edge) {
				long index_to =conversion_map->at(edge->get_to());
				double distance = edge->get_static_data()->get_distance();
				distance_map->at(node_index)->at(index_to) = distance;
			});
			node_index++;
		});
	}


	///<summary>
	/// ワーシャルフロイド法によるルーティングテーブルの作成
	/// 参照 (http://dai1741.github.io/maximum-algo-2012/docs/shortest-path/)
	/// nodesが正しく渡されていないとnullptrが返る
	///</summary>
	template <typename NODE, typename EDGE>
	std::unique_ptr<RoutingTable const>	WarshallFloyd<NODE>::create_routing_table(std::shared_ptr<const Collection::IdentifiableCollection<NODE const>> node_collection) const
	{
		//フィールドにnode_collectionへの参照を持たせる (距離計算 distanceメソッドのため)
		if (node_collection == nullptr) return nullptr;

		//各フィールドを初期化する
		initialize();
		
		//ここからルーティングテーブルの作成を行う，最初は全要素NOWHERE=-1で初期化
		std::unique_ptr<std::vector<std::vector<node_id>>> routing_table = std::make_unique<std::vector<std::vector<node_id>>>(NODE_COUNT, std::vector<node_id>(NODE_COUNT, NOWHERE));





		return std::move(routing_table);
	}

}
