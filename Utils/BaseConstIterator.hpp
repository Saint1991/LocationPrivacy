
namespace Graph
{

	///<summary>
	/// �R���X�g���N�^
	///</summary>
	template <typename NODE, typename NODE_DATA, typename EDGE>
	PrefixTree<NODE, NODE_DATA, EDGE>::BaseConstIterator::BaseConstIterator() : node(root)
	{

	}


	///<summary>
	/// �R���X�g���N�^
	///</summary>
	template <typename NODE, typename NODE_DATA, typename EDGE>
	PrefixTree<NODE, NODE_DATA, EDGE>::BaseConstIterator::BaseConstIterator(std::shared_ptr<NODE const> node) : node(node)
	{

	}


	///<summary>
	/// �f�X�g���N�^
	///</summary>
	template <typename NODE, typename NODE_DATA, typename EDGE>
	PrefixTree<NODE, NODE_DATA, EDGE>::BaseConstIterator::~BaseConstIterator()
	{

	}

	///<summary>
	/// �Q�ƊO��
	///</summary>
	template <typename NODE, typename NODE_DATA, typename EDGE>
	std::shared_ptr<NODE const> PrefixTree<NODE, NODE_DATA, EDGE>::BaseConstIterator::operator*() const
	{
		return node;
	}

	///<summary>
	/// �����ɍ����q�v�f���������ăC�e���[�^���擾����
	/// �����ɍ����v�f���Ȃ��ꍇ��nullptr���w���C�e���[�^��Ԃ��܂�
	///</summary>
	template <typename NODE, typename NODE_DATA, typename EDGE>
	PrefixTree<NODE, NODE_DATA, EDGE>::BaseConstIterator PrefixTree<NODE, NODE_DATA, EDGE>::BaseConstIterator::find_child_if(const std::function<bool(std::shared_ptr<NODE const>)>& compare) const
	{
		node->for_each_edge([&](std::shared_ptr<EDGE const> edge) {
			node_id to = edge->get_to();
			std::shared_ptr<NODE const> node = node_collection->read_by_id(to);
			if (compare(node)) return PrefixTree<NODE, NODE_DATA, EDGE>::BaseIterator(node);
		});
		return PrefixTree<NODE, NODE_DATA, EDGE>::BaseIterator(nullptr);
	}

	///<summary>
	/// ��r���Z�q
	///</summary>
	template <typename NODE, typename NODE_DATA, typename EDGE>
	bool PrefixTree<NODE, NODE_DATA, EDGE>::BaseConstIterator::operator==(const PrefixTree<NODE, NODE_DATA, EDGE>::BaseConstIterator& iter) const
	{
		return **iter == *node;
	}

	///<summary>
	/// ��r���Z�q
	///</summary>
	template <typename NODE, typename NODE_DATA, typename EDGE>
	bool PrefixTree<NODE, NODE_DATA, EDGE>::BaseConstIterator::operator!=(const PrefixTree<NODE, NODE_DATA, EDGE>::BaseConstIterator& iter) const
	{
		return **iter != *node;
	}

}
