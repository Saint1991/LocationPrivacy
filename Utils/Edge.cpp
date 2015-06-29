#include "stdafx.h"
#include "Edge.h"

///<summary>
/// �R���X�g���N�^
///</summary>
///<param name='to'>�ڑ���̃m�[�hID</param>
///<param name='data'>�G�b�W�̎������l</param>
template <typename EDGE_DATA>
Graph::Edge<EDGE_DATA>::Edge(Graph::node_id to, EDGE_DATA data) : to(to), data(std::make_shared(data))
{
}


///<summary>
/// �f�X�g���N�^
///</summary>
template <typename EDGE_DATA>
Graph::Edge<EDGE_DATA>::~Edge()
{
}

template <typename EDGE_DATA>
bool operator==(Graph::Edge<EDGE_DATA> e1, Graph::Edge<EDGE_DATA> e2)
{
	return e1.to == e2.to;
}

template <typename EDGE_DATA>
bool operator!=(Graph::Edge<EDGE_DATA> e1, Graph::Edge<EDGE_DATA> e2)
{
	return !(e1 == e2)
}