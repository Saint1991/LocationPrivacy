#pragma once


///<summary>
/// �R���X�g���N�^
///</summary>
template <typename NODE_DATA, typename EDGE_DATA>
Graph::WarshallFloyd<NODE_DATA, EDGE_DATA>::WarshallFloyd(std::shared_ptr<const Collection::IdentifiableCollection<std::shared_ptr<const Node<NODE_DATA, EDGE_DATA>>>> node_collection) : node_collection(node_collection)
{
}


///<summary>
/// �f�X�g���N�^
///</summary>
template <typename NODE_DATA, typename EDGE_DATA>
Graph::WarshallFloyd<NODE_DATA, EDGE_DATA>::~WarshallFloyd()
{
}


///<summary>
/// ���[�V�����t���C�h�@�ɂ�郋�[�e�B���O�e�[�u���̍쐬
/// �Q�� (http://dai1741.github.io/maximum-algo-2012/docs/shortest-path/)
///</summary>
template <typename NODE_DATA, typename EDGE_DATA>
std::unique_ptr<const std::vector<std::vector<Graph::node_id>>> Graph::WarshallFloyd<NODE_DATA, EDGE_DATA>::create_routing_table() const
{
	if (node_collection == nullptr) return nullptr;
	std::unique_ptr<const std::vector<std::vector<Graph::node_id>>> routing_table = std::make_unique<const std::vector<std::vector<Graph::node_id>>>();
}
