#include "stdafx.h"
#include "MapNode.h"


namespace Geography 
{
	
	///<summary>
	/// �R���X�g���N�^
	///</summary>
	MapNode::MapNode(const Graph::node_id& id, std::shared_ptr<LatLng> position) : Graph::BasicNode<LatLng>(id, position)
	{
	}


	///<summary>
	/// �R�s�[�R���X�g���N�^
	///</summary>
	MapNode::MapNode(const MapNode& node) : BasicNode<LatLng>(node)
	{
	}

	///<summary>
	/// �f�X�g���N�^
	///</summary>
	MapNode::~MapNode()
	{
	}

	
	///<summary>
	/// �m�[�h�Ԃ̋����v�Z(m)
	///</summary>
	double Geography::distance(const MapNode& node1, const MapNode& node2) 
	{
		return GeoCalculation::lambert_distance(*node1.data, *node2.data);
	}

	
	///<summary>
	/// �m�[�hfrom���猩���m�[�hto�̕��ʊp�̌v�Z
	/// (��0�x�C��90�x�C��180�x�C�k270�x�C�P�ʂ̓��W�A��)
	///</summary>
	double Geography::azimuth_angle(const MapNode& from, const MapNode& to)
	{
		return GeoCalculation::lambert_azimuth_angle(*from.data, *to.data);
	}
}

