#pragma once
#include "stdafx.h"
#include "Tree.h"
#include "BaseConstIterator.h"
#include "BasicEdge.h"

namespace Graph
{

	///<summary>
	/// Tree関連のイテレータ(add_childで現状BasicEdgeしか使えない)
	///</summary>
	template <typename NODE, typename NODE_DATA, typename EDGE = Graph::BasicEdge, typename EDGE_DATA = nullptr_t>
	class BaseIterator : public BaseConstIterator<NODE, NODE_DATA, EDGE>
	{		
	public:
		BaseIterator(node_id id, std::shared_ptr<Collection::IdentifiableCollection<node_id, NODE>> node_collection);
		virtual ~BaseIterator();
		std::shared_ptr<NODE> operator*();
		std::shared_ptr<NODE> operator->();
		BaseIterator<NODE, NODE_DATA, EDGE, EDGE_DATA> find_child_if(const std::function<bool(std::shared_ptr<NODE const>)>& compare);
		void add_child(std::shared_ptr<NODE> child, std::shared_ptr<EDGE_DATA> edge_data);
		void add_child(std::shared_ptr<NODE> child, const EDGE_DATA& edge_data);
	};


	///<summary>
	/// EDGE = BasicEdgeの場合の部分特殊化
	///</summary>
	template <typename NODE, typename NODE_DATA>
	class BaseIterator<NODE, NODE_DATA, Graph::BasicEdge, nullptr_t> : public BaseConstIterator<NODE, NODE_DATA, Graph::BasicEdge>
	{
	public:
		BaseIterator(node_id id, std::shared_ptr<Collection::IdentifiableCollection<node_id, NODE>> node_collection);
		virtual ~BaseIterator();
		std::shared_ptr<NODE> operator*();
		std::shared_ptr<NODE> operator->();
		BaseIterator<NODE, NODE_DATA, Graph::BasicEdge, nullptr_t> find_child_if(const std::function<bool(std::shared_ptr<NODE const>)>& compare);
		void add_child(std::shared_ptr<NODE> child);
	};
}

#include "BaseIterator.hpp"