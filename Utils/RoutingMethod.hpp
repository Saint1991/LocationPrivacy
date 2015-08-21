
namespace Graph
{
	///<summary>
	/// 2つのノード間の距離を取得する
	/// from == toの場合は0が返り，接続していない場合はNO_CONNECTIONが返る
	/// NODEにはBasicPathDataの派生クラスをエッジに持つNodeクラスを継承したクラスしか持てないので注意
	///</summary>
	template <typename NODE>
	double RoutingMethod<NODE>::distance(const node_id& from, const node_id& to) 
	{
		if (from == to) return 0;

		std::shared_ptr<NODE const> node = node_collection->read_by_id(from);
		if (node == nullptr) return NO_CONNECTION;

		auto edge = node->get_static_edge_to(to);
		if (edge == nullptr) return NO_CONNECTION;

		auto path_data = path->get_static_data();
		if (path_data == nullptr) return NO_CONNECTIOIN;

		return path_data->get_distance();
	}

	template <typename NODE>
	void create_distance_map(std::shared_ptr<const Collection::IdentifiableCollection<std::shared_ptr<NODE const>>> node_collectioin)
	{
		distance_map = std::make_unique<std::vector<std::vector<double>>>();
	}

	///<summary>
	/// node_collectionを設定する
	///</summary>
	template <typename NODE>
	void RoutingMethod<NODE>::set_node_collection(std::shared_ptr<const Collection::IdentifiableCollection<std::shared_ptr<NODE const>> nodes)
	{
		node_collection = nodes;
		create_distance_map();
	}
}
