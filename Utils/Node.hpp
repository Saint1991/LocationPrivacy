
namespace Graph 
{

	///<summary>
	/// コンストラクタ
	///</summary>
	///<param name='id'>ノードID</param>
	///<param name='data'>ノードに持たせたいデータ</param>
	template <typename NODE_DATA, typename EDGE>
	Node<NODE_DATA, EDGE>::Node(node_id id, std::shared_ptr<NODE_DATA> data)
		: Identifiable<node_id>(id), data(data), 
		edge_list(std::make_shared< std::set<std::shared_ptr<BasicEdge>, std::function<bool(std::shared_ptr<BasicEdge>, std::shared_ptr<BasicEdge>)>	>>([](std::shared_ptr<BasicEdge> edge1, std::shared_ptr<BasicEdge> edge2) {
			return edge1->get_to() < edge2->get_to();
		}))
	{
	}

	///<summary>
	/// コピーコンストラクタ
	/// edge_listの中身をコピーする
	///</summary>
	template <typename NODE_DATA, typename EDGE>
	Node<NODE_DATA, EDGE>::Node(const Node<NODE_DATA, EDGE> &node) 
		: Identifiable<node_id>(node), data(std::make_shared<NODE_DATA>(*node.data)), 
		edge_list(std::make_shared< std::set<std::shared_ptr<BasicEdge>, std::function<bool(std::shared_ptr<BasicEdge>, std::shared_ptr<BasicEdge>)>	>>([](std::shared_ptr<BasicEdge> edge1, std::shared_ptr<BasicEdge> edge2) {
		return edge1->get_to() < edge2->get_to();
	}))
	{
		for (std::set<std::shared_ptr<BasicEdge>, std::function<bool(std::shared_ptr<BasicEdge>, std::shared_ptr<BasicEdge>)>>::const_iterator iter = node.edge_list->begin(); iter != node.edge_list->end(); iter++) {
			std::shared_ptr<EDGE> edge = std::dynamic_pointer_cast<EDGE>(*iter);
			edge_list->insert(std::make_shared<EDGE>(*edge));
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
		std::set<std::shared_ptr<BasicEdge>, std::function<bool(std::shared_ptr<BasicEdge>, std::shared_ptr<BasicEdge>)>>::const_iterator iter = edge_list->find(std::make_shared<BasicEdge>(id));
		if (iter == edge_list->end()) return nullptr;
		return std::dynamic_pointer_cast<EDGE const>(*iter);
	}


	///<summary>
	/// 指定したノードへのエッジのポインタを取得する
	/// 存在しない場合はnullptrを返す．
	///</summary>
	///<param name='id'>ノードID</param> 
	template <typename NODE_DATA, typename EDGE>
	std::shared_ptr<EDGE> Node<NODE_DATA, EDGE>::get_edge_to(node_id id)
	{
		std::set<std::shared_ptr<BasicEdge>, std::function<bool(std::shared_ptr<BasicEdge>, std::shared_ptr<BasicEdge>)>>::iterator iter = edge_list->find(std::make_shared<BasicEdge>(id));
		if (iter == edge_list->end()) return nullptr;
		return std::dynamic_pointer_cast<EDGE>(*iter);
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
		std::pair<std::set<std::shared_ptr<BasicEdge>, std::function<bool(std::shared_ptr<BasicEdge>, std::shared_ptr<BasicEdge>)>>::iterator, bool > result = edge_list->insert(edge);
		return result.second;
	}


	///<summary>
	/// 指定したIDのエッジを削除し，切断します
	/// 削除が発生した場合true，発生しなかった場合falseを返す．
	///</summary>
	///<param name='from'>切断するノードのID</param>
	template <typename NODE_DATA, typename EDGE>
	bool Node<NODE_DATA, EDGE>::disconnect_from(node_id from)
	{
		std::set<std::shared_ptr<BasicEdge>, std::function<bool(std::shared_ptr<BasicEdge>, std::shared_ptr<BasicEdge>)>>::iterator target = edge_list->find(std::make_shared<BasicEdge>(from));
		if (target != edge_list->end()) {
			edge_list->erase(target);
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
	std::vector<node_id> Node<NODE_DATA, EDGE>::get_connecting_node_list() const
	{
		std::vector<node_id> connect_node_list;
		for (std::set<std::shared_ptr<BasicEdge>, std::function<bool(std::shared_ptr<BasicEdge>, std::shared_ptr<BasicEdge>)>>::const_iterator iter = edge_list->begin(); iter != edge_list->end(); iter++) {
			node_id id = (*iter)->get_to();
			connect_node_list.push_back(id);
		}
		std::sort(connect_node_list.begin(), connect_node_list.end());
		return connect_node_list;
	}

	///<summary>
	/// 各エッジについてexecute_functionを実行する
	/// 変更が中身に反映されるので注意
	///</summary>
	template <typename NODE_DATA, typename EDGE>
	void Node<NODE_DATA, EDGE>::for_each_edge(const std::function<void(std::shared_ptr<EDGE>)>& execute_function)
	{
		for (std::set<std::shared_ptr<BasicEdge>, std::function<bool(std::shared_ptr<BasicEdge>, std::shared_ptr<BasicEdge>)>>::const_iterator iter = edge_list->begin(); iter != edge_list->end(); iter++) {
			std::shared_ptr<EDGE> edge = std::dynamic_pointer_cast<EDGE>(*iter);
			execute_function(edge);
		}
	}

	///<summary>
	/// 各エッジについてexecute_functionを実行する
	///</summary>
	template <typename NODE_DATA, typename EDGE>
	void Node<NODE_DATA, EDGE>::for_each_edge(const std::function<void(std::shared_ptr<EDGE const>)>& execute_function) const
	{
 		for (std::set<std::shared_ptr<BasicEdge>, std::function<bool(std::shared_ptr<BasicEdge>, std::shared_ptr<BasicEdge>)>>::const_iterator iter = edge_list->begin(); iter != edge_list->end(); iter++) {
			std::shared_ptr<EDGE const> edge = std::dynamic_pointer_cast<EDGE const>(*iter);
			execute_function(edge);
		}
	}


	///<summary>
	/// 各エッジについてexecute_functionを実行する
	/// ただし走査は逆順
	///</summary>
	template <typename NODE_DATA, typename EDGE>
	void Node<NODE_DATA, EDGE>::rfor_each_edge(const std::function<void(std::shared_ptr<EDGE>)>& execute_function)
	{
		for (std::set<std::shared_ptr<BasicEdge>, std::function<bool(std::shared_ptr<BasicEdge>, std::shared_ptr<BasicEdge>)>>::reverse_iterator iter = edge_list->rbegin(); iter != edge_list->rend(); iter++) {
			std::shared_ptr<EDGE> edge = std::dynamic_pointer_cast<EDGE>(*iter);
			execute_function(edge);
		}
	}

	///<summary>
	/// 各エッジについてexecute_functionを実行する
	/// ただし走査は逆順
	///</summary>
	template <typename NODE_DATA, typename EDGE>
	void Node<NODE_DATA, EDGE>::rfot_each_edge(const std::function<void(std::shared_ptr<EDGE const>)>& execute_function) const
	{
		for (std::set<std::shared_ptr<BasicEdge>, std::function<bool(std::shared_ptr<BasicEdge>, std::shared_ptr<BasicEdge>)>>::const_reverse_iterator iter = edge_list->rbegin(); iter != edge_list->rend(); iter++) {
			std::shared_ptr<EDGE const> edge = std::dynamic_pointer_cast<EDGE const>(*iter);
			execute_function(edge);
		}
	}
}


