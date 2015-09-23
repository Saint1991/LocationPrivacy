#include "stdafx.h"
#include "UnorderedTree.h"

namespace Graph
{

	///<summary>
	/// �R���X�g���N�^
	///</summary>
	template <typename ID_TYPE, typename NODE, typename EDGE>
	UnorderedTree<ID_TYPE, NODE, EDGE>::UnorderedTree()
	{
	}


	///<summary>
	/// �f�X�g���N�^
	///</summary>
	template <typename ID_TYPE, typename NODE, typename EDGE>
	UnorderedTree<ID_TYPE, NODE, EDGE>::~UnorderedTree()
	{
	}


	///<summary>
	/// ���[�g�m�[�h��ݒ肵�Ė؂����������܂�
	///</summary>
	template <typename ID_TYPE, typename NODE, typename EDGE>
	void UnorderedTree<ID_TYPE, NODE, EDGE>::initialize(NODE node)
	{
		std::shared_ptr<NODE> root_ptr = std::make_shared<NODE>(node);
		root_node = root_ptr;
		node_collection->add(root_node);
		connected_ids.insert(node.get_id());
	}


	///<summary>
	/// �C�e���[�^�������Ă���m�[�h�̎q�m�[�h�Ƃ���
	///</summary>
	template <typename ID_TYPE, typename NODE, typename EDGE>
	void UnorderedTree<ID_TYPE, NODE, EDGE>::insert(depth_first_iterator iter, NODE node)
	{
		
	}
}

