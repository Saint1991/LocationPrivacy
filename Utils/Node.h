#ifdef UTILS_EXPORTS
#define NODE_API __declspec(dllexport)
#else
#define NODE_API __declspec(dllimport)
#endif

#pragma once
#include "Edge.h"
#include "Identifiable.h"

namespace Graph {


	///<summary>
	/// �O���t�ɂ�����m�[�h��\���e���v���[�g�N���X
	/// �R���p�C���̎d�l�ケ���Ɏ��������������Ȃ�...
	///</summary>
	template <typename NODE_DATA, typename EDGE_DATA>
	class Node : public Identifiable
	{
	protected:
		std::unique_ptr<std::list<std::shared_ptr<Edge<EDGE_DATA>>>> edge_list;
	public:
		std::shared_ptr<NODE_DATA> data;
		
		Node(node_id id, NODE_DATA data);
		Node(const Node& node);
		virtual ~Node();

		void connect_to(Graph::Edge<EDGE_DATA> link_to);
		void connect_to(node_id to, EDGE_DATA data);
		bool disconnect_from(node_id from);
		std::shared_ptr<Graph::Edge<EDGE_DATA>> get_edge_to(node_id id);
		bool is_connecting_to(node_id id);
		std::list<node_id> get_connecting_node_list();

		bool operator==(Graph::Node<NODE_DATA, EDGE_DATA> node);
		bool operator!=(Graph::Node<NODE_DATA, EDGE_DATA> node);
	};
}







///<summary>
/// �R���X�g���N�^
///</summary>
///<param name='id'>�m�[�hID</param>
///<param name='data'>�m�[�h�Ɏ����������f�[�^</param>
template <typename NODE_DATA, typename EDGE_DATA>
Graph::Node<NODE_DATA, EDGE_DATA>::Node(Graph::node_id id, NODE_DATA data) : Identifiable(id), data(std::make_shared<NODE_DATA>(data)), edge_list(std::make_unique<std::list<std::shared_ptr<Graph::Edge<EDGE_DATA>>>>())
{
}

///<summary>
/// �R�s�[�R���X�g���N�^ (�s�v����) 
/// �e�X�g�ŗv�m�F
///</summary>
template <typename NODE_DATA, typename EDGE_DATA>
Graph::Node<NODE_DATA, EDGE_DATA>::Node(const Graph::Node<NODE_DATA, EDGE_DATA> &node) : Identifiable(node), data(node.data), edge_list(std::make_unique<std::list<std::shared_ptr<Graph::Edge<EDGE_DATA>>>>())
{
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
		std::shared_ptr<Graph::Edge<EDGE_DATA>> connect = std::make_shared<Graph::Edge<EDGE_DATA>>(link_to);
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
	for (std::list<std::shared_ptr<Graph::Edge<EDGE_DATA>>>::iterator iter = edge_list->begin(); iter != edge_list->end(); iter++) {
		if (iter->get()->to == id) {
			return *iter;
		}
	}
	return nullptr;
}

///<summary>
/// �w�肵���m�[�h�Ɛڑ����Ă��邩�ǂ����𔻒肷��
///</summary>
template <typename NODE_DATA, typename EDGE_DATA>
bool Graph::Node<NODE_DATA, EDGE_DATA>::is_connecting_to(Graph::node_id link_to)
{
	return get_edge_to(link_to) != nullptr;
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
		Graph::node_id id = (*iter)->to;
		connect_node_list.push_back(id);
	}
	connect_node_list.sort();
	return connect_node_list;
}

template <typename NODE_DATA, typename EDGE_DATA>
bool Graph::Node<NODE_DATA, EDGE_DATA>::operator==(Graph::Node<NODE_DATA, EDGE_DATA> node)
{
	return id == node.id;
}

template <typename NODE_DATA, typename EDGE_DATA>
bool Graph::Node<NODE_DATA, EDGE_DATA>::operator!=(Graph::Node<NODE_DATA, EDGE_DATA> node)
{
	return id != node.id;
}
