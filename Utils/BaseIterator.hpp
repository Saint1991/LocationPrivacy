
namespace Graph
{

	///<summary>
	/// �R���X�g���N�^
	///</summary>
	template <typename NODE, typename NODE_DATA, typename EDGE>
	BaseIterator<NODE, NODE_DATA, EDGE>::BaseIterator(node_id id, std::shared_ptr<Collection::IdentifiableCollection<node_id, NODE>> node_collection) 
		: BaseConstIterator<NODE, NODE_DATA, EDGE>(id, node_collection)
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
	/// �Q�ƊO��
	///</summary>
	template <typename NODE, typename NODE_DATA, typename EDGE>
	std::shared_ptr<NODE> BaseIterator<NODE, NODE_DATA, EDGE>::operator->()
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
		BaseIterator<NODE, NODE_DATA, EDGE> ret(-1, nullptr);
		if (node == nullptr) return ret;
		node->for_each_edge([&](std::shared_ptr<EDGE const> edge) {
			node_id to = edge->get_to();
			std::shared_ptr<NODE const> child_node = node_collection->read_by_id(to);
			if (compare(child_node)) ret = BaseIterator<NODE, NODE_DATA, EDGE>(to, node_collection);
		});
		return ret;
	}


	///<summary>
	/// �C�e���[�^���w���Ă���v�f�̎q�v�f�ɑ΂��āCchild��ǉ�����
	///</summary>
	template <typename NODE, typename NODE_DATA, typename EDGE>
	void BaseIterator<NODE, NODE_DATA, EDGE>::add_child(std::shared_ptr<NODE> child)
	{
		node_collection->add(child);
		node->connect_to(std::make_shared<Graph::BasicEdge>(child->get_id()));
	}
}

