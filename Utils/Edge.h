#ifdef UTILS_EXPORTS
#define EDGE_API __declspec(dllexport)
#else
#define EDGE_API __declspec(dllimport)
#endif

#pragma once

namespace Graph {

	typedef unsigned int node_id;

	///<summary>
	/// �O���t�̐ڑ����Ǘ�����G�b�W�̃e���v���[�g�N���X
	/// �R���p�C���̎d�l�ケ���Ɏ��������������Ȃ�...
	///</summary>
	template <typename EDGE_DATA>
	class Edge
	{
	public:
		node_id to;
		std::shared_ptr<EDGE_DATA> data;

		///<summary>
		/// �R���X�g���N�^
		///</summary>
		///<param name='to'>�ڑ���̃m�[�hID</param>
		///<param name='data'>�G�b�W�̎������l</param>
		Edge(node_id to, EDGE_DATA data) : to(to), data(std::make_shared<EDGE_DATA>(data)) {}
		
		///<summary>
		/// �f�X�g���N�^
		///</summary>
		virtual ~Edge() { }

		friend bool operator==(Edge<EDGE_DATA> e1, Edge<EDGE_DATA> e2) { return e1.to == e2.to; }
		friend bool operator!=(Edge<EDGE_DATA> e1, Edge<EDGE_DATA> e2) { return !(e1 == e2); }
	};
}



