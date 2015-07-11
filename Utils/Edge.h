#ifdef UTILS_EXPORTS
#define EDGE_API __declspec(dllexport)
#else
#define EDGE_API __declspec(dllimport)
#endif

#pragma once

namespace Graph {

	typedef unsigned long node_id;

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

///<summary>
/// コンストラクタ
///</summary>
///<param name='to'>接続先のノードID</param>
///<param name='data'>エッジの持つ属性値</param>
template <typename EDGE_DATA>
Graph::Edge<EDGE_DATA>::Edge(Graph::node_id to, EDGE_DATA data) : to(to), data(std::make_shared<EDGE_DATA>(data))
{
}


///<summary>
/// デストラクタ
///</summary>
template <typename EDGE_DATA>
Graph::Edge<EDGE_DATA>::~Edge()
{
}


///<summary>
/// リンク先のIDを取得する．
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