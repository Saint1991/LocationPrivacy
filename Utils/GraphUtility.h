#ifdef UTILS_EXPORTS
#define GRAPHUTILITY_API __declspec(dllexport)
#else
#define GRAPHUTILITY_API __declspec(dllimport)
#endif

#pragma once
#include "Coordinate.h"

namespace Graph 
{
	class GraphUtility
	{
	private:
		GraphUtility();
		~GraphUtility();
		
	public:
		GRAPHUTILITY_API static const std::vector<std::shared_ptr<Graph::Coordinate const>> convex_hull(std::vector<std::shared_ptr<Graph::Coordinate const>>& point_list);
		GRAPHUTILITY_API static double calc_convex_hull_size(std::vector<std::shared_ptr<Graph::Coordinate const>>& point_list);
	};
}


