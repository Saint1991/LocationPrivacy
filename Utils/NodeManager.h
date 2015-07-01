#ifdef UTILS_EXPORTS
#define NODEMANAGER_API __declspec(dllexport)
#else
#define NODEMANAGER_API __declspec(dllimport)
#endif

#pragma once
#include "Edge.h"
#include "Node.h"

namespace Graph {

	template <typename NODE_DATA, typename EDGE_DATA>
	class NodeManager
	{
	private:
		std::unique_ptr <std::vector<std::shared_ptr<Graph::Node<NODE_DATA, EDGE_DATA>>>> node_vector;

	public:
		NODEMANAGER_API NodeManager();
		NODEMANAGER_API virtual ~NodeManager();
		NODEMANAGER_API node_id create_node(node_id id);
		NODEMANAGER_API node_id create_node(NODE_DATA data);
		NODEMANAGER_API std::shared_ptr<Graph::Node<NODE_DATA, EDGE_DATA>> get_node(node_id id);
		NODEMANAGER_API bool connect_to(node_id from, node_id to, EDGE_DATA data);
		NODEMANAGER_API bool connect_each_other(node_id id1, node_id id2, EDGE_DATA data1, EDGE_DATA data2);
	};
}


