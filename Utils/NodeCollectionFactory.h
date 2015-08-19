#pragma once
#include "Edge.h"
#include "Node.h"
#include "IdentifiableCollection.h"

namespace Graph 
{

	///<summary>
	/// �m�[�h�̐����C�ڑ��֌W�̍\�����s������IdentifiableCollection��Ԃ��t�@�N�g�����ۃN���X�e���v���[�g
	/// create_nodes, set_connectivity���������K�v�D
	/// create_nodes, set_connectivity�̏����Ŏ��s����CIdentifiableCollection�̓��e���쐬����D
	/// �쐬��́C������X�V�������邩�ɂ����create_static_node_collection��create_updateable_node_collection���g�������Ď擾����D
	/// �擾��̓t�@�N�g�����̃R���N�V�����͎g�p�ł��Ȃ��Ȃ�̂Œ���
	///</summary>
	template <typename NODE, typename EDGE_DATA>
	class NodeCollectionFactory
	{
	
	private:
		//�R�s�[�֎~
		NodeCollectionFactory(const NodeCollectionFactory& factory);
	
	protected:
		std::shared_ptr<Collection::IdentifiableCollection<std::shared_ptr<NODE>>> node_collection;
		virtual void create_nodes() = 0;
		virtual void set_connectivities() = 0;

		//�ڑ��֌W�\���p�̃��\�b�h
		//������p����create_nodes��set_connectivities����������D
		bool add_node(std::shared_ptr<NODE> node);
		bool remove_node(const node_id& id);
		bool connect(const node_id& from, const node_id& to, EDGE_DATA data);
		bool connect_each_other(const node_id& node1, const node_id& node2, EDGE_DATA data);
		bool disconnect(const node_id& target, const node_id& from);
		void disconnect_each_other(const node_id& node1, const node_id& node2);
		
	public:
		NodeCollectionFactory();
		virtual ~NodeCollectionFactory();

		//�\����ɗp�r�ɉ����Ă����̂����ꂩ��p����IdentifiableCollection���擾����z��
		//�����̃��\�b�h�擾��́C���̃t�@�N�g�����ێ�����node_collection�͋�ɏ��������������D
		std::shared_ptr<const Collection::IdentifiableCollection<std::shared_ptr<const NODE>>> create_static_node_collection() const;
		std::shared_ptr<Collection::IdentifiableCollection<std::shared_ptr<NODE>>> create_updateable_node_collection();
	};
}

#include "NodeCollectionFactory.hpp"