#pragma once
#include "Identifiable.h"
#include "IdentifiableCollection.h"
#include "UnorderedTreeNode.h"

namespace Graph
{

	///<summary>
	/// 木構造を表すクラス
	/// 子ノードはIDで昇順に並べられます
	///</summary>
	template <typename NODE, typename NODE_DATA, typename EDGE>
	class UnorderedTree
	{
	
	static_assert(std::is_base_of<UnorderedTreeNode<NODE_DATA, EDGE>, NODE>::value, "template type NODE must be derived from UnorderedTreeNode<NODE_DATA, EDGE>");
	
	#pragma region iterators
	
	///<summary>
	/// 深さ優先探索用のイテレータ
	///</summary>
	class DepthFirstIterator : public std::iterator<std::forward_iterator_tag, NODE> {
	
	friend class UnorderedTree<NODE, NODE_DATA, EDGE>;
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
	/// 深さ優先探索用のイテレータ
	///</summary>
	class DepthFirstConstIterator : public std::iterator<std::forward_iterator_tag, NODE> {
	
		friend class UnorderedTree<NODE, NODE_DATA, EDGE>;
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
	/// 幅優先探索用のイテレータ
	///</summary>
	class BreadthFirstIterator : public std::iterator<std::forward_iterator_tag, NODE> {
	
		friend class UnorderedTree<NODE, NODE_DATA, EDGE>;
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
	/// 幅優先探索用のイテレータ
	///</summary>
	class BreadthFirstConstIterator : public std::iterator<std::forward_iterator_tag, NODE> {
	
		friend class UnorderedTree<NODE, NODE_DATA, EDGE>;
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
	
	#pragma endregion イテレータ

	private:
		std::set<node_id> connected_ids;

	protected:
		std::unique_ptr<Collection::IdentifiableCollection<node_id, NODE>> node_collection;
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


