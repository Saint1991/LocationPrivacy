
namespace Graph 
{

	///<summary>
	/// �R���X�g���N�^
	///</summary>
	///<param name='id'>�m�[�hID</param>
	///<param name='data'>�m�[�h�Ɏ����������f�[�^</param>
	template <typename NODE_DATA, typename EDGE>
	Node<NODE_DATA, EDGE>::Node(node_id id, std::shared_ptr<NODE_DATA> data)
		: Identifiable<node_id>(id), data(data), 
		edge_list(std::make_shared< std::set<std::shared_ptr<BasicEdge>, std::function<bool(std::shared_ptr<BasicEdge>, std::shared_ptr<BasicEdge>)>	>>([](std::shared_ptr<BasicEdge> edge1, std::shared_ptr<BasicEdge> edge2) {
			return edge1->get_to() < edge2->get_to();
		}))
	{
	}

	///<summary>
	/// �R�s�[�R���X�g���N�^
	/// edge_list�̒��g���R�s�[����
	///</summary>
	template <typename NODE_DATA, typename EDGE>
	Node<NODE_DATA, EDGE>::Node(const Node<NODE_DATA, EDGE> &node) 
		: Identifiable<node_id>(node), data(std::make_shared<NODE_DATA>(*node.data)), 
		edge_list(std::make_shared< std::set<std::shared_ptr<BasicEdge>, std::function<bool(std::shared_ptr<BasicEdge>, std::shared_ptr<BasicEdge>)>	>>([](std::shared_ptr<BasicEdge> edge1, std::shared_ptr<BasicEdge> edge2) {
		return edge1->get_to() < edge2->get_to();
	}))
	{
		for (std::set<std::shared_ptr<BasicEdge>, std::function<bool(std::shared_ptr<BasicEdge>, std::shared_ptr<BasicEdge>)>>::const_iterator iter = node.edge_list->begin(); iter != node.edge_list->end(); iter++) {
			std::shared_ptr<EDGE> edge = std::dynamic_pointer_cast<EDGE>(*iter);
			edge_list->insert(std::make_shared<EDGE>(*edge));
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
		std::set<std::shared_ptr<BasicEdge>, std::function<bool(std::shared_ptr<BasicEdge>, std::shared_ptr<BasicEdge>)>>::const_iterator iter = edge_list->find(std::make_shared<BasicEdge>(id));
		if (iter == edge_list->end()) return nullptr;
		return std::dynamic_pointer_cast<EDGE const>(*iter);
	}


	///<summary>
	/// �w�肵���m�[�h�ւ̃G�b�W�̃|�C���^���擾����
	/// ���݂��Ȃ��ꍇ��nullptr��Ԃ��D
	///</summary>
	///<param name='id'>�m�[�hID</param> 
	template <typename NODE_DATA, typename EDGE>
	std::shared_ptr<EDGE> Node<NODE_DATA, EDGE>::get_edge_to(node_id id)
	{
		std::set<std::shared_ptr<BasicEdge>, std::function<bool(std::shared_ptr<BasicEdge>, std::shared_ptr<BasicEdge>)>>::iterator iter = edge_list->find(std::make_shared<BasicEdge>(id));
		if (iter == edge_list->end()) return nullptr;
		return std::dynamic_pointer_cast<EDGE>(*iter);
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
		std::pair<std::set<std::shared_ptr<BasicEdge>, std::function<bool(std::shared_ptr<BasicEdge>, std::shared_ptr<BasicEdge>)>>::iterator, bool > result = edge_list->insert(edge);
		return result.second;
	}


	///<summary>
	/// �w�肵��ID�̃G�b�W���폜���C�ؒf���܂�
	/// �폜�����������ꍇtrue�C�������Ȃ������ꍇfalse��Ԃ��D
	///</summary>
	///<param name='from'>�ؒf����m�[�h��ID</param>
	template <typename NODE_DATA, typename EDGE>
	bool Node<NODE_DATA, EDGE>::disconnect_from(node_id from)
	{
		std::set<std::shared_ptr<BasicEdge>, std::function<bool(std::shared_ptr<BasicEdge>, std::shared_ptr<BasicEdge>)>>::iterator target = edge_list->find(std::make_shared<BasicEdge>(from));
		if (target != edge_list->end()) {
			edge_list->erase(target);
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
	std::vector<node_id> Node<NODE_DATA, EDGE>::get_connecting_node_list() const
	{
		std::vector<node_id> connect_node_list;
		for (std::set<std::shared_ptr<BasicEdge>, std::function<bool(std::shared_ptr<BasicEdge>, std::shared_ptr<BasicEdge>)>>::const_iterator iter = edge_list->begin(); iter != edge_list->end(); iter++) {
			node_id id = (*iter)->get_to();
			connect_node_list.push_back(id);
		}
		std::sort(connect_node_list.begin(), connect_node_list.end());
		return connect_node_list;
	}

	///<summary>
	/// �e�G�b�W�ɂ���execute_function�����s����
	/// �ύX�����g�ɔ��f�����̂Œ���
	///</summary>
	template <typename NODE_DATA, typename EDGE>
	void Node<NODE_DATA, EDGE>::for_each_edge(const std::function<void(std::shared_ptr<EDGE>)>& execute_function)
	{
		for (std::set<std::shared_ptr<BasicEdge>, std::function<bool(std::shared_ptr<BasicEdge>, std::shared_ptr<BasicEdge>)>>::const_iterator iter = edge_list->begin(); iter != edge_list->end(); iter++) {
			std::shared_ptr<EDGE> edge = std::dynamic_pointer_cast<EDGE>(*iter);
			execute_function(edge);
		}
	}

	///<summary>
	/// �e�G�b�W�ɂ���execute_function�����s����
	///</summary>
	template <typename NODE_DATA, typename EDGE>
	void Node<NODE_DATA, EDGE>::for_each_edge(const std::function<void(std::shared_ptr<EDGE const>)>& execute_function) const
	{
 		for (std::set<std::shared_ptr<BasicEdge>, std::function<bool(std::shared_ptr<BasicEdge>, std::shared_ptr<BasicEdge>)>>::const_iterator iter = edge_list->begin(); iter != edge_list->end(); iter++) {
			std::shared_ptr<EDGE const> edge = std::dynamic_pointer_cast<EDGE const>(*iter);
			execute_function(edge);
		}
	}


	///<summary>
	/// �e�G�b�W�ɂ���execute_function�����s����
	/// �����������͋t��
	///</summary>
	template <typename NODE_DATA, typename EDGE>
	void Node<NODE_DATA, EDGE>::rfor_each_edge(const std::function<void(std::shared_ptr<EDGE>)>& execute_function)
	{
		for (std::set<std::shared_ptr<BasicEdge>, std::function<bool(std::shared_ptr<BasicEdge>, std::shared_ptr<BasicEdge>)>>::reverse_iterator iter = edge_list->rbegin(); iter != edge_list->rend(); iter++) {
			std::shared_ptr<EDGE> edge = std::dynamic_pointer_cast<EDGE>(*iter);
			execute_function(edge);
		}
	}

	///<summary>
	/// �e�G�b�W�ɂ���execute_function�����s����
	/// �����������͋t��
	///</summary>
	template <typename NODE_DATA, typename EDGE>
	void Node<NODE_DATA, EDGE>::rfot_each_edge(const std::function<void(std::shared_ptr<EDGE const>)>& execute_function) const
	{
		for (std::set<std::shared_ptr<BasicEdge>, std::function<bool(std::shared_ptr<BasicEdge>, std::shared_ptr<BasicEdge>)>>::const_reverse_iterator iter = edge_list->rbegin(); iter != edge_list->rend(); iter++) {
			std::shared_ptr<EDGE const> edge = std::dynamic_pointer_cast<EDGE const>(*iter);
			execute_function(edge);
		}
	}
}


