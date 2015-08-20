
namespace Graph 
{

	///<summary>
	/// �R���X�g���N�^
	///</summary>
	template <typename NODE_DATA>
	 BasicNode<NODE_DATA>::BasicNode(const  node_id& id, std::shared_ptr<NODE_DATA> data) :  Node<NODE_DATA, BasicPath>(id, data)
	{
	}

	///<summary>
	/// �R�s�[�R���X�g���N�^
	///</summary>
	template <typename NODE_DATA>
	BasicNode<NODE_DATA>::BasicNode(const  BasicNode& node) : Node<NODE_DATA, BasicPath>(node)
	{
	}

	///<summary>
	/// �f�X�g���N�^
	///</summary>
	template <typename NODE_DATA>
	 BasicNode<NODE_DATA>::~BasicNode()
	{
	}

	 ///<summary>
	 /// to�܂ł̋������X�V����
	 /// to�ւ̃G�b�W��ێ����Ă��Ȃ��ꍇ��false��Ԃ�
	 ///</summary>
	template <typename NODE_DATA>
	bool  BasicNode<NODE_DATA>::update_distance_to(const node_id& to, const double& distance)
	{
		std::shared_ptr<BasicPath> target_path = get_edge_to(to);
		if (target_path == nullptr) return false;
		target_path->update_distance(distance);
		return true;
	}

	///<summary>
	/// to�܂ł̋������擾����
	/// �ڑ����Ă��Ȃ��m�[�h�Ȃ�DBL_MAX���Ԃ����
	///</summary>
	template <typename NODE_DATA>
	const double BasicNode<NODE_DATA>::distance_to(const node_id& to) const
	{
		std::shared_ptr<BasicPath const> target_path = get_static_edge_to(to);
		if (target_path == nullptr) return DBL_MAX;
		return target_path->distance();
	}

}
