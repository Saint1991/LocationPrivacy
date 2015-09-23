#ifdef UTILS_EXPORTS
#define BASIC_EDGE_API __declspec(dllexport)
#else
#define BASIC_EDGE_API __declspec(dllimport)
#endif
#pragma once

namespace Graph
{

	typedef long node_id;
	constexpr node_id NOWHERE = -1L;
	constexpr node_id SELF = -2L;

	///<summary>
	/// �ڑ���������G�b�W�̊�{�f�[�^
	///</summary>
	class BASIC_EDGE_API BasicEdge
	{
	protected:
		node_id _to;

	public:
		BasicEdge(const node_id& to);
		virtual ~BasicEdge();
		
		node_id get_to() const;
		bool operator==(const BasicEdge& e) const;
		bool operator!=(const BasicEdge& e) const;
		bool operator==(const node_id& id) const;
		bool operator!=(const node_id& id) const;
	};
}


