
namespace Graph 
{
	///<summary>
	/// �R���X�g���N�^
	///</summary>
	///<param name='to'>�ڑ���̃m�[�hID</param>
	///<param name='data'>�G�b�W�̎������l</param>
	template <typename EDGE_DATA>
	Edge<EDGE_DATA>::Edge(const Graph::node_id& to, std::shared_ptr<EDGE_DATA> data) : BasicEdge(to), data(data)
	{
	}

	
	///<summary>
	/// �R�s�[�R���X�g���N�^
	///</summary>
	template <typename EDGE_DATA>
	Edge<EDGE_DATA>::Edge(const Edge<EDGE_DATA>& edge) :BasicEdge(edge._to)
	{
		EDGE_DATA copy = *edge.data;
		data = std::make_shared<EDGE_DATA>(copy);
	}

	
	///<summary>
	/// �f�X�g���N�^
	///</summary>
	template <typename EDGE_DATA>
	Edge<EDGE_DATA>::~Edge()
	{
	}

	///<summary>
	/// �f�[�^�̍X�V���s��
	///</summary>
	template <typename EDGE_DATA>
	void Edge<EDGE_DATA>::update_data(std::shared_ptr<EDGE_DATA> data)
	{
		this->data = data;
	}


	///<summary>
	/// �ύX�\�ȏ�ԂŃf�[�^�̃|�C���^���擾����
	///</summary>
	template <typename EDGE_DATA>
	std::shared_ptr<EDGE_DATA> Edge<EDGE_DATA>::get_data()
	{
		return data;
	}

	///<summary>
	/// �ύX�s�̏�ԂŃf�[�^�̃|�C���^���擾����
	///</summary>
	template <typename EDGE_DATA>
	std::shared_ptr<EDGE_DATA const> Edge<EDGE_DATA>::get_static_data() const
	{
		return data;
	}

	template <typename EDGE_DATA>
	bool Edge<EDGE_DATA>::operator==(const Edge<EDGE_DATA>& e) const
	{
		return _to == e.get_to();
	}

	template <typename EDGE_DATA>
	bool Edge<EDGE_DATA>::operator!=(const Edge<EDGE_DATA>& e) const
	{
		return _to != e.get_to();
	}
}

