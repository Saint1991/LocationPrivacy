
///<summary>
/// �R���X�g���N�^
///</summary>
///<param name='to'>�ڑ���̃m�[�hID</param>
///<param name='data'>�G�b�W�̎������l</param>
template <typename EDGE_DATA>
Graph::Edge<EDGE_DATA>::Edge(Graph::node_id to, EDGE_DATA data) : to(to), data(std::make_shared<EDGE_DATA>(data))
{
}


///<summary>
/// �f�X�g���N�^
///</summary>
template <typename EDGE_DATA>
Graph::Edge<EDGE_DATA>::~Edge()
{
}


///<summary>
/// �����N���ID���擾����D
///</summary>
template <typename EDGE_DATA>
Graph::node_id Graph::Edge<EDGE_DATA>::get_to() const
{
	return to;
}

template <typename EDGE_DATA>
bool Graph::Edge<EDGE_DATA>::operator==(Graph::Edge<EDGE_DATA> e) const
{
	return to == e.to;
}

template <typename EDGE_DATA>
bool Graph::Edge<EDGE_DATA>::operator!=(Graph::Edge<EDGE_DATA> e) const
{
	return to != e.to;
}