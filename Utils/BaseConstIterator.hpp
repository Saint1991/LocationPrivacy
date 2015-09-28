
namespace Graph
{

	///<summary>
	/// �R���X�g���N�^
	///</summary>
	template <typename NODE, typename NODE_DATA, typename EDGE>
	BaseConstIterator<NODE, NODE_DATA, EDGE>::BaseConstIterator() : node(root)
	{

	}


	///<summary>
	/// �R���X�g���N�^
	///</summary>
	template <typename NODE, typename NODE_DATA, typename EDGE>
	BaseConstIterator<NODE, NODE_DATA, EDGE>::BaseConstIterator(std::shared_ptr<NODE const> node) : node(node)
	{

	}


	///<summary>
	/// �f�X�g���N�^
	///</summary>
	template <typename NODE, typename NODE_DATA, typename EDGE>
	BaseConstIterator<NODE, NODE_DATA, EDGE>::~BaseConstIterator()
	{

	}

	///<summary>
	/// �Q�ƊO��
	///</summary>
	template <typename NODE, typename NODE_DATA, typename EDGE>
	std::shared_ptr<NODE const> BaseConstIterator<NODE, NODE_DATA, EDGE>::operator*() const
	{
		return node;
	}

	template <typename 

	///<summary>
	/// �����ɍ����q�v�f���������ăC�e���[�^���擾����
	/// �����ɍ����v�f���Ȃ��ꍇ��nullptr���w���C�e���[�^��Ԃ��܂�
	///</summary>
	template <typename NODE, typename NODE_DATA, typename EDGE>
	BaseConstIterator<NODE, NODE_DATA, EDGE> BaseConstIterator<NODE, NODE_DATA, EDGE>::find_child_if(const std::function<bool(std::shared_ptr<NODE const>)>& compare) const
	{
		node->for_each_edge([&](std::shared_ptr<EDGE const> edge) {
			node_id to = edge->get_to();
			std::shared_ptr<NODE const> node = node_collection->read_by_id(to);
			if (compare(node)) return BaseConstIterator<NODE, NODE_DATA, EDGE>(node);
		});
		return BaseConstIterator<NODE, NODE_DATA, EDGE>(nullptr);
	}

	///<summary>
	/// ��r���Z�q
	///</summary>
	template <typename NODE, typename NODE_DATA, typename EDGE>
	bool BaseConstIterator<NODE, NODE_DATA, EDGE>::operator==(const BaseConstIterator<NODE, NODE_DATA, EDGE>& iter) const
	{
		return **iter == *node;
	}

	///<summary>
	/// ��r���Z�q
	///</summary>
	template <typename NODE, typename NODE_DATA, typename EDGE>
	bool BaseConstIterator<NODE, NODE_DATA, EDGE>::operator!=(const BaseConstIterator<NODE, NODE_DATA, EDGE>& iter) const
	{
		return **iter != *node;
	}

}
