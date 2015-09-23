#pragma once
#include "Identifiable.h"
#include "IdentifiableCollection.h"
#include "Node.h"

namespace Graph
{

	///<summary>
	/// PrefixTreeを表すクラス
	/// 子ノードはIDで昇順に並べられます
	/// NODEクラスにはコンストラクタの引数が(node_id, std::shared_ptr<NODE_DATA>)のもののみ使用可能なので注意
	/// 挿入は末尾に対してのみ可能, 簡単化のため削除には対応しない (ID周りの管理が面倒になるため)
	/// iteratorが参照しているノードに対して直接insert, deleteを行うとID等の管理が行われなくなる可能性があるので注意
	///</summary>
	template <typename NODE, typename NODE_DATA, typename EDGE>
	class PrefixTree
	{
	
	static_assert(std::is_base_of<Node<NODE_DATA, EDGE>, NODE>::value, "template type NODE must be derived from UnorderedTreeNode<NODE_DATA, EDGE>");
	
	#pragma region iterators
	
	class BaseIterator : public std::iterator<std::forward_iterator_tag, NODE> {
	friend class PrefixTree<NODE, NODE_DATA, EDGE>;
	protected:
		std::shared_ptr<NODE> node;
		BaseIterator();
		BaseIterator(std::shared_ptr<NODE> node);
		virtual ~BaseIterator();
	public:
		std::shared_ptr<NODE> operator*();
		bool operator==(const BaseIterator& iter) const;
		bool operator!=(const BaseIterator& iter) const;
	};

	class BaseConstIterator : public std::iterator<std::forward_iterator_tag, NODE> {
	friend class PrefixTree<NODE, NODE_DATA, EDGE>;
	protected:
		std::shared_ptr<NODE const> node;
		BaseConstIterator();
		BaseConstIterator(std::shared_ptr<NODE const> node);
		virtual ~BaseConstIterator();
	public:
		std::shared_ptr<NODE const> operator*() const;
		bool operator==(const BaseConstIterator& iter) const;
		bool operator!=(const BaseConstIterator& iter) const;
	};


	///<summary>
	/// 深さ優先探索用のイテレータ
	///</summary>
	class DepthFirstIterator : public PrefixTree<NODE, NODE_DATA, EDGE>::BaseIterator {
	friend class PrefixTree<NODE, NODE_DATA, EDGE>;
	private:
		DepthFirstIterator();
		DepthFirstIterator(std::shared_ptr<NODE> node);
	public:
		DepthFirstIterator operator++();
	};
	friend class DepthFirstIterator;
	typedef DepthFirstIterator depth_first_iterator;

	///<summary>
	/// 深さ優先探索用のイテレータ
	///</summary>
	class DepthFirstConstIterator : public PrefixTree<NODE, NODE_DATA, EDGE>::BaseConstIterator {
	friend class PrefixTree<NODE, NODE_DATA, EDGE>;
	private:
		DepthFirstConstIterator();
		DepthFirstConstIterator(std::shared_ptr<NODE const> node);
	public:
		DepthFirstConstIterator operator++();
	};
	friend class DepthFirstConstIterator;
	typedef DepthFirstConstIterator depth_first_const_iterator;

	///<summary>
	/// 幅優先探索用のイテレータ
	///</summary>
	class BreadthFirstIterator : public PrefixTree<NODE, NODE_DATA, EDGE>::BaseIterator {
	friend class PrefixTree<NODE, NODE_DATA, EDGE>;
	private:
		BreadthFirstIterator();
		BreadthFirstIterator(std::shared_ptr<NODE> node);
	public:
		BreadthFirstIterator operator++();
		
	};
	friend class BreadthFirstIterator;
	typedef BreadthFirstIterator breadth_first_iterator;

	///<summary>
	/// 幅優先探索用のイテレータ
	///</summary>
	class BreadthFirstConstIterator : public PrefixTree<NODE, NODE_DATA, EDGE>::BaseConstIterator {
	friend class PrefixTree<NODE, NODE_DATA, EDGE>;
	private:
		BreadthFirstConstIterator();
		BreadthFirstConstIterator(std::shared_ptr<NODE const> node);
	public:
		BreadthFirstConstIterator operator++();
	};
	friend class BreadthFirstConstIterator;
	typedef BreadthFirstConstIterator breadth_first_const_iterator;
	
	#pragma endregion イテレータ

	protected:
		std::unique_ptr<Collection::IdentifiableCollection<node_id, NODE>> node_collection;
		std::shared_ptr<NODE const> root_node = nullptr;
		void initialize(std::shared_ptr<NODE_DATA> root_data = nullptr);

	public:
		PrefixTree();
		virtual ~PrefixTree();
		
		depth_first_iterator root();
		depth_first_iterator end();

		depth_first_const_iterator root() const;
		depth_first_const_iterator end() const;
		
		breadth_first_iterator root();
		breadth_first_iterator end();
		
		breadth_first_const_iterator root() const;
		breadth_first_const_iterator end() const;

		bool insert(depth_first_iterator iter, EDGE edge, NODE_DATA node_data);
		bool insert(breadth_first_iterator iter, EDGE edge, NODE_DATA node_data);
	};
}

#include "PrefixTree.hpp"

