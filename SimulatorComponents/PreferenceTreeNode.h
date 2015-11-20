#pragma once
#include "BasicEdge.h"
#include "Node.h"

namespace User
{

	typedef std::string category_id;

	///<summary>
	/// PreferenceTree�ɗp����m�[�h
	///</summary>
	class PreferenceTreeNode : public Graph::Node<User::category_id, Graph::BasicEdge>
	{
	protected:
		double counter;
		Graph::node_id parent;

	public:
		PreferenceTreeNode(Graph::node_id id, Graph::node_id parent, const category_id& category);
		PreferenceTreeNode(Graph::node_id id, Graph::node_id parent,  std::shared_ptr<category_id> category);
		PreferenceTreeNode(const PreferenceTreeNode& node);
		~PreferenceTreeNode();
		
		const User::category_id category_id() const;
		int count() const;
		void count_up(double add_num = 1.0);
		bool is_leef() const;
		Graph::node_id get_parent() const;
		std::vector<Graph::node_id> get_children() const;
	};
}


