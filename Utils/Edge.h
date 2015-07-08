#ifdef UTILS_EXPORTS
#define EDGE_API __declspec(dllexport)
#else
#define EDGE_API __declspec(dllimport)
#endif

#pragma once

namespace Graph {

	typedef unsigned int node_id;

	template <typename EDGE_DATA>
	class Edge
	{
	protected:
		std::shared_ptr<EDGE_DATA> data;
	
	public:
		node_id to;
		EDGE_API Edge(node_id to, EDGE_DATA data);
		EDGE_API virtual ~Edge();

		EDGE_API friend bool operator==(Edge<EDGE_DATA> e1, Edge<EDGE_DATA> e2);
		EDGE_API friend bool operator!=(Edge<EDGE_DATA> e1, Edge<EDGE_DATA> e2);
	};
}



