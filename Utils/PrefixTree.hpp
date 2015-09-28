
namespace Graph
{

	///<summary>
	/// �R���X�g���N�^
	///</summary>
	template <typename NODE, typename NODE_DATA, typename EDGE>
	PrefixTree<NODE, NODE_DATA, EDGE>::PrefixTree()
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
	///</summary>
	template <typename NODE, typename NODE_DATA, typename EDGE>
	void PrefixTree<NODE, NODE_DATA, EDGE>::initialize(std::shared_ptr<NODE_DATA> root_data)
	{
		node_id current_id = node_collection->size();
		std::shared_ptr<NODE> node = std::make_shared<NODE>(current_id, root_data);
		root_node = node;
		node_collection->add(node);
	}

	///<summary>
	/// root�m�[�h���w���C�e���[�^���擾
	///</summary>
	template <typename NODE, typename NODE_DATA, typename EDGE>
	template <typename ITER_TYPE>
	ITER_TYPE PrefixTree<NODE, NODE_DATA, EDGE>::root()
	{
		ITER_TYPE ret(root_node);
		return ret;
	}

	///<summary>
	/// root�m�[�h���w���C�e���[�^���擾
	///</summary>
	template <typename NODE, typename NODE_DATA, typename EDGE>
	template <typename ITER_TYPE>
	const ITER_TYPE PrefixTree<NODE, NODE_DATA, EDGE>::root() const
	{
		const ITER_TYPE ret(root_node);
		return ret;
	}

	///<summary>
	/// �I�[(nullptr)���w���C�e���[�^��Ԃ�
	///</summary>
	template <typename NODE, typename NODE_DATA, typename EDGE>
	template <typename ITER_TYPE>
	ITER_TYPE PrefixTree<NODE, NODE_DATA, EDGE>::end()
	{
		ITER_TYPE ret(nullptr);
		return ret;
	}

	///<summary>
	/// �I�[(nullptr)���w���C�e���[�^��Ԃ�
	///</summary>
	template <typename NODE, typename NODE_DATA, typename EDGE>
	template <typename ITER_TYPE>
	const ITER_TYPE  PrefixTree<NODE, NODE_DATA, EDGE>::end() const
	{
		const ITER_TYPE ret(nullptr);
		return ret;
	}


	///<summary>
	/// �w�肵��ID�����m�[�h���w���C�e���[�^���擾����
	///</summary>
	template <typename NODE, typename NODE_DATA, typename EDGE>
	template <typename ITER_TYPE>
	ITER_TYPE PrefixTree<NODE, NODE_DATA, EDGE>::get_iter_by_id(node_id id)
	{
		std::shared_ptr<NODE> node = node_collection->get_by_id(id);
		ITER_TYPE ret(node);
		return ret;
	}

	///<summary>
	/// �w�肵��ID�����m�[�h���w���C�e���[�^���擾����
	///</summary>
	template <typename NODE, typename NODE_DATA, typename EDGE>
	template <typename ITER_TYPE>
	const ITER_TYPE PrefixTree<NODE, NODE_DATA, EDGE>::get_iter_by_id(node_id id) const
	{
		std::shared_ptr<NODE const> node = node_collection->read_by_id(id);
		const ITER_TYPE ret(node);
		return ret;
	}

	///<summary>
	/// �C�e���[�^���w���Ă���m�[�h�̎q�m�[�h�Ƃ��ăf�[�^node_data�����m�[�h��}����edge�𒣂�܂��D
	///</summary>
	template <typename NODE, typename NODE_DATA, typename EDGE>
	bool Graph::PrefixTree<NODE, NODE_DATA, EDGE>::insert(base_iterator iter, EDGE edge, NODE_DATA node_data)
	{
		std::shared_ptr<NODE> target = *iter;
		if (target == nullptr) return false;
		bool result = target->connect_to(std::make_shared<EDGE>(edge));

		node_id current_id = node_collection->size();
		result = node_collection->add(std::make_shared<NODE>(current_id, node_data)) && result;
		return result;
	}
}

