#pragma once
#include "Identifiable.h"
#include "BasicEdge.h"

namespace Graph
{

	///<summary>
	/// UnorderedTreeで用いるノード
	/// IDはstring
	/// 子ノードはID順にソートして保持
	///</summary>
	template <typename DATA_TYPE, typename EDGE>
	class UnorderedTreeNode : public Identifiable<node_id>
	{

	static_assert(std::is_base_of<BasicEdge, EDGE>::value, "template type EDGE must be derived from BasicEdge");

	protected:
		std::shared_ptr<	std::set<std::shared_ptr<EDGE>, std::function<bool(std::shared_ptr<EDGE>, std::shared_ptr<EDGE>)>> > edge_list;
	
	public:

		std::shared_ptr<DATA_TYPE> data;

		UnorderedTreeNode(node_id id, std::shared_ptr<DATA_TYPE> data);
		UnorderedTreeNode(const UnorderedTreeNode<DATA_TYPE, EDGE>& node);
		virtual ~UnorderedTreeNode();

		bool add_child(EDGE to_child);

		std::shared_ptr<EDGE const> get_static_edge_to(const node_id& child_id) const;
		std::shared_ptr<EDGE> get_edge_to(const node_id& child_id);

		bool has_child_of(const node_id& target_id) const;
		std::vector<node_id> get_child_ids() const;
		
	};
}

#include "UnorderedTreeNode.hpp"


