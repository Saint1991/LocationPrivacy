#ifdef UTILS_EXPORTS
#define BASIC_EDGE_API __declspec(dllexport)
#else
#define BASIC_EDGE_API __declspec(dllimport)
#endif
#pragma once

namespace Graph
{

	typedef unsigned long node_id;
	const node_id NOWHERE = ULONG_MAX;

	///<summary>
	/// 接続先を示すエッジの基本データ
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
	};
}


