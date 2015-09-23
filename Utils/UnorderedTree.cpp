#include "stdafx.h"
#include "UnorderedTree.h"

namespace Graph
{

	///<summary>
	/// �R���X�g���N�^
	///</summary>
	template <typename NODE, typename NODE_DATA, typename EDGE>
	UnorderedTree<NODE, NODE_DATA, EDGE>::UnorderedTree()
	{
	}


	///<summary>
	/// �f�X�g���N�^
	///</summary>
	template <typename NODE, typename NODE_DATA, typename EDGE>
	UnorderedTree<NODE, NODE_DATA, EDGE>::~UnorderedTree()
	{
	}


	///<summary>
	/// ���[�g�m�[�h��ݒ肵�Ė؂����������܂�
	///</summary>
	template <typename NODE, typename NODE_DATA, typename EDGE>
	void UnorderedTree<NODE, NODE_DATA, EDGE>::initialize(NODE node)
	{
		std::shared_ptr<NODE> root_ptr = std::make_shared<NODE>(node);
		root_node = root_ptr;
		node_collection->add(root_node);
		connected_ids.insert(node.get_id());
	}


	///<summary>
	/// �C�e���[�^�������Ă���m�[�h�̎q�m�[�h�Ƃ���
	///</summary>
	template <typename NODE, typename NODE_DATA, typename EDGE>
	void UnorderedTree<NODE, NODE_DATA, EDGE>::insert(depth_first_iterator iter, NODE node)
	{
		
	}
}

