
namespace Graph 
{


	///<summary>
	/// �R���X�g���N�^
	///</summary>
	template <typename NODE, typename EDGE_DATA>
	NodeCollectionFactory<NODE, EDGE_DATA>::NodeCollectionFactory() 
		: node_collection(std::make_shared<Collection::IdentifiableCollection<std::shared_ptr<NODE>>>())
	{
	}

	///<summary>
	/// �f�X�g���N�^
	///</summary>
	template <typename NODE, typename EDGE_DATA>
	NodeCollectionFactory<NODE, EDGE_DATA>::~NodeCollectionFactory()
	{
	}

	///<summary>
	/// create_nodes, set_connectivities�̎����ʂ�ɃR���N�V�������\�����C
	/// �ύX�s�̏�ԂɃ��b�N�����R���N�V�������擾����D
	///</summary>
	template <typename NODE, typename EDGE_DATA>
	std::shared_ptr<const Collection::IdentifiableCollection<std::shared_ptr<const NODE>>> NodeCollectionFactory<NODE, EDGE_DATA>::create_static_node_collection() const
	{
		create_nodes();
		set_connectivities();
		return std::move(node_collection);
	}

	///<summary>
	/// create_nodes, set_connectivities�̎����ʂ�ɃR���N�V�������\�����C
	/// �X�V�\�ȏ�ԂŃR���N�V�������擾����D
	///</summary>
	template <typename NODE, typename EDGE_DATA>
	std::shared_ptr<Collection::IdentifiableCollection<std::shared_ptr<NODE>>> NodeCollectionFactory<NODE, EDGE_DATA>::create_updateable_node_collection()
	{
		create_nodes();
		set_connectivities();
		return std::move(node_collection);
	}


	///<summary>
	/// Node���p�����Ă���N���X��shared_ptr��ǉ�����
	/// ����ID�̃m�[�h���쐬���悤�Ƃ����ꍇ��DuplicatedIdException���X���[����Cfalse���Ԃ����D
	///</summary>
	template <typename NODE, typename EDGE_DATA>
	bool NodeCollectionFactory<NODE, EDGE_DATA>::add_node(std::shared_ptr<NODE> node)
	{
		return node_collection->add(node);
	}

	///<summary>
	/// �w�肵��ID�����m�[�h���폜����
	/// �폜�����������ꍇ��true��Ԃ�
	///</summary>
	template <typename NODE, typename EDGE_DATA>
	bool NodeCollectionFactory<NODE, EDGE_DATA>::remove_node(const node_id& id)
	{
		return node_collection->remove_by_id(id);
	}


	///<summary>
	/// from����to��data�𑮐��Ɏ������N���쐬���܂��D
	/// from, to�����ꂩ�̃m�[�h�����݂��Ȃ������ꍇ��C�����̃����N��ǉ����悤�Ƃ����ꍇ��false��Ԃ��܂��D
	///</summary>
	template <typename NODE, typename EDGE_DATA>
	bool NodeCollectionFactory<NODE, EDGE_DATA>::connect(const node_id& from, const node_id& to, EDGE_DATA data)
	{
		std::shared_ptr<NODE> node = node_collection->get_by_id(from);
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
	template <typename NODE, typename EDGE_DATA>
	bool NodeCollectionFactory<NODE, EDGE_DATA>::connect_each_other(const node_id& id1, const node_id& id2, EDGE_DATA data)
	{
		std::shared_ptr<NODE> node1 = node_collection->get_by_id(from);
		std::shared_ptr<NODE> node2 = node_collection->get_by_id(to);
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
	template <typename NODE, typename EDGE_DATA>
	bool NodeCollectionFactory<NODE, EDGE_DATA>::disconnect(const node_id& target, const node_id& from)
	{
		std::shared_ptr<NODE> node = node_collection->get_by_id(target);
		if (target == nullptr) {
			return false;
		}
		return node->disconnect_from(from);
	}


	///<summary>
	/// 2�̃m�[�h�Ԃ̃����N��o�����ؒf���܂��D
	///</summary>
	template <typename NODE, typename EDGE_DATA>
	void NodeCollectionFactory<NODE, EDGE_DATA>::disconnect_each_other(const node_id& id1, const node_id& id2)
	{
		std::shared_ptr<NODE> node1 = node_collection->get_by_id(id1);
		std::shared_ptr<NODE> node2 = node_collection->get_by_id(id2);

		if (node1 != nullptr) {
			node1->disconnect_from(id2);
		}

		if (node2 != nullptr) {
			node2->disconnect_from(id1);
		}
	}
}

