#include "stdafx.h"
#include "NodeManager.h"


///<summary>
/// �R���X�g���N�^
///</summary>
template <typename NODE_DATA, typename EDGE_DATA>
Graph::NodeManager<NODE_DATA, EDGE_DATA>::NodeManager() : node_collection(std::make_unique<Collection::IdentifiableCollection>())
{
	node_collection->clear();
}

///<summary>
/// �f�X�g���N�^
///</summary>
template <typename NODE_DATA, typename EDGE_DATA>
Graph::NodeManager<NODE_DATA, EDGE_DATA>::~NodeManager()
{
	node_collection->clear();
}

///<summary>
/// �w�肵��ID�̃m�[�h��Ԃ��D
/// ������Ȃ��ꍇ��nullptr��Ԃ��D
///</summary>
/// <param name='id'>�擾�������m�[�h��ID</param>
template <typename NODE_DATA, typename EDGE_DATA>
std::shared_ptr<Graph::Node<NODE_DATA, EDGE_DATA>> Graph::NodeManager<NODE_DATA, EDGE_DATA>::get_node(Graph::node_id id)
{
	//�vdynamic_cast?
	return node_collection->find_by_id(id);
}

///<summary>
/// �m�[�h��V�K�쐬���C�쐬�����m�[�h��ID��Ԃ��܂��D
///</summary>
template <typename NODE_DATA, typename EDGE_DATA>
Graph::node_id Graph::NodeManager<NODE_DATA, EDGE_DATA>::create_node(NODE_DATA data)
{
	std::size_t new_id = node_collection->size();
	std::shared_ptr<Graph::Node<NODE_DATA, EDGE_DATA>> node = std::make_shared<Graph::Node<NODE_DATA, EDGE_DATA>>(new_id, data);
	node_collection->push_back(node);
}

///<summary>
/// from����to�Ɏw�肵���f�[�^�𑮐��Ƃ��Ă������N�𒣂�
///</summary>
template <typename NODE_DATA, typename EDGE_DATA>
bool Graph::NodeManager<NODE_DATA, EDGE_DATA>::connect_to(Graph::node_id from, Graph::node_id to, EDGE_DATA data)
{
	std::shared_ptr<Graph::Node<NODE_DATA, EDGE_DATA>> from_node = get_node(from);
	std::shared_ptr<Graph::Node<NODE_DATA, EDGE_DATA>> to_node = get_node(to);
	if (from_node != nullptr && to_node != nullptr) {
		from_node->connect_to(to, data);
		return true;
	}
	return false;
}

///<summary>
/// id1��id2�𑊌݂Ƀ����N����D
/// id1����id2�ւ�data1�Cid2����id1�ւ�data2�̑����l�������������N��������
///</summary>
template <typename NODE_DATA, typename EDGE_DATA>
bool Graph::NodeManager<NODE_DATA, EDGE_DATA>::connect_each_other(Graph::node_id id1, Graph::node_id id2, EDGE_DATA data1, EDGE_DATA data2)
{
	std::shared_ptr<Graph::Node<NODE_DATA, EDGE_DATA>> node1 = get_node(id1);
	std::shared_ptr<Graph::Node<NODE_DATA, EDGE_DATA>> node2 = get_node(id2);
	if (from_node != nullptr && to_node != nullptr) {
		from_node->connect_to(id2, data1);
		to_node->connect_to(id1, data2);
		return true;
	}
	return false;
}


///<summary>
/// target��from�ɑ΂��Ă������N���폜����D
/// �폜�����������ꍇ��true���C�����N��������Ȃ��ꍇ��false��Ԃ��D
///</summary>
template <typename NODE_DATA, typename EDGE_DATA>
void Graph::NodeManager<NODE_DATA, EDGE_DATA>::disconnect_from(Graph::node_id target, Graph::node_id from)
{
	std::shared_ptr <Graph::Node<NODE_DATA, EDGE_DATA>> target_node = get_node(target);
	if (target_node != nullptr) {
		target_node->disconnect_from(from);
	}
}

///<summary>
/// ���݂Ƀ����N��ؒf����D
/// �������Ƃ��Ƀ����N�����݂��C�ؒf�����ꍇ��true�C����ȊO��false��Ԃ��D
///</summary>
template <typename NODE_DATA, typename EDGE_DATA>
void Graph::NodeManager<NODE_DATA, EDGE_DATA>::disconnect_each_other(Graph::node_id id1, Graph::node_id id2)
{
	std::shared_ptr<Graph::Node<NODE_DATA, EDGE_DATA>> node1 = get_node(id1);
	if (node1 != nullptr) {
		node1->disconnect_from(id2);
	}

	std::shared_ptr< Graph::Node<NODE_DATA, EDGE_DATA>> node2 = get_node(id2);
	if (node2 != nullptr) {
		node2->disconnect_from(id1);
	}
}

///<summary>
/// �m�[�h�R���N�V�������\�[�g����D
/// �\�[�g����ƃm�[�h�ɃA�N�Z�X����ۂ̒T���̌������オ��D
///</summary>
template <typename NODE_DATA, typename EDGE_DATA>
void Graph::NodeManager<NODE_DATA, EDGE_DATA>::sort_node_collection()
{
	node_collection->sort();
}