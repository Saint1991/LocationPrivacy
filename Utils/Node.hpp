
namespace Graph 
{

	///<summary>
	/// コンストラクタ
	///</summary>
	///<param name='id'>ノードID</param>
	///<param name='data'>ノードに持たせたいデータ</param>
	template <typename NODE_DATA, typename EDGE>
	Node<NODE_DATA, EDGE>::Node(node_id id, std::shared_ptr<NODE_DATA> data)
		: Identifiable(id), data(data), edge_list(std::make_shared<std::list<std::shared_ptr<EDGE>>>())
	{
	}

	///<summary>
	/// コピーコンストラクタ
	/// edge_listの中身をコピーする
	///</summary>
	template <typename NODE_DATA, typename EDGE>
	Node<NODE_DATA, EDGE>::Node(const Node<NODE_DATA, EDGE> &node) 
		: Identifiable(node), data(node.data), edge_list(std::make_shared<std::list<std::shared_ptr<EDGE>>>())
	{
		for (std::list<std::shared_ptr<EDGE>>::const_iterator iter = node.edge_list->begin(); iter != node.edge_list->end(); iter++) {
			EDGE copy = *(*iter);
			edge_list->push_back(std::make_shared<EDGE>(copy));
		}
	}

	///<summary>
	/// デストラクタ
	///</summary>
	template <typename NODE_DATA, typename EDGE>
	Node<NODE_DATA, EDGE>::~Node()
	{
	}


	///<summary>
	/// 指定したノードへのエッジのポインタを取得する
	/// エッジへの変更は不可
	/// 存在しない場合はnullptrを返す．
	///</summary>
	///<param name='id'>ノードID</param> 
	template <typename NODE_DATA, typename EDGE>
	std::shared_ptr<EDGE const> Node<NODE_DATA, EDGE>::get_static_edge_to(node_id id) const
	{
		for (std::list<std::shared_ptr<EDGE>>::const_iterator iter = edge_list->begin(); iter != edge_list->end(); iter++) {
			if ((*iter)->get_to() == id) {
				return *iter;
			}
		}
		return nullptr;
	}


	///<summary>
	/// 指定したノードへのエッジのポインタを取得する
	/// 存在しない場合はnullptrを返す．
	///</summary>
	///<param name='id'>ノードID</param> 
	template <typename NODE_DATA, typename EDGE>
	std::shared_ptr<EDGE> Node<NODE_DATA, EDGE>::get_edge_to(node_id id)
	{
		for (std::list<std::shared_ptr<EDGE>>::const_iterator iter = edge_list->begin(); iter != edge_list->end(); iter++) {
			if ((*iter)->get_to() == id) {
				return *iter;
			}
		}
		return nullptr;
	}

	///<summary>
	/// 指定したパラメータを持つエッジを接続エッジリストに追加します．
	/// 既存のエッジの場合は追加せずにfalseを返します．
	///</summary>
	///<param name='to'>接続先ノード</param>
	///<param name='data'>エッジが持つ属性値データ</param>
	template <typename NODE_DATA, typename EDGE>
	bool Node<NODE_DATA, EDGE>::connect_to(std::shared_ptr<EDGE> edge)
	{
		std::shared_ptr<EDGE> target = get_edge_to(edge->get_to());
		if (target != nullptr) {
			return false;
		}
		edge_list->push_back(edge);
		return true;
	}


	///<summary>
	/// 指定したIDのエッジを削除し，切断します
	/// 削除が発生した場合true，発生しなかった場合falseを返す．
	///</summary>
	///<param name='from'>切断するノードのID</param>
	template <typename NODE_DATA, typename EDGE>
	bool Node<NODE_DATA, EDGE>::disconnect_from(node_id from)
	{
		std::shared_ptr<EDGE> target = get_edge_to(from);
		if (target != nullptr) {
			edge_list->remove(target);
			return true;
		}
		return false;
	}

	///<summary>
	/// 指定したノードと接続しているかどうかを判定する
	///</summary>
	template <typename NODE_DATA, typename EDGE>
	bool Node<NODE_DATA, EDGE>::is_connecting_to(node_id link_to) const
	{
		return get_static_edge_to(link_to) != nullptr;
	}

	///<summary>
	/// 接続先ノードのIDリストを返す．
	/// ノード番号は昇順にソート
	///</summary>
	template <typename NODE_DATA, typename EDGE>
	std::list<node_id> Node<NODE_DATA, EDGE>::get_connecting_node_list() const
	{
		std::list<node_id> connect_node_list;
		for (std::list<std::shared_ptr<EDGE>>::const_iterator iter = edge_list->begin(); iter != edge_list->end(); iter++) {
			node_id id = (*iter)->get_to();
			connect_node_list.push_back(id);
		}
		connect_node_list.sort();
		return connect_node_list;
	}

	///<summary>
	/// 各エッジについてexecute_functionを実行する
	/// 変更が中身に反映されるので注意
	///</summary>
	template <typename NODE_DATA, typename EDGE>
	void Node<NODE_DATA, EDGE>::for_each_edge(const std::function<void(std::shared_ptr<EDGE>)>& execute_function)
	{
		for (std::list<std::shared_ptr<EDGE>>::const_iterator iter = edge_list->begin(); iter != edge_list->end(); iter++) {
			execute_function(*iter);
		}
	}

	///<summary>
	/// 各エッジについてexecute_functionを実行する
	///</summary>
	template <typename NODE_DATA, typename EDGE>
	void Node<NODE_DATA, EDGE>::for_each_edge(const std::function<void(std::shared_ptr<EDGE const>)>& execute_function) const
	{
 		for (std::list<std::shared_ptr<EDGE>>::const_iterator iter = edge_list->begin(); iter != edge_list->end(); iter++) {
			execute_function(*iter);
		}
	}
}


