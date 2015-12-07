
namespace Graph
{


	///<summary>
	/// �R���X�g���N�^
	///</summary>
	template <typename NODE, typename DATA>
	SequentialTree<NODE, DATA>::SequentialTree()
	{
		initialize();
	}


	///<summary>
	/// �R���X�g���N�^
	///</summary>
	template <typename NODE, typename DATA>
	SequentialTree<NODE, DATA>::SequentialTree(std::shared_ptr<DATA> root_data)
	{
		initialize(root_data);
	}

	template <typename NODE, typename DATA>
	SequentialTree<NODE, DATA>::~SequentialTree()
	{
	}


	///<summary>
	/// �ł������V�[�P���X�̒������擾����
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
	/// �t�m�[�h��S�Ď擾����
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
	/// �w�肵���[���ɂ���m�[�h��S�Ď擾����
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
	/// �؂����ǂ���node�ɑΉ�����m�[�h�̃V�[�P���X���擾����
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
	/// ������t�܂ł̑S�ẴV�[�P���X�ɑ΂���execute_function�����s����
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
	/// ����length�̑S�ẴV�[�P���X�ɑ΂���execute_function�����s����
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

