
namespace Graph 
{

	///<summary>
	/// コンストラクタ
	///</summary>
	template <typename NODE_DATA>
	 BasicNode<NODE_DATA>::BasicNode(const  node_id& id, std::shared_ptr<NODE_DATA> data) :  Node<NODE_DATA, BasicPath>(id, data)
	{
	}

	///<summary>
	/// コピーコンストラクタ
	///</summary>
	template <typename NODE_DATA>
	BasicNode<NODE_DATA>::BasicNode(const  BasicNode& node) : Node<NODE_DATA, BasicPath>(node)
	{
	}

	///<summary>
	/// デストラクタ
	///</summary>
	template <typename NODE_DATA>
	 BasicNode<NODE_DATA>::~BasicNode()
	{
	}

	 ///<summary>
	 /// toまでの距離を更新する
	 /// toへのエッジを保持していない場合はfalseを返す
	 ///</summary>
	template <typename NODE_DATA>
	bool  BasicNode<NODE_DATA>::update_distance_to(const node_id& to, const double& distance)
	{
		std::shared_ptr<BasicPath> target_path = get_edge_to(to);
		if (target_path == nullptr) return false;
		target_path->update_distance(distance);
		return true;
	}

	///<summary>
	/// toまでの距離を取得する
	/// 接続していないノードならDBL_MAXが返される
	///</summary>
	template <typename NODE_DATA>
	const double BasicNode<NODE_DATA>::distance_to(const node_id& to) const
	{
		std::shared_ptr<BasicPath const> target_path = get_static_edge_to(to);
		if (target_path == nullptr) return DBL_MAX;
		return target_path->distance();
	}

}
