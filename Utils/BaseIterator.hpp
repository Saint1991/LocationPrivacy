
namespace Graph
{

	///<summary>
	/// �R���X�g���N�^
	///</summary>
	template <typename NODE, typename NODE_DATA, typename EDGE, typename EDGE_DATA>
	BaseIterator<NODE, NODE_DATA, EDGE, EDGE_DATA>::BaseIterator(node_id id, std::shared_ptr<Collection::IdentifiableCollection<node_id, NODE>> node_collection) 
		: BaseConstIterator<NODE, NODE_DATA, EDGE>(id, node_collection)
	{

	}

	///<summary>
	/// �f�X�g���N�^
	///</summary>
	template <typename NODE, typename NODE_DATA, typename EDGE, typename EDGE_DATA>
	BaseIterator<NODE, NODE_DATA, EDGE, EDGE_DATA>::~BaseIterator()
	{

	}

	///<summary>
	/// �Q�ƊO��
	///</summary>
	template <typename NODE, typename NODE_DATA, typename EDGE, typename EDGE_DATA>
	std::shared_ptr<NODE> BaseIterator<NODE, NODE_DATA, EDGE, EDGE_DATA>::operator*()
	{
		return node;
	}

	///<summary>
	/// �Q�ƊO��
	///</summary>
	template <typename NODE, typename NODE_DATA, typename EDGE, typename EDGE_DATA>
	std::shared_ptr<NODE> BaseIterator<NODE, NODE_DATA, EDGE, EDGE_DATA>::operator->()
	{
		return node;
	}

	///<summary>
	/// �����ɍ����q�v�f���������ăC�e���[�^���擾����
	/// �����ɍ����v�f���Ȃ��ꍇ��nullptr���w���C�e���[�^��Ԃ��܂�
	///</summary>
	template <typename NODE, typename NODE_DATA, typename EDGE, typename EDGE_DATA>
	BaseIterator<NODE, NODE_DATA, EDGE, EDGE_DATA> BaseIterator<NODE, NODE_DATA, EDGE, EDGE_DATA>::find_child_if(const std::function<bool(std::shared_ptr<NODE const>)>& compare)
	{
		BaseIterator<NODE, NODE_DATA, EDGE, EDGE_DATA> ret(-1, nullptr);
		if (node == nullptr) return ret;
		node->for_each_edge([&](std::shared_ptr<EDGE const> edge) {
			node_id to = edge->get_to();
			std::shared_ptr<NODE const> child_node = node_collection->read_by_id(to);
			if (compare(child_node)) ret = BaseIterator<NODE, NODE_DATA, EDGE, EDGE_DATA>(to, node_collection);
		});
		return ret;
	}


	///<summary>
	/// �C�e���[�^���w���Ă���v�f�̎q�v�f�ɑ΂��āCchild��ǉ�����
	/// edge_data�͒ǉ�����m�[�h���w���G�b�W�ɕێ������
	///</summary>
	template <typename NODE, typename NODE_DATA, typename EDGE, typename EDGE_DATA>
	void BaseIterator<NODE, NODE_DATA, EDGE, EDGE_DATA>::add_child(std::shared_ptr<NODE> child, std::shared_ptr<EDGE_DATA> edge_data)
	{
		Graph::node_id child_id = child->get_id();
		if (!node_collection->contains(child_id)) {
			node_collection->add(child);
		}
		if (!node->is_connecting_to(child_id)) {
			node->connect_to(std::make_shared<EDGE>(child->get_id(), edge_data));
		}
	}

	///<summary>
	/// �C�e���[�^���w���Ă���v�f�̎q�v�f�ɑ΂��āCchild��ǉ�����
	/// edge_data�͒ǉ�����m�[�h���w���G�b�W�ɕێ������
	///</summary>
	template <typename NODE, typename NODE_DATA, typename EDGE, typename EDGE_DATA>
	void BaseIterator<NODE, NODE_DATA, EDGE, EDGE_DATA>::add_child(std::shared_ptr<NODE> child, const EDGE_DATA& edge_data)
	{
		add_child(child, std::make_shared<EDGE_DATA>(edge_data));
	}

	#pragma region BasicEdge
	///<summary>
	/// �R���X�g���N�^
	///</summary>
	template <typename NODE, typename NODE_DATA>
	BaseIterator<NODE, NODE_DATA, Graph::BasicEdge, nullptr_t>::BaseIterator(node_id id, std::shared_ptr<Collection::IdentifiableCollection<node_id, NODE>> node_collection)
		: BaseConstIterator<NODE, NODE_DATA, Graph::BasicEdge>(id, node_collection)
	{

	}

	///<summary>
	/// �f�X�g���N�^
	///</summary>
	template <typename NODE, typename NODE_DATA>
	BaseIterator<NODE, NODE_DATA, Graph::BasicEdge, nullptr_t>::~BaseIterator()
	{

	}

	///<summary>
	/// �Q�ƊO��
	///</summary>
	template <typename NODE, typename NODE_DATA>
	std::shared_ptr<NODE> BaseIterator<NODE, NODE_DATA, Graph::BasicEdge, nullptr_t>::operator*()
	{
		return node;
	}

	///<summary>
	/// �Q�ƊO��
	///</summary>
	template <typename NODE, typename NODE_DATA>
	std::shared_ptr<NODE> BaseIterator<NODE, NODE_DATA, Graph::BasicEdge, nullptr_t>::operator->()
	{
		return node;
	}

	///<summary>
	/// �����ɍ����q�v�f���������ăC�e���[�^���擾����
	/// �����ɍ����v�f���Ȃ��ꍇ��nullptr���w���C�e���[�^��Ԃ��܂�
	///</summary>
	template <typename NODE, typename NODE_DATA>
	BaseIterator<NODE, NODE_DATA, Graph::BasicEdge, nullptr_t> BaseIterator<NODE, NODE_DATA, Graph::BasicEdge, nullptr_t>::find_child_if(const std::function<bool(std::shared_ptr<NODE const>)>& compare)
	{
		BaseIterator<NODE, NODE_DATA, Graph::BasicEdge> ret(-1, nullptr);
		if (node == nullptr) return ret;
		node->for_each_edge([&](std::shared_ptr<Graph::BasicEdge const> edge) {
			node_id to = edge->get_to();
			std::shared_ptr<NODE const> child_node = node_collection->read_by_id(to);
			if (compare(child_node)) ret = BaseIterator<NODE, NODE_DATA, Graph::BasicEdge>(to, node_collection);
		});
		return ret;
	}


	///<summary>
	/// �C�e���[�^���w���Ă���v�f�̎q�v�f�ɑ΂��āCchild��ǉ�����
	///</summary>
	template <typename NODE, typename NODE_DATA>
	void BaseIterator<NODE, NODE_DATA, Graph::BasicEdge, nullptr_t>::add_child(std::shared_ptr<NODE> child)
	{
		Graph::node_id child_id = child->get_id();
		if (!node_collection->contains(child_id)) {
			node_collection->add(child);
		}
		if (!node->is_connecting_to(child_id)) {
			node->connect_to(std::make_shared<Graph::BasicEdge>(child->get_id()));
		}
	}

	#pragma endregion BasicEdge�ɂ��Ă̕������ꉻ
}

