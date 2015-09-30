#pragma once
#include "ProbabilityEdge.h"
#include "Node.h"

namespace User
{

	typedef std::string category_id;

	///<summary>
	/// PreferenceTree�ɗp����m�[�h
	///</summary>
	class PreferenceTreeNode : public Graph::Node<User::category_id, ProbabilityEdge>
	{
	protected:
		int counter;

	public:
		PreferenceTreeNode(Graph::node_id id, category_id category);
		PreferenceTreeNode(Graph::node_id id, std::shared_ptr<category_id> category);
		PreferenceTreeNode(const PreferenceTreeNode& node);
		~PreferenceTreeNode();
		const User::category_id category_id() const;
		int visit_count() const;
		void count_up();
	};
}


