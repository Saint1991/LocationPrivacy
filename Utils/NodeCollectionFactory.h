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


///<summary>
/// create_nodes, set_connectivities�̎����ʂ�ɃR���N�V�������\�����C
/// �ύX�s�̏�ԂɃ��b�N�����R���N�V�������擾����D
///</summary>
template <typename NODE_DATA, typename EDGE_DATA>
std::unique_ptr<const Collection::IdentifiableCollection<std::shared_ptr<Graph::Node<NODE_DATA, EDGE_DATA>>>> Graph::NodeCollectionFactory<NODE_DATA, EDGE_DATA>::create_static_node_collection() const
{
	std::cout << "Start Creating Node Collection." << std::endl;
	
	create_nodes();
	set_connectivities();
	std::unique_ptr<const Collection::IdentifiableCollection<std::shared_ptr<Graph::Node<NODE_DATA, EDGE_DATA>>>> temp = std::move(node_collection);
	node_collection = std::make_unique<Collection::IdentifiableCollection<std::shared_ptr<Graph::Node<NODE_DATA, EDGE_DATA>>>();
	
	std::cout << "Finish Creating Node Collection." << std::endl;
	return std::move(temp);
}

///<summary>
/// create_nodes, set_connectivities�̎����ʂ�ɃR���N�V�������\�����C
/// �X�V�\�ȏ�ԂŃR���N�V�������擾����D
///</summary>
template <typename NODE_DATA, typename EDGE_DATA>
std::unique_ptr<Collection::IdentifiableCollection<std::shared_ptr<Graph::Node<NODE_DATA, EDGE_DATA>>>> Graph::NodeCollectionFactory<NODE_DATA, EDGE_DATA>::create_updateable_node_collection()
{
	std::cout << "Start Creating Node Collection." << std::endl;
	
	create_nodes();
	set_connectivities();
	std::unique_ptr<Collection::IdentifiableCollection<std::shared_ptr<Graph::Node<NODE_DATA, EDGE_DATA>>>> temp = std::move(node_collection);
	node_collection = std::make_unique<Collection::IdentifiableCollection<std::shared_ptr<Graph::Node<NODE_DATA, EDGE_DATA>>>();
	
	std::cout << "Finish Creating Node Collection." << std::endl;
	return std::move(temp);
}


///<summary>
/// �^����ꂽ�����Ńm�[�h��V�K�쐬����D
/// ����ID�̃m�[�h���쐬���悤�Ƃ����ꍇ��DuplicatedIdException���X���[����Cfalse���Ԃ����D
///</summary>
template <typename NODE_DATA, typename EDGE_DATA>
bool Graph::NodeCollectionFactory<NODE_DATA, EDGE_DATA>::create_node(Graph::node_id id, NODE_DATA data)
{
	return node_collection->add(Graph::Node<NODE_DATA, EDGE_DATA>(id, data));
}


///<summary>
/// �w�肵��ID�����m�[�h���폜����
///</summary>
template <typename NODE_DATA, typename EDGE_DATA>
bool Graph::NodeCollectionFactory<NODE_DATA, EDGE_DATA>::remove_node(Graph::node_id id)
{
	return node_collection->remove_by_id(id);
}


///<summary>
/// from����to��data�𑮐��Ɏ������N���쐬���܂��D
/// from, to�����ꂩ�̃m�[�h�����݂��Ȃ������ꍇ��C�����̃����N��ǉ����悤�Ƃ����ꍇ��false��Ԃ��܂��D
///</summary>
template <typename NODE_DATA, typename EDGE_DATA>
bool Graph::NodeCollectionFactory<NODE_DATA, EDGE_DATA>::connect(Graph::node_id from, Graph::node_id to, EDGE_DATA data)
{
	std::shared_ptr<Graph::Node<NODE_DATA, EDGE_DATA> node = node_collection->get_by_id(from);
	bool is_to_exists = node_collection->contains(to);
	if (node == nullptr || !is_to_exists) {
		return false;
	}
	return node->connect_to(to, data);
}


///<summary>
/// �w�肵��ID�Ԃɑ��݂Ƀ����N���쐬���܂��D
/// �o�����Ƀ����N���쐬�ł��Ȃ���΁C�������ؒf����Ă����Ԃɖ߂��܂��D
/// �o�����Ƃ��ɐ��������ꍇtrue���C���s�����ꍇfalse��Ԃ��܂��D
///</summary>
template <typename NODE_DATA, typename EDGE_DATA>
bool Graph::NodeCollectionFactory<NODE_DATA, EDGE_DATA>::connect_each_other(Graph::node_id id1, Graph::node_id id2, EDGE_DATA data)
{
	std::shared_ptr < Graph::Node<NODE_DATA, EDGE_DATA> node1 = node_collection->get_by_id(from);
	std::shared_ptr < Graph::Node<NODE_DATA, EDGE_DATA> node2 = node_collection->get_by_id(to);
	if (node1 == nullptr || node2 == nullptr) {
		return false;
	}

	if (!node1->connect_to(id2, data)) {
		return false;
	}

	if (!node2->connect_to(id1, data)) {
		node1->disconnect_from(id2);
		return false;
	}
	return true;
}



///<summary>
/// target�m�[�h����from�m�[�h�ւ̃����N��ؒf���܂��D
/// �ؒf�ɐ��������ꍇ��true���C�����N��m�[�h��������Ȃ������ꍇ��false��Ԃ��܂��D
///</summary>
template <typename NODE_DATA, typename EDGE_DATA>
bool Graph::NodeCollectionFactory<NODE_DATA, EDGE_DATA>::disconnect(Graph::node_id target, Graph::node_id from)
{
	std::shared_ptr<Graph::Node<NODE_DATA, EDGE_DATA>> node = node_collection->get_by_id(target);
	if (target == nullptr) {
		return false;
	}

	return node->disconnect_from(from);
}


///<summary>
/// 2�̃m�[�h�Ԃ̃����N��o�����ؒf���܂��D
///</summary>
template <typename NODE_DATA, typename EDGE_DATA>
void Graph::NodeCollectionFactory<NODE_DATA, EDGE_DATA>::disconnect_each_other(Graph::node_id id1, Graph::node_id id2)
{
	std::shared_ptr<Graph::Node<NODE_DATA, EDGE_DATA>> node1 = node_collection->get_by_id(id1);
	std::shared_ptr<Graph::Node<NODE_DATA, EDGE_DATA>> node2 = node_collection->get_by_id(id2);

	if (node1 != nullptr) {
		node1->disconnect_from(id2);
	}

	if (node2 != nullptr) {
		node2->disconnect_from(id1);
	}
}
