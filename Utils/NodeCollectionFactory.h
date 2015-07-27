#pragma once
#include "Edge.h"
#include "Node.h"
#include "IdentifiableCollection.h"

namespace Graph {

	///<summary>
	/// �m�[�h�̐����C�ڑ��֌W�̍\�����s������IdentifiableCollection��Ԃ��t�@�N�g�����ۃN���X�e���v���[�g
	/// create_nodes, set_connectivity���������K�v�D
	/// create_nodes, set_connectivity�̏����Ŏ��s����CIdentifiableCollection�̓��e���쐬����D
	/// �쐬��́C������X�V�������邩�ɂ����create_static_node_collection��create_updateable_node_collection���g�������Ď擾����D
	/// ������̓N���X�̓�����Ԃ͏����������̂Œ���
	/// �R���p�C���̎d�l������������ɏ��������Ȃ�...
	///</summary>
	template <typename NODE_DATA, typename EDGE_DATA>
	class NodeCollectionFactory
	{
	protected:

		std::unique_ptr<Collection::IdentifiableCollection<std::shared_ptr<Node<NODE_DATA, EDGE_DATA>>>> node_collection;
		virtual void create_nodes() = 0;
		virtual void set_connectivities() = 0;

		//�ڑ��֌W�\���p�̃��\�b�h
		//������p����create_nodes��set_connectivities����������D
		bool create_node(node_id id, NODE_DATA data);
		bool remove_node(node_id id);
		bool connect(node_id from, node_id to, EDGE_DATA data);
		bool connect_each_other(node_id node1, node_id node2, EDGE_DATA data);
		bool disconnect(node_id target, node_id from);
		void disconnect_each_other(node_id node1, node_id node2);
		
	public:
		NodeCollectionFactory();
		virtual ~NodeCollectionFactory();

		//�\����ɗp�r�ɉ����Ă����̂����ꂩ��p����IdentifiableCollection���擾����z��
		//�����̃��\�b�h�擾��́C���̃t�@�N�g�����ێ�����node_collection�͋�ɏ��������������D
		std::unique_ptr<const Collection::IdentifiableCollection<std::shared_ptr<Node<NODE_DATA, EDGE_DATA>>>> create_static_node_collection() const;
		std::unique_ptr<Collection::IdentifiableCollection<std::shared_ptr<Node<NODE_DATA, EDGE_DATA>>>> create_updateable_node_collection();

	};
}

#include "NodeCollectionFactory.hpp"