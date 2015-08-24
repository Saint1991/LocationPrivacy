
namespace Geography
{

	///<summary>
	/// コンストラクタ
	///</summary>
	template <typename POI_DATA>
	Poi<POI_DATA>::Poi(std::string id, std::shared_ptr<POI_DATA> data) : id(id), data(data), connection_map(std::make_unique<std::unordered_map<node_poi_relation>>())
	{
	}


	///<summary>
	/// デストラクタ
	///</summary>
	template <typename POI_DATA>
	Poi<POI_DATA>::~Poi()
	{
	}


	///<summary>
	/// POIのIDの取得
	///</summary>
	template <typename POI_DATA>
	const std::string Poi<POI_DATA>::get_id() const
	{
		return id;
	}


	///<summary>
	/// 変更不可の状態で保持しているデータを取得する
	///</summary>
	template <typename POI_DATA>
	std::shared_ptr<POI_DATA const> Poi<POI_DATA>::get_static_data() const
	{
		return data;
	}


	///<summary>
	/// 保持しているデータを取得する
	/// 変更が中身にも反映されるので注意
	///</summary>
	template <typename POI_DATA>
	std::shared_ptr<POI_DATA> Poi<POI_DATA>::get_data()
	{
		return data;
	}


	///<summary>
	/// ノードとの関係構成用
	/// ((ノードAのID, ノードBのID), (ノードAまでの距離, ノードBまでの距離))を表すデータを格納する
	///</summary>
	template <typename POI_DATA>
	void Poi<POI_DATA>::add_relation(const node_poi_relation& relation)
	{
		connection_map->insert(relation);
	}


	///<summary>
	/// このPOIに隣接するノードのID一覧を取得する
	///</summary>
	template <typaname POI_DATA>
	std::list<Graph::node_id> Poi<POI_DATA>::get_srounding_nodes() const
	{
		std::list<Graph::node_id> ret;
		for (std::unordered_map<node_poi_relation>::const_iterator iter = connection_map->begin(); iter != connection_map->end(); iter++) {
			std::pair<Graph::node_id, Graph::node_id> id_pair = iter->first;
			ret.push_back(id_pair.first);
			ret.push_back(id_pair.second);
		}

		ret.sort();
		ret.unique();
		return std::move(ret);
	}
}


