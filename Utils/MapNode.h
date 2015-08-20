#ifdef UTILS_EXPORTS
#define MAP_NODE_API __declspec(dllexport)
#else
#define MAP_NODE_API __declspec(dllimport)
#endif

#pragma once
#include "LatLng.h"
#include "GeoCalculation.h"
#include "BasicNode.h"

namespace  Geography
{
	class MAP_NODE_API MapNode : public Graph::BasicNode<LatLng>
	{
	public:
		MapNode(const Graph::node_id& id, std::shared_ptr<LatLng> position);
		MapNode(const MapNode& node);
		virtual ~MapNode();

		MAP_NODE_API friend double distance(const MapNode& node1, const MapNode& node2);
		MAP_NODE_API friend double azimuth_angle(const MapNode& from, const MapNode& node2);
	};

}

