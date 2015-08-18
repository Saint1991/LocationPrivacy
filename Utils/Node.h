#pragma once
#include "BasicEdge.h"
#include "Edge.h"
#include "Identifiable.h"

namespace Graph 
{

	///<summary>
	/// �O���t�ɂ�����m�[�h��\���e���v���[�g�N���X
	/// EDGE�ɂ�BasicEdge����h�������N���X�̂ݎw��\
	///</summary>
	template <typename NODE_DATA, typename EDGE>
	class Node : public Identifiable
	{

	//EDGE��BasicEdge����h�������N���X���𔻒�
	static_assert(std::is_base_of<BasicEdge, EDGE>::value, "Template type EDGE is not derived from BasicEdge");
	
	protected:
		std::unique_ptr<std::list<std::shared_ptr<EDGE>>> edge_list;

	public:
		std::shared_ptr<NODE_DATA> data;

		Node(node_id id, std::shared_ptr<NODE_DATA> data);
		Node(const Node& node);
		virtual ~Node();

		std::shared_ptr<EDGE const> get_static_edge_to(node_id id) const;
		std::shared_ptr<EDGE> get_edge_to(node_id id);

		bool connect_to(std::shared_ptr<EDGE> edge);
		bool disconnect_from(node_id from);
		
		bool is_connecting_to(node_id id) const;
		std::list<node_id> get_connecting_node_list() const;

		bool operator==(Graph::Node<NODE_DATA, EDGE> node) const;
		bool operator!=(Graph::Node<NODE_DATA, EDGE> node) const;
	};
}
#include "Node.hpp"

