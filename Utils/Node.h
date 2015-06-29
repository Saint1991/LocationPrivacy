#ifdef UTILS_EXPORTS
#define NODE_API __declspec(dllexport)
#else
#define NODE_API __declspec(dllimport)
#endif

#pragma once
#include "Edge.h"

namespace Graph {

	template <typename NODE_DATA, typename EDGE_DATA>
	class Node
	{
	protected:
		node_id id;
		std::unique_ptr<std::list<std::shared_ptr<Edge<EDGE_DATA>>>> list;
	public:
		std::shared_ptr<NODE_DATA> data;
		NODE_API Node(node_id id, NODE_DATA data);
		NODE_API virtual ~Node();

		NODE_API void connect_to(Graph::Edge<EDGE_DATA> link_to);
		NODE_API void connect_to(node_id to, EDGE_DATA data);
		NODE_API bool disconnect_from(node_id from);
		NODE_API std::shared_ptr<Graph::Edge<EDGE_DATA>> get_edge_to(node_id id);
		NODE_API bool is_connecting_to(node_id id);
		NODE_API std::list<node_id> get_connecting_node_list();
	};
}