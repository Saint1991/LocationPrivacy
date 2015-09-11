#ifdef SIMULATORCOMPONENTS_EXPORTS
#define BASIC_ROAD_API __declspec(dllexport)
#else
#define BASIC_ROAD_API __declspec(dllimport)
#endif

#pragma once
#include "Edge.h"
#include "BasicPathData.h"

namespace Map
{

	///<summary>
	/// 距離付きのデータを持つパスデータ
	///</summary>
	class BASIC_ROAD_API BasicRoad : public Graph::Edge<Graph::BasicPathData>
	{
	public:
		BasicRoad(Graph::node_id to, double distance);
		BasicRoad(Graph::node_id to, std::shared_ptr<Graph::BasicPathData> data);
		BasicRoad(const BasicRoad& road);
		virtual ~BasicRoad();
		double distance() const;
		void update_distance(const double& distance);
	};
}


