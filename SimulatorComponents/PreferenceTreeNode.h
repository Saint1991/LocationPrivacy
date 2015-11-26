#pragma once
#include "BasicEdge.h"
#include "Node.h"

namespace User
{

	typedef std::string category_id;
	constexpr char* ANY = "ANY";

	///<summary>
	/// PreferenceTreeÇ…ópÇ¢ÇÈÉmÅ[Éh
	///</summary>
	class PreferenceTreeNode : public Graph::Node<User::category_id, Graph::BasicEdge>
	{
	protected:
		double counter;
		Graph::node_id parent;
		int depth;

	public:
		PreferenceTreeNode(Graph::node_id id, Graph::node_id parent, unsigned int depth, const category_id& category);
		PreferenceTreeNode(Graph::node_id id, Graph::node_id parent, unsigned int depth,  std::shared_ptr<category_id> category);
		PreferenceTreeNode(const PreferenceTreeNode& node);
		~PreferenceTreeNode();
		
		const User::category_id category_id() const;
		double count() const;
		void count_up(double add_num = 1.0);
		bool is_leef() const;
		Graph::node_id get_parent() const;
		int get_depth() const;
		std::vector<Graph::node_id> get_children() const;
	};
}


