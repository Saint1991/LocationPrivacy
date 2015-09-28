#pragma once
#include "BasicEdge.h"
#include "Identifiable.h"

namespace Graph 
{

	///<summary>
	/// グラフにおけるノードを表すテンプレートクラス
	/// EDGEにはBasicEdgeから派生したクラスのみ指定可能
	///</summary>
	template <typename NODE_DATA, typename EDGE>
	class Node : public Identifiable<node_id>
	{

	//EDGEがBasicEdgeから派生したクラスかを判定
	static_assert(std::is_base_of<BasicEdge, EDGE>::value, "Template type EDGE must be derived from BasicEdge");
	
	protected:
		std::shared_ptr<	std::set<std::shared_ptr<BasicEdge>, std::function<bool(std::shared_ptr<BasicEdge>, std::shared_ptr<BasicEdge>)>> > edge_list;

	public:
		std::shared_ptr<NODE_DATA> data;

		Node(node_id id, std::shared_ptr<NODE_DATA> data);
		Node(const Node<NODE_DATA, EDGE>& node);
		virtual ~Node();

		std::shared_ptr<EDGE const> get_static_edge_to(node_id id) const;
		std::shared_ptr<EDGE> get_edge_to(node_id id);

		virtual bool connect_to(std::shared_ptr<EDGE> edge);
		bool disconnect_from(node_id from);
		
		bool is_connecting_to(node_id id) const;
		std::vector<node_id> get_connecting_node_list() const;

		void for_each_edge(const std::function<void(std::shared_ptr<EDGE>)>& execute_function);
		void for_each_edge(const std::function<void(std::shared_ptr<EDGE const>)>& execute_function) const;
		void rfor_each_edge(const std::function<void(std::shared_ptr<EDGE>)>& execute_function);
		void rfot_each_edge(const std::function<void(std::shared_ptr<EDGE const>)>& execute_function) const;
	};
}
#include "Node.hpp"

