#ifdef SIMULATORCOMPONENTS_EXPORTS
#define MAP_NODE_ID_INDICATOR_API __declspec(dllexport)
#else
#define MAP_NODE_ID_INDICATOR_API __declspec(dllimport)
#endif

#pragma once
#include "BasicEdge.h"

namespace Map
{

	enum NodeType {INTERSECTION, POI, INVALID};


	///<summary>
	/// Map���Node���w���������߂̃N���X
	/// ID�Ǝ�ʂŕ\�����
	///</summary>
	struct MAP_NODE_ID_INDICATOR_API MapNodeIdIndicator
	{
	private:
		Graph::node_id _id;
		NodeType _type;

	public:
		MapNodeIdIndicator(Graph::node_id id, NodeType type = POI);
		~MapNodeIdIndicator();

		Graph::node_id id() const;
		NodeType type() const;
	};
}


