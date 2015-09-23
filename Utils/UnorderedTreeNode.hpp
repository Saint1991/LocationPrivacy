
namespace Graph
{

	///<summary>
	/// コンストラクタ
	///</summary>
	template <typename DATA_TYPE, typename EDGE>
	UnorderedTreeNode<DATA_TYPE, EDGE>::UnorderedTreeNode(node_id id, std::shared_ptr<DATA_TYPE> data) : Identifiable<node_id>(id), data(data),
		edge_list(std::make_shared< std::set<std::shared_ptr<EDGE>, std::function<bool(std::shared_ptr<EDGE>, std::shared_ptr<EDGE>)>	>>([](std::shared_ptr<EDGE> a, std::shared_ptr<EDGE> b) {
		return a->get_to() < b->get_to();
	}))
	{
	}


	///<summary>
	/// コピーコンストラクタ
	///</summary>
	template <typename DATA_TYPE, typename EDGE>
	UnorderedTreeNode<DATA_TYPE, EDGE>::UnorderedTreeNode(const UnorderedTreeNode& node) : Identifiable<node_id>(node.id), data(std::make_shared<DATA_TYPE>(node.data)),
		edge_list(std::make_shared< std::set<std::shared_ptr<EDGE>, std::function<bool(std::shared_ptr<EDGE>, std::shared_ptr<EDGE>)>	>>([](std::shared_ptr<EDGE> a, std::shared_ptr<EDGE> b) {
			return a->get_to() < b->get_to();
		}))
	{
		for (auto iter = node.edge_list->begin(); iter != node.edge_list->end(); iter++) {
			edge_list->insert(std::make_shared<EDGE>(**iter));
		}
	}


	///<summary>
	/// デストラクタ
	///</summary>
	template <typename DATA_TYPE, typename EDGE>
	UnorderedTreeNode<DATA_TYPE, EDGE>::~UnorderedTreeNode()
	{
	}


	///<summary>
	/// 子ノードへのエッジを追加する
	/// 重複したIDを持つ要素を追加しようとした場合はfalseが返されます．
	///</summary>
	template <typename DATA_TYPE, typename EDGE>
	bool UnorderedTreeNode<DATA_TYPE, EDGE>::add_child(EDGE to_child)
	{
		std::pair< std::set< std::shared_ptr<EDGE>, std::function<bool(std::shared_ptr<EDGE>, std::shared_ptr<EDGE>)>>::iterator, bool> result = edge_list->insert(std::make_shared<EDGE>(to_child));
		return result.second;
	}


	///<summary>
	/// idがchild_idのノードへのエッジを取得する
	/// 存在しないエッジの場合はnullptrを返す
	///</summary>
	template <typename DATA_TYPE, typename EDGE>
	std::shared_ptr<EDGE const> UnorderedTreeNode<DATA_TYPE, EDGE>::get_static_edge_to(const node_id& child_id) const
	{
		std::shared_ptr<EDGE const> edge = edge_list->find(child_id);
		if (edge = edge_list->end()) return nullptr;
		return edge;
	}


	///<summary>
	/// idがchild_idのノードへのエッジを取得する
	/// 存在しないエッジの場合はnullptrを返す
	///</summary>
	template <typename DATA_TYPE, typename EDGE>
	std::shared_ptr<EDGE> UnorderedTreeNode<DATA_TYPE, EDGE>::get_edge_to(const node_id& child_id)
	{
		std::shared_ptr<EDGE> edge = edge_list->find(child_id);
		if (edge = edge_list->end()) return nullptr;
		return edge;
	}


	///<summary>
	/// 指定したIDを持つ子が存在するかどうかチェックする
	///</summary>
	template <typename DATA_TYPE, typename EDGE>
	bool UnorderedTreeNode<DATA_TYPE, EDGE>::has_child_of(const node_id& target_id) const
	{
		bool is_exists = edge_list->find(target_id) != edge_list->end();
		return is_exists;
	}


	///<summary>
	/// 子ノードのID一覧を取得する
	///</summary>
	template <typename DATA_TYPE, typename EDGE>
	std::vector<node_id>UnorderedTreeNode<DATA_TYPE, EDGE>::get_child_ids() const
	{
		std::vector<node_id> ret;
		for (auto iter = edge_list->begin(); iter != edge_list->end(); iter++) {
			ret.push_back(iter->get_to());
		}
		return ret;
	}
}

