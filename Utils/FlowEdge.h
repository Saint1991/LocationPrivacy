#ifdef UTILS_EXPORTS
#define FLOW_EDGE_API __declspec(dllexport)
#else
#define FLOW_EDGE_API __declspec(dllimport)
#endif

#pragma once
#include "Edge.h"

namespace Graph
{
	class FLOW_EDGE_API FlowEdge : public Edge<double>
	{
	public:
		FlowEdge(node_id id, double initial_flow = 1.0);
		FlowEdge(node_id id, std::shared_ptr<double> initial_flow);
		~FlowEdge();

		void flow_in(double amount);
		void flow_out(double amount);
		double get_flow() const;
		void set_flow(double amount);
	};
}


