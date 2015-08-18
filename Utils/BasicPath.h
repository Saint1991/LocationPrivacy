#ifdef UTILS_EXPORTS
#define BASIC_PATH_API __declspec(dllexport)
#else
#define BASIC_PATH_API __declspec(dllimport)
#endif

#pragma once
#include "Edge.h"
#include "BasicPathData.h"

namespace Graph
{

	///<summary>
	/// 距離付きのデータを持つパスデータ
	///</summary>
	class BASIC_PATH_API BasicPath : public Edge<Graph::BasicPathData>
	{
	public:
		BasicPath(const Graph::node_id& to, std::shared_ptr<Graph::BasicPathData> data);
		virtual ~BasicPath();
		const double distance() const;
		void update_distance(const double& distance);
	};
}


