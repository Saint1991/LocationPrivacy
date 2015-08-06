#pragma once
#include "Edge.h"
#include "Identifiable.h"

namespace Graph 
{

	///<summary>
	/// グラフにおけるノードを表すテンプレートクラス
	///</summary>
	template <typename NODE_DATA, typename EDGE_DATA>
	class Node : public Identifiable
	{
	protected:
		std::unique_ptr<std::list<std::shared_ptr<Edge<EDGE_DATA>>>> edge_list;
		std::shared_ptr<Graph::Edge<EDGE_DATA>> get_edge_to(node_id id) const;

	public:
		std::shared_ptr<NODE_DATA> data;
		
		Node(node_id id, NODE_DATA data);
		Node(node_id id, std::shared_ptr<NODE_DATA> data);
		Node(const Node& node);
		virtual ~Node();

		bool connect_to(node_id to, EDGE_DATA data);
		bool disconnect_from(node_id from);

		std::shared_ptr<EDGE_DATA const> get_edge_data(node_id to) const;
		bool update_edge_data(node_id id, EDGE_DATA data);
		bool connect_or_update(node_id id, EDGE_DATA data);
		
		bool is_connecting_to(node_id id) const;
		std::list<node_id> get_connecting_node_list() const;

		bool operator==(Graph::Node<NODE_DATA, EDGE_DATA> node) const;
		bool operator!=(Graph::Node<NODE_DATA, EDGE_DATA> node) const;
	};
}

#include "Node.hpp"