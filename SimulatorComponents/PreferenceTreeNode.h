#pragma once
#include "ProbabilityEdge.h"
#include "Node.h"

namespace User
{

	typedef std::string category_id;

	///<summary>
	/// PreferenceTree‚É—p‚¢‚éƒm[ƒh
	///</summary>
	class PreferenceTreeNode : public Graph::Node<category_id, ProbabilityEdge>
	{
	protected:
		int counter;

	public:
		PreferenceTreeNode(Graph::node_id id, category_id category);
		PreferenceTreeNode(const PreferenceTreeNode& node);
		~PreferenceTreeNode();
		const category_id category_id() const;
	};
}


