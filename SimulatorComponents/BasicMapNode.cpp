#include "stdafx.h"
#include "BasicMapNode.h"

namespace Map
{

	///<summary>
	/// �R���X�g���N�^

	///</summary>
	BasicMapNode::BasicMapNode(Graph::node_id id, Geography::LatLng position) 
		: Graph::Node<Geography::LatLng, BasicRoad>(id, std::make_shared<Geography::LatLng>(position))
	{
	}


	///<summary>
	/// �R�s�[�R���X�g���N�^
	///</summary>
	BasicMapNode::BasicMapNode(const BasicMapNode& node) : Graph::Node<Geography::LatLng, BasicRoad>(node)
	{
	}

	///<summary>
	/// �f�X�g���N�^
	///</summary>
	BasicMapNode::~BasicMapNode()
	{
	}


	///<summary>
	/// �ܓx���擾
	///</summary>
	double BasicMapNode::lat() const
	{
		return data->lat();
	}

	///<summary>
	/// �o�x���擾
	///</summary>
	double BasicMapNode::lng() const
	{
		return data->lng();
	}


	///<summary>
	/// LatLng���擾����
	///</summary>
	Geography::LatLng BasicMapNode::get_point() const
	{
		return *data;
	}

	///<summary>
	/// to�ɑ΂��ċ���distance�̃G�b�W���쐬
	///</summary>
	bool BasicMapNode::connect_to(Graph::node_id to, double distance)
	{
		return Graph::Node<Geography::LatLng, BasicRoad>::connect_to(std::make_shared<BasicRoad>(to, distance));
	}

	///<summary>
	/// to�ɑ΂��ċ���distance�̃G�b�W���쐬
	///</summary>
	bool BasicMapNode::connect_to(std::shared_ptr<BasicRoad> road)
	{
		return Graph::Node<Geography::LatLng, BasicRoad>::connect_to(road);
	}


	///<summary>
	/// to�̃m�[�h�܂ł̋������擾���܂�
	/// �������אڂ��Ă��Ȃ��ꍇ��LONG_MAX��Ԃ��܂�
	///</summary>
	double BasicMapNode::distance_to_next_node(Graph::node_id to) const
	{
		std::shared_ptr<BasicRoad const> road = get_static_edge_to(to);
		if (road == nullptr) return LONG_MAX;
		return road->distance();
	}
}

