#pragma once
#include "Node.h"
#include "BasicGeoNodeData.h"
#include "BasicPathData.h"
#include "GeoCalculation.h"


namespace Geography
{
	template <typename NODEDATA, typename EDGEDATA>
	class GeoNode : public Graph::Node<NODEDATA, EDGEDATA>
	{
	//NODEDATA‚ªBasicGeoNodeData‚ğŒp³‚µ‚Ä‚¢‚é‚©”»’è
	static_assert(std::is_base_of<BasicGeoNodeData, NODEDATA>::value, "Template Type NODEDATA is not Derived from BasicGeoNodeData at GeoNode.h");

	//EDGEDATA‚ªBasicPathData‚ğŒp³‚µ‚Ä‚¢‚é‚©”»’è
	static_assert(std::is_base_of<BasicPathData, EDGEDATA>::value, "Template Type EDGEDATA is not derived from BasicPathData at GeoNode.h");

	public:
		GeoNode(Graph::node_id id, NODEDATA data);
		virtual ~GeoNode();
		
		friend double distance(const Geography::GeoNode<NODEDATA, EDGEDATA>& node1, const Geography::GeoNode<NODEDATA, EDGEDATA>& node2);
		friend double azimuth_angle(const Geography::GeoNode<NODEDATA, EDGEDATA>& from, const Geography::GeoNode<NODEDATA, EDGEDATA>& to);
	};
}

#include "GeoNode.hpp"