#pragma once
#include "Identifiable.h"
#include "IdentifiableCollection.h"

namespace Graph
{

	///<summary>
	/// �؍\����\���N���X
	/// �q�m�[�h��ID�ŏ����ɕ��ׂ��܂�
	///</summary>
	template <typename ID_TYPE, typename NODE, typename EDGE>
	class UnorderedTree
	{
	
	static_assert(std::is_base_of<Identifiable<ID_TYPE>, NODE>::value, "template type NODE must be derived from Identifiable<ID_TYPE>");
	
	#pragma region iterators
	
	///<summary>
	/// �[���D��T���p�̃C�e���[�^
	///</summary>
	class DepthFirstIterator : public std::iterator<std::forward_iterator_tag, NODE> {
	
	friend class UnorderedTree<ID_TYPE, NODE, EDGE>;
	private:
		std::shared_ptr<NODE> node;
		DepthFirstIterator();
		DepthFirstIterator(std::shared_ptr<NODE> node);
	public:
		DepthFirstIterator operator++();
		std::shared_ptr<NODE> operator*();
		bool operator==(DepthFirstIterator iter);
	};
	friend class DepthFirstIterator;
	typedef DepthFirstIterator depth_first_iterator;

	///<summary>
	/// �[���D��T���p�̃C�e���[�^
	///</summary>
	class DepthFirstConstIterator : public std::iterator<std::forward_iterator_tag, NODE> {
	
	friend class UnorderedTree<ID_TYPE, NODE, EDGE>;
	private:
		std::shared_ptr<NODE const> node;
		DepthFirstConstIterator();
		DepthFirstConstIterator(std::shared_ptr<NODE const> node);
	public:
		DepthFirstConstIterator operator++();
		std::shared_ptr<NODE const> operator*() const;
	};
	friend class DepthFirstConstIterator;
	typedef DepthFirstConstIterator depth_first_const_iterator;

	///<summary>
	/// ���D��T���p�̃C�e���[�^
	///</summary>
	class BreadthFirstIterator : public std::iterator<std::forward_iterator_tag, NODE> {
	
	friend class UnorderedTree<ID_TYPE, NODE, EDGE>;
	private:
		std::shared_ptr<NODE> node;
		BreadthFirstIterator();
		BreadthFirstIterator(std::shared_ptr<NODE> node);
	public:
		BreadthFirstIterator operator++();
		std::shared_ptr<NODE> operator*();
	};
	friend class BreadthFirstIterator;
	typedef BreadthFirstIterator breadth_first_iterator;

	///<summary>
	/// ���D��T���p�̃C�e���[�^
	///</summary>
	class BreadthFirstConstIterator : public std::iterator<std::forward_iterator_tag, NODE> {
	
	friend class UnorderedTree<ID_TYPE, NODE, EDGE>;
	private:
		std::shared_ptr<NODE const> node;
		BreadthFirstConstIterator();
		BreadthFirstConstIterator(std::shared_ptr<NODE const> node);
	public:
		BreadthFirstConstIterator operator++();
		std::shared_ptr<NODE const> operator*() const;
	};
	friend class BreadthFirstConstIterator;
	typedef BreadthFirstConstIterator breadth_first_const_iterator;
	
	#pragma endregion �C�e���[�^

	private:
		std::set<ID_TYPE> connected_ids;

	protected:
		std::unique_ptr<Collection::IdentifiableCollection<ID_TYPE, NODE>> node_collection;
		std::shared_ptr<NODE const> root_node = nullptr;
		void initialize(NODE root);

	public:
		UnorderedTree();
		virtual ~UnorderedTree();
		
		depth_first_iterator root();
		depth_first_iterator end();

		depth_first_const_iterator root() const;
		depth_first_const_iterator end() const;
		
		breadth_first_iterator root();
		breadth_first_iterator end();
		
		breadth_first_const_iterator root() const;
		breadth_first_const_iterator end() const;

		void insert(depth_first_iterator iter, NODE node);
		void insert(breadth_first_iterator iter, NODE node);
	};
}


