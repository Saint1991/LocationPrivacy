#ifdef UTILS_EXPORTS
#define EDGE_API __declspec(dllexport)
#else
#define EDGE_API __declspec(dllimport)
#endif

#pragma once

namespace Graph {

	typedef unsigned long node_id;

	///<summary>
	/// �O���t�̐ڑ����Ǘ�����G�b�W�̃e���v���[�g�N���X
	/// �R���p�C���̎d�l�ケ���Ɏ��������������Ȃ�...
	///</summary>
	template <typename EDGE_DATA>
	class Edge
	{
	protected:
		Graph::node_id to;
	
	public:	
		std::shared_ptr<EDGE_DATA> data;
		
		Edge(node_id to, EDGE_DATA data);
		virtual ~Edge();
		node_id get_to() const;
		bool operator==(Edge<EDGE_DATA> e) const;
		bool operator!=(Edge<EDGE_DATA> e) const;
	};
}

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