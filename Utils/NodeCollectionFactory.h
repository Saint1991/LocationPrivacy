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
	/// �m�[�h�̐����C�ڑ��֌W�̍\�����s������IdentifiableCollection��Ԃ��t�@�N�g���e���v���[�g
	/// ������̓N���X�̓�����Ԃ͏����������̂Œ���
	/// �R���p�C���̎d�l������������ɏ��������Ȃ�...
	///</summary>
	template <typename NODE_DATA, typename EDGE_DATA>
	class NodeCollectionFactory
	{
	private:
		std::unique_ptr<Collection::IdentifiableCollection<Node<NODE_DATA, EDGE_DATA>>> node_collection;
		
	public:
		NodeCollectionFactory();
		virtual ~NodeCollectionFactory();

		//�\����ɗp�r�ɉ����Ă����̂����ꂩ��p����IdentifiableCollection���擾����z��
		std::unique_ptr<Collection::IdentifiableCollection< Node<const NODE_DATA, EDGE_DATA>>> get_static_node_collection();
		std::unique_ptr<Collection::IdentifiableCollection<Node<NODE_DATA, EDGE_DATA>>> get_updateable_node_collection();
		
		//�ڑ��֌W�\���p�̊e�탁�\�b�h


	};
}
