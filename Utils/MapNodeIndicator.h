#ifdef UTILS_EXPORTS
#define MAP_NODE_INDICATOR_API __declspec(dllexport)
#else
#define MAP_NODE_INDICATOR_API __declspec(dllimport)
#endif

#pragma once
#include "BasicEdge.h"
#include "FileExporter.h"
namespace Graph
{

	enum NodeType { INTERSECTION, POI, OTHERS, INVALID };

	///<summary>
	/// Map上のNodeを指し示すためのクラス
	/// IDと種別で表される
	///</summary>
	struct MAP_NODE_INDICATOR_API MapNodeIndicator : public IO::FileExportable
	{
	private:
		Graph::node_id _id;
		NodeType _type;

	public:
		static constexpr char* NODE_ID = "NodeID";
		static constexpr char*  NODE_TYPE = "NodeType";

		MapNodeIndicator();
		MapNodeIndicator(Graph::node_id id, NodeType type = POI);
		~MapNodeIndicator();

		Graph::node_id id() const;
		NodeType type() const;

		bool operator==(const MapNodeIndicator& id) const;
		bool operator!=(const MapNodeIndicator& id) const;

		std::unordered_map<std::string, std::string> get_export_data() const;
	};
}


