#ifdef SIMULATORCOMPONENTS_EXPORTS
#define _API __declspec(dllexport)
#else
#define _API __declspec(dllimport)
#endif

#pragma once
#include "Node.h"
#include "LatLng.h"
#include "BasicRoad.h"


namespace Map
{

	///<summary>
	/// 地図上のノードの基本クラス
	///</summary>
	class BasicMapNode : public Graph::Node<Geography::LatLng, BasicRoad>
	{
	public:
		BasicMapNode(Graph::node_id id, Geography::LatLng position);
		BasicMapNode(const BasicMapNode& node);
		~BasicMapNode();

		double lat() const;
		double lng() const;

		bool connect_to(Graph::node_id, double distance);
		bool connect_to(std::shared_ptr<BasicRoad> road);
		double distance_to_next_node(Graph::node_id to) const;
	};

	typedef BasicMapNode BasicIntersection;
}


