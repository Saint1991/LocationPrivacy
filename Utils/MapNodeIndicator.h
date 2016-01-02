#ifdef UTILS_EXPORTS
#define MAP_NODE_INDICATOR_API __declspec(dllexport)
#else
#define MAP_NODE_INDICATOR_API __declspec(dllimport)
#endif

#pragma once
#include "BasicEdge.h"

namespace Graph
{

	enum NodeType { INTERSECTION, POI, OTHERS, INVALID };

	///<summary>
	/// Map上のNodeを指し示すためのクラス
	/// IDと種別で表される
	///</summary>
	struct MAP_NODE_INDICATOR_API MapNodeIndicator
	{
	private:
		Graph::node_id _id;
		NodeType _type;

	public:
		MapNodeIndicator();
		MapNodeIndicator(Graph::node_id id, NodeType type = POI);
		~MapNodeIndicator();

		Graph::node_id id() const;
		NodeType type() const;

		bool operator==(const MapNodeIndicator& id) const;
		bool operator!=(const MapNodeIndicator& id) const;
	};

}

#pragma region Hash
namespace std
{
	template <>
	struct hash<Graph::MapNodeIndicator>
	{
		size_t operator()(const Graph::MapNodeIndicator& id) const
		{
			return (hash<Graph::node_id>()(id.id()) ^ (hash<Graph::NodeType>()(id.type()) << 1));
		}
	};
}
#pragma endregion MapNodeIndicatorをunordered_mapのキーに指定できるようにする


