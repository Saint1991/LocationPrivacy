#include "stdafx.h"
#include "Node.h"

///<summary>
/// �R���X�g���N�^
///</summary>
///<param name='id'>�m�[�hID</param>
///<param name='data'>�m�[�h�Ɏ����������f�[�^</param>
template <typename NODE_DATA, typename EDGE_DATA>
Graph::Node<NODE_DATA, EDGE_DATA>::Node(Graph::node_id id, NODE_DATA data) : id(id), data(std::make_shared<NODE_DATA>(data)), edge_list(std::make_unique<std::list<std::shared_ptr<Graph::Edge<EDGE_DATA>>>>())
{
}

///<summary>
/// �R�s�[�R���X�g���N�^ (�s�v����) 
/// �e�X�g�ŗv�m�F
///</summary>
template <typename NODE_DATA, typename EDGE_DATA>
Graph::Node<NODE_DATA, EDGE_DATA>::Node(const Graph::Node<NODE_DATA, EDGE_DATA> &node) : id(node->id)
{
	edge_list = std::make_unique < std::list<std::shared_ptr<Graph::Edge<EDGE_DATA>>>();
	for (std::list<std::shared_ptr<Graph::Edge<EDGE_DATA>>>::iterator iter = node.edge_list->begin(); iter != node.edge_list->end(); iter++) {
		std::shared_ptr<Graph::Edge<EDGE_DATA>> s_iter = *iter;
		std::shared_ptr<Graph::Edge<EDGE_DATA>> copy = std::make_shared<Graph::Edge<EDGE_DATA>>(s_iter->to, s_iter->data);
		edge_list->push_back(s_iter);
	}
}

///<summary>
/// �f�X�g���N�^
///</summary>
template <typename NODE_DATA, typename EDGE_DATA>
Graph::Node<NODE_DATA, EDGE_DATA>::~Node()
{
}

///<summary>
/// �w�肵���G�b�W��ڑ��G�b�W���X�g�ɒǉ����܂��D
/// �����̃����N�̏ꍇ�͋������X�V���܂�
///</summary>
///<param name='link_to'>�ڑ����X�g�ɒǉ�����m�[�h</param>
template <typename NODE_DATA, typename EDGE_DATA>
void Graph::Node<NODE_DATA, EDGE_DATA>::connect_to(Graph::Edge<EDGE_DATA> link_to)
{
	std::shared_ptr<Graph::Edge<EDGE_DATA>> target = this->get_edge_to(link_to.to);
	if (target == nullptr) {
		std::shared_ptr<Graph::Edge<EDGE_DATA>> connect = std::make_shared<Graph::Edge>(link_to);
		edge_list->push_back(connect);
	}
}

///<summary>
/// �w�肵���p�����[�^�����G�b�W��ڑ��G�b�W���X�g�ɒǉ����܂��D
/// �����̃����N�̏ꍇ�͋������X�V���܂�
///</summary>
///<param name='to'>�ڑ���m�[�h</param>
///<param name='data'>�G�b�W���������l�f�[�^</param>
template <typename NODE_DATA, typename EDGE_DATA>
void Graph::Node<NODE_DATA, EDGE_DATA>::connect_to(Graph::node_id to, EDGE_DATA data)
{
	Graph::Edge<EDGE_DATA> link_to(to, data);
	connect_to(link_to);
}

///<summary>
/// �w�肵��ID�̃G�b�W���폜���C�ؒf���܂�
/// �폜�����������ꍇtrue�C�������Ȃ������ꍇfalse��Ԃ��D
///</summary>
///<param name='from'>�ؒf����m�[�h��ID</param>
template <typename NODE_DATA, typename EDGE_DATA>
bool Graph::Node<NODE_DATA, EDGE_DATA>::disconnect_from(Graph::node_id from)
{
	std::shared_ptr<Graph::Edge<EDGE_DATA>> target = this->get_edge_to(from);
	if (target != nullptr) {
		this->edge_list->remove(target);
		return true;
	}
	return false;
}

///<summary>
/// �w�肵���m�[�h�ւ̃G�b�W�̃|�C���^���擾����
/// ���݂��Ȃ��ꍇ��nullptr��Ԃ��D
///</summary>
///<param name='id'>�m�[�hID</param> 
template <typename NODE_DATA, typename EDGE_DATA>
std::shared_ptr<Graph::Edge<EDGE_DATA>> Graph::Node<NODE_DATA, EDGE_DATA>::get_edge_to(Graph::node_id id)
{
	std::shared_ptr <Graph::Edge<EDGE_DATA> ret = nullptr;
	for (std::list<std::shared_ptr<Graph::Edge<EDGE_DATA>>>::iterator iter = edge_list->begin(); iter != edge_list->end(); iter++) {
		if (iter->get()->to == id) {
			ret.reset(iter->get());
		}
	}
	return ret;
}

///<summary>
/// �w�肵���m�[�h�Ɛڑ����Ă��邩�ǂ����𔻒肷��
///</summary>
template <typename NODE_DATA, typename EDGE_DATA>
bool Graph::Node<NODE_DATA, EDGE_DATA>::is_connecting_to(Graph::node_id link_to)
{
	return get_edge_to(id) != nullptr;
}

///<summary>
/// �ڑ����Ă���m�[�h��ID���X�g��Ԃ��D
/// �m�[�h�ԍ��͏����Ƀ\�[�g
///</summary>
template <typename NODE_DATA, typename EDGE_DATA>
std::list<Graph::node_id> Graph::Node<NODE_DATA, EDGE_DATA>::get_connecting_node_list()
{
	std::list<Graph::node_id> connect_node_list;
	for (std::list<std::shared_ptr<Graph::Edge<EDGE_DATA>>>::iterator iter = edge_list->begin(); iter != edge_list->end(); iter++) {
		connect_node_list.sort();
		return connect_node_list;
	}
}