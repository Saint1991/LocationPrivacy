#pragma once

namespace Graph {

	typedef unsigned long node_id;
	const node_id NOWHERE = ULONG_MAX;

	///<summary>
	/// グラフの接続を管理するエッジのテンプレートクラス
	/// コンパイラの仕様上ここに実装も書くしかない...
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