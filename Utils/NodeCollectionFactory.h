#pragma once
#include "BasicEdge.h"
#include "Edge.h"
#include "IdentifiableCollection.h"

namespace Graph 
{

	///<summary>
	/// �m�[�h�̐����C�ڑ��֌W�̍\�����s������IdentifiableCollection��Ԃ��t�@�N�g�����ۃN���X�e���v���[�g
	/// create_nodes, set_connectivity���������K�v�D
	/// create_nodes, set_connectivity�̏����Ŏ��s����CIdentifiableCollection�̓��e���쐬����D
	/// �쐬��́C������X�V�������邩�ɂ����create_static_node_collection��create_updateable_node_collection���g�������Ď擾����D
	/// �擾��̓t�@�N�g�����̃R���N�V������nullptr�ɂȂ�̂Œ���
	/// �܂��CNODE, EDGE, EDGE_DATA�̌^�̐��������Ƃ�Ă��Ȃ��ƃR���p�C���ł��Ȃ��̂Œ���
	/// EDGE�ɂ�BasicEdge��Edge<EDGE_DATA>�̔h���N���X�̂ݎg�p�ł��܂�
	/// Connect�n�̃��\�b�h��BasicEdge�ŕʓ���C����BasicEdge�ȊO�ŕʓ���ł��Ȃ��悤�ɂȂ��Ă��邩�m�F���K�v
	/// BasicEdge���g������EDGE_DATA��nullptr_t�ɂ��Ă�������
	///</summary>
	template <typename NODE, typename EDGE, typename EDGE_DATA>
	class NodeCollectionFactory
	{
	static_assert(std::is_base_of<Edge<EDGE_DATA>, EDGE>::value || std::is_same<BasicEdge, EDGE>::value, "template type EDGE is not derived from Edge<EDGE_DATA>");
	private:
		//�R�s�[�֎~
		NodeCollectionFactory(const NodeCollectionFactory& factory);
	
	protected:
		std::shared_ptr<Collection::IdentifiableCollection<Graph::node_id, NODE>> node_collection;
		virtual void create_nodes() = 0;
		virtual void set_connectivities() = 0;

		//�ڑ��֌W�\���p�̃��\�b�h
		//������p����create_nodes��set_connectivities����������D
		bool add_node(std::shared_ptr<NODE> node);
		bool remove_node(const node_id& id);
		bool connect(const node_id& from, const node_id& to, std::shared_ptr<EDGE_DATA> data);
		bool connect_each_other(const node_id& node1, const node_id& node2, std::shared_ptr<EDGE_DATA> data);
		bool disconnect(const node_id& target, const node_id& from);
		void disconnect_each_other(const node_id& node1, const node_id& node2);

	public:
		NodeCollectionFactory();
		virtual ~NodeCollectionFactory();

		//�\����ɗp�r�ɉ����Ă����̂����ꂩ��p����IdentifiableCollection���擾����z��
		//�����̃��\�b�h�擾��́C���̃t�@�N�g�����ێ�����node_collection�͋�ɏ��������������D
		std::shared_ptr<const Collection::IdentifiableCollection<long, NODE>> create_static_node_collection();
		std::shared_ptr<Collection::IdentifiableCollection<long, NODE>> create_updateable_node_collection();
	};

	
	#pragma region BasicEdge
	template <typename NODE>
	class NodeCollectionFactory<NODE, BasicEdge, nullptr_t>
	{
		bool connect(const node_id& from, const node_id& to);
		bool connect_each_other(const node_id& id1, const node_id& id2);
	};
	#pragma endregion EDGE��BasicEdge�̎��̂��߂̕������ꉻ
	
}

#include "NodeCollectionFactory.hpp"