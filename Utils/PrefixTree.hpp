
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
	/// �C�e���[�^���w���Ă���m�[�h�̎q�m�[�h�Ƃ��ăf�[�^node_data�����m�[�h��}����edge�𒣂�܂��D
	///</summary>
	template <typename NODE, typename NODE_DATA, typename EDGE>
	bool PrefixTree<NODE, NODE_DATA, EDGE>::insert(depth_first_iterator iter, EDGE edge, NODE_DATA node_data)
	{
		std::shared_ptr<NODE> target = *iter;
		if (target == nullptr) return false;
		bool result = target->connect_to(std::make_shared<EDGE>(edge));

		node_id current_id = node_collection->size();
		result = node_collection->add(std::make_shared<NODE>(current_id, node_data)) && result;
		return result;
	}

	///<summary>
	/// �C�e���[�^���w���Ă���m�[�h�̎q�m�[�h�Ƃ��ăf�[�^node_data�����m�[�h��}����edge�𒣂�܂��D
	///</summary>
	template <typename NODE, typename NODE_DATA, typename EDGE>
	bool PrefixTree<NODE, NODE_DATA, EDGE>::insert(breadth_first_iterator iter, EDGE edge, NODE_DATA node_Data)
	{
		std::shared_ptr<NODE> target = *iter;
		if (target == nullptr) return false;
		bool result = target->connect_to(std::make_shared<EDGE>(edge));

		node_id current_id = node_collection->size();
		result = node_collection->add(std::make_shared<NODE>(current_id, node_data)) && result;
		return result;
	}


#pragma region iterators

#pragma region BaseIterator

	///<summary>
	/// �R���X�g���N�^
	///</summary>
	template <typename NODE, typename NODE_DATA, typename EDGE>
	PrefixTree<NODE, NODE_DATA, EDGE>::BaseIterator::BaseIterator() : node(root)
	{

	}


	///<summary>
	/// �R���X�g���N�^
	///</summary>
	template <typename NODE, typename NODE_DATA, typename EDGE>
	PrefixTree<NODE, NODE_DATA, EDGE>::BaseIterator::BaseIterator(std::shared_ptr<NODE> node) : node(node)
	{

	}


	///<summary>
	/// �f�X�g���N�^
	///</summary>
	template <typename NODE, typename NODE_DATA, typename EDGE>
	PrefixTree<NODE, NODE_DATA, EDGE>::BaseIterator::~BaseIterator()
	{

	}

	///<summary>
	/// �Q�ƊO��
	///</summary>
	template <typename NODE, typename NODE_DATA, typename EDGE>
	std::shared_ptr<NODE> PrefixTree<NODE, NODE_DATA, EDGE>::BaseIterator::operator*()
	{
		return node;
	}


	///<summary>
	/// ��r���Z�q
	///</summary>
	template <typename NODE, typename NODE_DATA, typename EDGE>
	bool PrefixTree<NODE, NODE_DATA, EDGE>::BaseIterator::operator==(const PrefixTree<NODE, NODE_DATA, EDGE>::BaseIterator& iter) const
	{
		return **iter == *node;
	}

	///<summary>
	/// ��r���Z�q
	///</summary>
	template <typename NODE, typename NODE_DATA, typename EDGE>
	bool PrefixTree<NODE, NODE_DATA, EDGE>::BaseIterator::operator!=(const PrefixTree<NODE, NODE_DATA, EDGE>::BaseIterator& iter) const
	{
		return **iter != *node;
	}


#pragma endregion

#pragma region BaseConstIterator

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

#pragma endregion


#pragma region DepthFirstIterator

	///<summary>
	/// �R���X�g���N�^ 
	///</summary>
	template <typename NODE, typename NODE_DATA, typename EDGE>
	PrefixTree<NODE, NODE_DATA, EDGE>::DepthFirstIterator::DepthFirstIterator() : PrefixTree<NODE, NODE_DATA, EDGE>::BaseIterator(root_node)
	{

	}


	///<summary>
	/// �R���X�g���N�^
	///</summary>
	template <typename NODE, typename NODE_DATA, typename EDGE>
	PrefixTree<NODE, NODE_DATA, EDGE>::DepthFirstIterator::DepthFirstIterator(std::shared_ptr<NODE> node) : PrefixTree<NODE, NODE_DATA, EDGE>::BaseIterator(node)
	{

	}


#pragma endregion 

#pragma endregion �C�e���[�^
}

