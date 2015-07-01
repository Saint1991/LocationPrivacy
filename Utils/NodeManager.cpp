#include "stdafx.h"
#include "NodeManager.h"

template <typename NODE_DATA, typename EDGE_DATA>
Graph::NodeManager<NODE_DATA, EDGE_DATA>::NodeManager() : node_vector(std::make_unique<std::vector<std::shared_ptr<Graph::Node<NODE_DATA, EDGE_DATA>>>>())
{
}

template <typename NODE_DATA, typename EDGE_DATA>
Graph::NodeManager<NODE_DATA, EDGE_DATA>::~NodeManager()
{
}
