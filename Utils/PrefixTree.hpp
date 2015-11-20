
namespace Graph
{

	///<summary>
	/// �R���X�g���N�^
	///</summary>
	template <typename NODE, typename NODE_DATA, typename EDGE>
	PrefixTree<NODE, NODE_DATA, EDGE>::PrefixTree() : node_collection(std::make_shared<Collection::IdentifiableCollection<node_id, NODE>>())
	{
	}


	///<summary>
	/// �f�X�g���N�^
	///</summary>
	template <typename NODE, typename NODE_DATA, typename EDGE>
	PrefixTree<NODE, NODE_DATA, EDGE>::~PrefixTree()
	{

	}


	///<summary>
	/// ���[�g�m�[�h��ݒ肵�Ė؂����������܂�
	/// ���[�g�m�[�h�̐e�m�[�h��ID-1�Ƃ��Ă���
	///</summary>
	template <typename NODE, typename NODE_DATA, typename EDGE>
	void PrefixTree<NODE, NODE_DATA, EDGE>::initialize(std::shared_ptr<NODE_DATA> root_data)
	{
		node_id current_id = node_collection->size();
		std::shared_ptr<NODE> node = std::make_shared<NODE>(current_id, -1, root_data);
		root_node = node;
		node_collection->add(node);
	}


	///<summary>
	/// root�m�[�h���w���Ă����ԂŃC�e���[�^���擾����
	///</summary>
	template <typename NODE, typename NODE_DATA, typename EDGE>
	template <typename ITER_TYPE>
	ITER_TYPE PrefixTree<NODE, NODE_DATA, EDGE>::root()
	{
		ITER_TYPE ret(root_node->get_id(), node_collection);
		return ret;
	}


	///<summary>
	/// �I�[�m�[�h(nullptr)���w���Ă����ԂŃC�e���[�^���擾����
	///</summary>
	template <typename NODE, typename NODE_DATA, typename EDGE>
	template <typename ITER_TYPE>
	ITER_TYPE PrefixTree<NODE, NODE_DATA, EDGE>::end()
	{
		ITER_TYPE ret(INVALID, nullptr);
		return ret;
	}


	///<summary>
	/// �w�肵��ID�����m�[�h���w���ď�ԂŃC�e���[�^���擾����
	///</summary>
	template <typename NODE, typename NODE_DATA, typename EDGE>
	template <typename ITER_TYPE>
	ITER_TYPE PrefixTree<NODE, NODE_DATA, EDGE>::get_iter_by_id(node_id id)
	{
		ITER_TYPE ret(id, node_collection);
		return ret;
	}

		
	///<summary>
	/// �C�e���[�^���w���Ă���v�f�̎q�v�f�Ƃ��ăf�[�^��node_data�̃m�[�h��}����edge�𒣂�
	/// Insert�����m�[�h��Ԃ��܂�
	/// �ǉ��Ɏ��s�����ꍇ��nullptr��Ԃ��܂�
	///</summary>
	template <typename NODE, typename NODE_DATA, typename EDGE>
	BaseIterator<NODE, NODE_DATA, EDGE> PrefixTree<NODE, NODE_DATA, EDGE>::insert(base_iterator iter, EDGE edge, NODE_DATA node_data)
	{
		std::shared_ptr<NODE> target = *iter;
		if (target == nullptr) return base_iterator(INVALID, nullptr);
		bool result = target->connect_to(std::make_shared<EDGE>(edge));

		node_id current_id = node_collection->size();
		std::shared_ptr<NODE> node = std::make_shared<NODE>(current_id, node_data);
		result = node_collection->add(node) && result;
		return result ? base_iterator(current_id, node_collection) : base_iterator(INVALID, nullptr);
	}
}