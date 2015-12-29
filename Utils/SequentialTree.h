#pragma once
#include "stdafx.h"
#include "SequentialTreeNode.h"
#include "Sequence.h"
#include "Tree.h"

namespace Graph
{
	template <typename NODE, typename DATA>
	class SequentialTree : public Tree<NODE, DATA, BasicEdge, nullptr_t>
	{
	static_assert(std::is_base_of<SequentialTreeNode<DATA>, NODE>::value, "Template type NODE must be derived from SequentialTreeNode");
	public:
		SequentialTree(std::shared_ptr<DATA> root_data = nullptr);
		~SequentialTree();

		virtual void initialize(std::shared_ptr<NODE> root_node);

		std::vector<std::shared_ptr<NODE const>> get_all_leaves() const;
		std::vector<std::shared_ptr<NODE const>> get_all_nodes_in_depth(int depth) const;
		int max_depth() const;

		Collection::Sequence<std::shared_ptr<NODE const>> get_sequence_by_node(std::shared_ptr<NODE const> node) const;
		void for_each_sequence(const std::function<void(const Collection::Sequence<std::shared_ptr<NODE const>>)>& execute_function) const;
		void for_each_sequence(size_t length, const std::function<void(const Collection::Sequence<std::shared_ptr<NODE const>>)>& execute_function) const;
	};
}

#include "SequentialTree.hpp"



