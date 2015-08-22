
namespace Graph 
{

	///<summary>
	/// �R���X�g���N�^
	///</summary>
	///<param name='id'>�m�[�hID</param>
	///<param name='data'>�m�[�h�Ɏ����������f�[�^</param>
	template <typename NODE_DATA, typename EDGE>
	Node<NODE_DATA, EDGE>::Node(node_id id, std::shared_ptr<NODE_DATA> data)
		: Identifiable(id), data(data), edge_list(std::make_shared<std::list<std::shared_ptr<EDGE>>>())
	{
	}

	///<summary>
	/// �R�s�[�R���X�g���N�^
	/// edge_list�̒��g���R�s�[����
	///</summary>
	template <typename NODE_DATA, typename EDGE>
	Node<NODE_DATA, EDGE>::Node(const Node<NODE_DATA, EDGE> &node) 
		: Identifiable(node), data(node.data), edge_list(std::make_shared<std::list<std::shared_ptr<EDGE>>>())
	{
		for (std::list<std::shared_ptr<EDGE>>::const_iterator iter = node.edge_list->begin(); iter != node.edge_list->end(); iter++) {
			EDGE copy = *(*iter);
			edge_list->push_back(std::make_shared<EDGE>(copy));
		}
	}

	///<summary>
	/// �f�X�g���N�^
	///</summary>
	template <typename NODE_DATA, typename EDGE>
	Node<NODE_DATA, EDGE>::~Node()
	{
	}


	///<summary>
	/// �w�肵���m�[�h�ւ̃G�b�W�̃|�C���^���擾����
	/// �G�b�W�ւ̕ύX�͕s��
	/// ���݂��Ȃ��ꍇ��nullptr��Ԃ��D
	///</summary>
	///<param name='id'>�m�[�hID</param> 
	template <typename NODE_DATA, typename EDGE>
	std::shared_ptr<EDGE const> Node<NODE_DATA, EDGE>::get_static_edge_to(node_id id) const
	{
		for (std::list<std::shared_ptr<EDGE>>::const_iterator iter = edge_list->begin(); iter != edge_list->end(); iter++) {
			if ((*iter)->get_to() == id) {
				return *iter;
			}
		}
		return nullptr;
	}


	///<summary>
	/// �w�肵���m�[�h�ւ̃G�b�W�̃|�C���^���擾����
	/// ���݂��Ȃ��ꍇ��nullptr��Ԃ��D
	///</summary>
	///<param name='id'>�m�[�hID</param> 
	template <typename NODE_DATA, typename EDGE>
	std::shared_ptr<EDGE> Node<NODE_DATA, EDGE>::get_edge_to(node_id id)
	{
		for (std::list<std::shared_ptr<EDGE>>::const_iterator iter = edge_list->begin(); iter != edge_list->end(); iter++) {
			if ((*iter)->get_to() == id) {
				return *iter;
			}
		}
		return nullptr;
	}

	///<summary>
	/// �w�肵���p�����[�^�����G�b�W��ڑ��G�b�W���X�g�ɒǉ����܂��D
	/// �����̃G�b�W�̏ꍇ�͒ǉ�������false��Ԃ��܂��D
	///</summary>
	///<param name='to'>�ڑ���m�[�h</param>
	///<param name='data'>�G�b�W���������l�f�[�^</param>
	template <typename NODE_DATA, typename EDGE>
	bool Node<NODE_DATA, EDGE>::connect_to(std::shared_ptr<EDGE> edge)
	{
		std::shared_ptr<EDGE> target = get_edge_to(edge->get_to());
		if (target != nullptr) {
			return false;
		}
		edge_list->push_back(edge);
		return true;
	}


	///<summary>
	/// �w�肵��ID�̃G�b�W���폜���C�ؒf���܂�
	/// �폜�����������ꍇtrue�C�������Ȃ������ꍇfalse��Ԃ��D
	///</summary>
	///<param name='from'>�ؒf����m�[�h��ID</param>
	template <typename NODE_DATA, typename EDGE>
	bool Node<NODE_DATA, EDGE>::disconnect_from(node_id from)
	{
		std::shared_ptr<EDGE> target = get_edge_to(from);
		if (target != nullptr) {
			edge_list->remove(target);
			return true;
		}
		return false;
	}

	///<summary>
	/// �w�肵���m�[�h�Ɛڑ����Ă��邩�ǂ����𔻒肷��
	///</summary>
	template <typename NODE_DATA, typename EDGE>
	bool Node<NODE_DATA, EDGE>::is_connecting_to(node_id link_to) const
	{
		return get_static_edge_to(link_to) != nullptr;
	}

	///<summary>
	/// �ڑ���m�[�h��ID���X�g��Ԃ��D
	/// �m�[�h�ԍ��͏����Ƀ\�[�g
	///</summary>
	template <typename NODE_DATA, typename EDGE>
	std::list<node_id> Node<NODE_DATA, EDGE>::get_connecting_node_list() const
	{
		std::list<node_id> connect_node_list;
		for (std::list<std::shared_ptr<EDGE>>::const_iterator iter = edge_list->begin(); iter != edge_list->end(); iter++) {
			node_id id = (*iter)->get_to();
			connect_node_list.push_back(id);
		}
		connect_node_list.sort();
		return connect_node_list;
	}

	///<summary>
	/// �e�G�b�W�ɂ���execute_function�����s����
	/// �ύX�����g�ɔ��f�����̂Œ���
	///</summary>
	template <typename NODE_DATA, typename EDGE>
	void Node<NODE_DATA, EDGE>::for_each_edge(const std::function<void(std::shared_ptr<EDGE>)>& execute_function)
	{
		for (std::list<std::shared_ptr<EDGE>>::const_iterator iter = edge_list->begin(); iter != edge_list->end(); iter++) {
			execute_function(*iter);
		}
	}

	///<summary>
	/// �e�G�b�W�ɂ���execute_function�����s����
	///</summary>
	template <typename NODE_DATA, typename EDGE>
	void Node<NODE_DATA, EDGE>::for_each_edge(const std::function<void(std::shared_ptr<EDGE const>)>& execute_function) const
	{
 		for (std::list<std::shared_ptr<EDGE>>::const_iterator iter = edge_list->begin(); iter != edge_list->end(); iter++) {
			execute_function(*iter);
		}
	}
}


