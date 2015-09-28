
namespace Graph
{

	///<summary>
	/// �R���X�g���N�^
	///</summary>
	template <typename NODE, typename NODE_DATA, typename EDGE>
	BaseConstIterator<NODE, NODE_DATA, EDGE>::BaseConstIterator(node_id id, std::shared_ptr<Collection::IdentifiableCollection<node_id, NODE>> node_collection) 
		: node_collection(node_collection)
	{
		if (node_collection != nullptr) node = node_collection->get_by_id(id);
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


	///<summary>
	/// �Q�ƊO��
	///</summary>
	template <typename NODE, typename NODE_DATA, typename EDGE>
	std::shared_ptr<NODE const> BaseConstIterator<NODE, NODE_DATA, EDGE>::operator->() const
	{
		return node;
	}

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
