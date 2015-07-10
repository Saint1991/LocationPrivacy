#ifdef UTILS_EXPORTS
#define EDGE_API __declspec(dllexport)
#else
#define EDGE_API __declspec(dllimport)
#endif

#pragma once

namespace Graph {

	typedef unsigned int node_id;

	///<summary>
	/// グラフの接続を管理するエッジのテンプレートクラス
	/// コンパイラの仕様上ここに実装も書くしかない...
	///</summary>
	template <typename EDGE_DATA>
	class Edge
	{
	public:
		node_id to;
		std::shared_ptr<EDGE_DATA> data;

		///<summary>
		/// コンストラクタ
		///</summary>
		///<param name='to'>接続先のノードID</param>
		///<param name='data'>エッジの持つ属性値</param>
		Edge(node_id to, EDGE_DATA data) : to(to), data(std::make_shared<EDGE_DATA>(data)) {}
		
		///<summary>
		/// デストラクタ
		///</summary>
		virtual ~Edge() { }

		friend bool operator==(Edge<EDGE_DATA> e1, Edge<EDGE_DATA> e2) { return e1.to == e2.to; }
		friend bool operator!=(Edge<EDGE_DATA> e1, Edge<EDGE_DATA> e2) { return !(e1 == e2); }
	};
}



