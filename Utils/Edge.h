#pragma once
#include "stdafx.h"
#include "BasicEdge.h"




namespace Graph 
{

	///<summary>
	/// �O���t�̐ڑ����Ǘ�����G�b�W�̃e���v���[�g�N���X
	/// �C�ӂ̑����f�[�^�̃|�C���^�����Ă�
	///</summary>
	template <typename EDGE_DATA>
	class Edge : public BasicEdge
	{
	protected:
		std::shared_ptr<EDGE_DATA> data;
	
	public:	
		Edge(const node_id& to, std::shared_ptr<EDGE_DATA> data);
		Edge(const Edge& edge);
		virtual ~Edge();
		void update_data(std::shared_ptr<EDGE_DATA> data);
		std::shared_ptr<EDGE_DATA> get_data();
		std::shared_ptr<EDGE_DATA const> get_static_data() const;
		bool operator==(const Edge<EDGE_DATA>& e) const;
		bool operator!=(const Edge<EDGE_DATA>& e) const;
	};

}

#include "Edge.hpp"