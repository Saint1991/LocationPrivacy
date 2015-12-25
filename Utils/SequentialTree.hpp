
namespace Graph
{

	///<summary>
	/// コンストラクタ
	///</summary>
	template <typename NODE, typename DATA>
	SequentialTree<NODE, DATA>::SequentialTree(std::shared_ptr<DATA> root_data) : Tree<NODE, DATA, BasicEdge>()
	{
		Graph::node_id current_id = node_collection->size();
		initialize(std::make_shared<NODE>(current_id, -1, 0, root_data));
	}


	///<summary>
	/// デストラクタ
	///</summary>
	template <typename NODE, typename DATA>
	SequentialTree<NODE, DATA>::~SequentialTree()
	{
	}


	///<summary>
	/// ルートノードを指定して木を初期化する
	/// ルートでは親ノードのIDは-1とする
	/// そうでない場合はinvalid_argumentをスローする
	///</summary>
	template <typename NODE, typename DATA>
	void SequentialTree<NODE, DATA>::initialize(std::shared_ptr<NODE> root_node)
	{
		if (root_node->get_parent() != -1) throw std::invalid_argument("Parent Node ID must be -1 for root Node");
		Tree<NODE, DATA, BasicEdge>::initialize(root_node);
	}

	///<summary>
	/// 最も長いシーケンスの長さを取得する
	///</summary>
	template <typename NODE, typename DATA>
	int SequentialTree<NODE, DATA>::max_depth() const
	{
		int max = 0;
		node_collection->foreach([&max](std::shared_ptr<NODE const> node) {
			int depth = node->get_depth();
			if (max < depth) max = depth;
		});
		return max;
	}


	///<summary>
	/// 葉ノードを全て取得する
	///</summary>
	template <typename NODE, typename DATA>
	std::vector<std::shared_ptr<NODE const>> SequentialTree<NODE, DATA>::get_all_leaves() const
	{
		std::vector<std::shared_ptr<NODE const>> ret;
		node_collection->foreach([&ret](std::shared_ptr<NODE const> node) {
			if (node->is_leaf()) ret.push_back(node);
		});
		return ret;
	}


	///<summary>
	/// 指定した深さにあるノードを全て取得する
	///</summary>
	template <typename NODE, typename DATA>
	std::vector<std::shared_ptr<NODE const>> SequentialTree<NODE, DATA>::get_all_nodes_in_depth(int depth) const
	{
		std::vector<std::shared_ptr<NODE const>> ret;
		node_collection->foreach([&depth, &ret](std::shared_ptr<NODE const> node) {
			if (node->get_depth() == depth) ret.push_back(node);
		});
		return ret;
	}


	///<summary>
	/// 木をたどってnodeに対応するノードのシーケンスを取得する
	///</summary>
	template <typename NODE, typename DATA>
	Collection::Sequence<std::shared_ptr<NODE const>> SequentialTree<NODE, DATA>::get_sequence_by_node(std::shared_ptr<NODE const> node) const
	{
		Graph::node_id current_node_id = node->get_id();
		std::shared_ptr<NODE const> current_node = node;

		Collection::Sequence<std::shared_ptr<NODE const>> sequence;
		while (current_node != nullptr && current_node_id != 0 && current_node_id != -1) {
			sequence.push_back(current_node);
			current_node_id = current_node->get_parent();
			current_node = node_collection->read_by_id(current_node_id);
		}

		std::reverse(sequence.begin(), sequence.end());
		return sequence;
	}


	///<summary>
	/// 根から葉までの全てのシーケンスに対してexecute_functionを実行する
	///</summary>
	template <typename NODE, typename DATA>
	void SequentialTree<NODE, DATA>::for_each_sequence(const std::function<void(const Collection::Sequence<std::shared_ptr<NODE const>>)>& execute_function) const
	{
		node_collection->foreach([&](std::shared_ptr<NODE const> node) {
			if (node->is_leaf()) {
				Collection::Sequence<std::shared_ptr<NODE const>> sequence = get_sequence_by_node(node);
				execute_function(sequence);
			}
		});
	}


	///<summary>
	/// 長さlengthの全てのシーケンスに対してexecute_functionを実行する
	///</summary>
	template <typename NODE, typename DATA>
	void SequentialTree<NODE, DATA>::for_each_sequence(size_t length, const std::function<void(const Collection::Sequence<std::shared_ptr<NODE const>>)>& execute_function) const
	{
		node_collection->foreach([&](std::shared_ptr<NODE const> node) {
			if (node->get_depth() == length) {
				Collection::Sequence<std::shared_ptr<NODE const>> sequence = get_sequence_by_node(node);
				execute_function(sequence);
			}
		});
	}
}

