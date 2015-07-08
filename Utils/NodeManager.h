#ifdef UTILS_EXPORTS
#define NODEMANAGER_API __declspec(dllexport)
#else
#define NODEMANAGER_API __declspec(dllimport)
#endif

#pragma once
#include "Edge.h"
#include "Node.h"
#include "IdentifiableCollection.h"


namespace Graph {

	///<summary>
	/// ノードのID等の管理を行うクラス
	///</summary>
	template <typename NODE_DATA, typename EDGE_DATA>
	class NodeManager
	{
	private:
		std::unique_ptr<Collection::IdentifiableCollection<Node<NODE_DATA, EDGE_DATA>>> node_collection;

	public:
		NODEMANAGER_API NodeManager();
		NODEMANAGER_API virtual ~NodeManager();
		NODEMANAGER_API std::shared_ptr<Graph::Node<NODE_DATA, EDGE_DATA>> get_node(node_id id);
		NODEMANAGER_API node_id create_node(NODE_DATA data);
		NODEMANAGER_API bool connect_to(node_id from, node_id to, EDGE_DATA data);
		NODEMANAGER_API bool connect_each_other(node_id id1, node_id id2, EDGE_DATA data1, EDGE_DATA data2);
		NODEMANAGER_API void disconnect_from(node_id target, node_id from);
		NODEMANAGER_API void disconnect_each_other(node_id id1, node_id id2);
		NODEMANAGER_API void sort_node_collection();
	};
}
