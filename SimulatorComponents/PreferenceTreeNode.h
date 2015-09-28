#pragma once
#include "ProbabilityEdge.h"
#include "Node.h"

namespace User
{

	///<summary>
	/// PreferenceTree‚É—p‚¢‚éƒm[ƒh
	///</summary>
	class PreferenceTreeNode : public Graph::Node<std::string, ProbabilityEdge>
	{
	protected:
		int counter;

	public:
		PreferenceTreeNode(Graph::node_id id, std::string category);
		PreferenceTreeNode(const PreferenceTreeNode& node);
		~PreferenceTreeNode();
		const std::string category_id() const;
	};
}


