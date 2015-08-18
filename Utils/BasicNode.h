#pragma once
#include "Node.h"
#include "BasicPath.h"

namespace Graph 
{
	///<summary>
	/// エッジが距離を持っているノード用テンプレート
	///</summary>
	template <typename NODE_DATA>
	class BasicNode : public Node<NODE_DATA, BasicPath>
	{
	public:
		BasicNode(const node_id& id, std::shared_ptr<NODE_DATA> data);
		BasicNode(const BasicNode& node);
		virtual ~BasicNode();

		bool update_distance_to(const node_id& to, const double& distance);
		const double distance_to(const node_id& to) const;
	};
}

#include "BasicNode.hpp"
