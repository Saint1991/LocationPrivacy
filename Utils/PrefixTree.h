#pragma once
#include "Identifiable.h"
#include "IdentifiableCollection.h"
#include "Node.h"
#include "BaseIterator.h"
#include "BaseConstIterator.h"
#include "DepthFirstIterator.h"
#include "DepthFirstConstIterator.h"
#include "BreadthFirstIterator.h"
#include "BreadthFirstConstIterator.h"

namespace Graph
{

	///<summary>
	/// PrefixTree��\���N���X
	/// �q�m�[�h��ID�ŏ����ɕ��ׂ��܂�
	/// NODE�N���X�ɂ̓R���X�g���N�^�̈�����(node_id, NODE��shared_ptr)�̂��̂̂ݎg�p�\�Ȃ̂Œ���
	/// �}���͖����ɑ΂��Ă̂݉\, �ȒP���̂��ߍ폜�ɂ͑Ή����Ȃ� (ID����̊Ǘ����ʓ|�ɂȂ邽��)
	/// iterator���Q�Ƃ��Ă���m�[�h�ɑ΂��Ē���insert, delete���s����ID���̊Ǘ����s���Ȃ��Ȃ�\��������̂Œ���
	///</summary>
	template <typename NODE, typename NODE_DATA, typename EDGE>
	class PrefixTree
	{
	static_assert(std::is_base_of<Node<NODE_DATA, EDGE>, NODE>::value, "template type NODE must be derived from Node<NODE_DATA, EDGE>");

	protected:
		std::unique_ptr<Collection::IdentifiableCollection<node_id, NODE>> node_collection;
		std::shared_ptr<NODE> root_node = nullptr;
		void initialize(std::shared_ptr<NODE_DATA> root_data = nullptr);

	public:

		#pragma region Iterators
		typedef BaseIterator<NODE, NODE_DATA, EDGE> base_iterator;
		typedef BaseConstIterator<NODE, NODE_DATA, EDGE> base_const_iterator;
		typedef DepthFirstIterator<NODE, NODE_DATA, EDGE> depth_first_iterator;
		typedef DepthFirstConstIterator<NODE, NODE_DATA, EDGE> depth_first_const_iterator;
		typedef BreadthFirstIterator<NODE, NODE_DATA, EDGE> breadth_first_iterator;
		typedef BreadthFirstConstIterator<NODE, NODE_DATA, EDGE> breadth_first_const_iterator;
		#pragma endregion
		
		PrefixTree();
		virtual ~PrefixTree();

		template <typename ITER_TYPE> ITER_TYPE root();
		template <typename ITER_TYPE> const ITER_TYPE root() const;
		template <typename ITER_TYPE> ITER_TYPE end();
		template <typename ITER_TYPE> const ITER_TYPE end() const;
		template <typename ITER_TYPE> ITER_TYPE get_iter_by_id(node_id id);
		template <typename ITER_TYPE> const ITER_TYPE get_iter_by_id(node_id id) const;

		bool insert(base_iterator iter, EDGE edge, NODE_DATA node_data);
	};
}

#include "PrefixTree.hpp"