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

#include "Edge.hpp"