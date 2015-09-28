
namespace Graph
{

	///<summary>
	/// �R���X�g���N�^
	///</summary>
	template <typename NODE, typename NODE_DATA, typename EDGE>
	BaseIterator<NODE, NODE_DATA, EDGE>::BaseIterator() : node(nullptr)
	{

	}


	///<summary>
	/// �R���X�g���N�^
	///</summary>
	template <typename NODE, typename NODE_DATA, typename EDGE>
	BaseIterator<NODE, NODE_DATA, EDGE>::BaseIterator(std::shared_ptr<NODE> node) : node(node)
	{

	}


	///<summary>
	/// �f�X�g���N�^
	///</summary>
	template <typename NODE, typename NODE_DATA, typename EDGE>
	BaseIterator<NODE, NODE_DATA, EDGE>::~BaseIterator()
	{

	}

	///<summary>
	/// �Q�ƊO��
	///</summary>
	template <typename NODE, typename NODE_DATA, typename EDGE>
	std::shared_ptr<NODE> BaseIterator<NODE, NODE_DATA, EDGE>::operator*()
	{
		return node;
	}

	///<summary>
	/// �����ɍ����q�v�f���������ăC�e���[�^���擾����
	/// �����ɍ����v�f���Ȃ��ꍇ��nullptr���w���C�e���[�^��Ԃ��܂�
	///</summary>
	template <typename NODE, typename NODE_DATA, typename EDGE>
	BaseIterator<NODE, NODE_DATA, EDGE> BaseIterator<NODE, NODE_DATA, EDGE>::find_child_if(const std::function<bool(std::shared_ptr<NODE const>)>& compare)
	{
		node->for_each_edge([&](std::shared_ptr<EDGE const> edge) {
			node_id to = edge->get_to();
			std::shared_ptr<NODE> node = node_collection->get_by_id(to);
			if (compare(node)) return base_iterator(node);
		});
		return base_iterator(nullptr);
	}

	
	///<summary>
	/// ��r���Z�q
	///</summary>
	template <typename NODE, typename NODE_DATA, typename EDGE>
	bool BaseIterator<NODE, NODE_DATA, EDGE>::operator==(const BaseIterator<NODE, NODE_DATA, EDGE>& iter) const
	{
		return **iter == *node;
	}
	

	///<summary>
	/// ��r���Z�q
	///</summary>
	template <typename NODE, typename NODE_DATA, typename EDGE>
	bool BaseIterator<NODE, NODE_DATA, EDGE>::operator!=(const BaseIterator<NODE, NODE_DATA, EDGE>& iter) const
	{
		return **iter != *node;
	}
	
}

