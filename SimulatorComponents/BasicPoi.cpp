#include "stdafx.h"
#include "BasicPoi.h"

namespace Map
{


	///<summary>
	/// �R���X�g���N�^
	///</summary>
	BasicPoi::BasicPoi(Graph::node_id id, Geography::PoiData data) : Graph::Node<Geography::PoiData, BasicRoad>(id, std::make_shared<Geography::PoiData>(data))
	{

	}


	///<summary>
	/// �R���X�g���N�^
	///</summary>
	BasicPoi::BasicPoi(Graph::node_id id, Geography::LatLng position, std::string venue_name, std::string category_id, std::string category_name) 
		: Graph::Node<Geography::PoiData, BasicRoad>(id, std::make_shared<Geography::PoiData>(venue_name, category_id, category_name, position))
	{

	}


	///<summary>
	/// �R�s�[�R���X�g���N�^
	///</summary>
	BasicPoi::BasicPoi(const BasicPoi& poi) : Graph::Node<Geography::PoiData, BasicRoad>(poi)
	{

	}


	///<summary>
	/// �f�X�g���N�^
	///</summary>
	BasicPoi::~BasicPoi()
	{
	}


	///<summary>
	/// �g�p�֎~�̂���
	///</summary>
	bool BasicPoi::connect_to(std::shared_ptr<BasicRoad> road)
	{
		return false;
	}

	///<summary>
	/// �J�e�S��ID�̎擾
	///</summary>
	const std::string BasicPoi::category_id() const
	{
		return data->get_category_id();
	}


	///<summary>
	/// �J�e�S�����̎擾
	///</summary>
	const std::string BasicPoi::category_name() const
	{
		return data->get_category_name();
	}


	///<summary>
	/// POI���̎擾
	///</summary>
	const std::string BasicPoi::name() const
	{
		return data->get_venue_name();
	}


	///<summary>
	/// �ܓx�̎擾 
	///</summary>
	double BasicPoi::lat() const
	{
		return data->get_position().lat();
	}


	///<summary>
	/// �o�x�̎擾
	///</summary>
	double BasicPoi::lng() const
	{
		return data->get_position().lng();
	}


	///<summary>
	/// �ڑ��֌W��ݒ肵�܂�
	/// �����̐ڑ��֌W���������ꍇ�͂�����폜���ĐV�����l��ݒ肵�܂�
	/// �Е��ł��ڑ������܂������Ȃ������ꍇ��false��Ԃ��܂�
	///</summary>
	bool BasicPoi::set_relation(Graph::node_id id1, double distance_to1, Graph::node_id id2, double distance_to2)
	{
		edge_list->clear();
		bool is_succeed = true;
		is_succeed = is_succeed && Graph::Node<Geography::PoiData, BasicRoad>::connect_to(std::make_shared<BasicRoad>(id1, distance_to1));
		is_succeed = is_succeed && Graph::Node<Geography::PoiData, BasicRoad>::connect_to(std::make_shared<BasicRoad>(id2, distance_to2));
		return is_succeed;
	}
	
	///<summary>
	/// �ڑ��֌W��ݒ肵�܂�
	/// �����̐ڑ��֌W���������ꍇ�͂�����폜���ĐV�����l��ݒ肵�܂�
	/// �Е��ł��ڑ������܂������Ȃ������ꍇ��false��Ԃ��܂�
	///</summary>
	bool BasicPoi::set_relation(std::pair<Graph::node_id, double> relation1, std::pair<Graph::node_id, double> relation2)
	{
		return set_relation(relation1.first, relation1.second, relation2.first, relation2.second);
	}


	///<summary>
	/// �ǂ�Node�̊Ԃɂ��邩���擾���܂�
	/// �אڃm�[�h��ID�̑g��Ԃ��܂�
	///</summary>
	std::pair<Graph::node_id, Graph::node_id> BasicPoi::between() const
	{
		std::set<std::shared_ptr<Graph::BasicEdge>, std::function<bool(std::shared_ptr<Graph::BasicEdge>, std::shared_ptr<Graph::BasicEdge>)>>::const_iterator iter = edge_list->begin();
		Graph::node_id id1 = INVALID;
		Graph::node_id id2 = INVALID;
		
		if (edge_list->size() != 2) return std::pair<Graph::node_id, Graph::node_id>(id1, id2);
		id1 = (*iter)->get_to();
		id2 = (*(++iter))->get_to();
		return std::pair<Graph::node_id, Graph::node_id>(id1, id2);
	}


	///<summary>
	/// to�܂ł̋������擾����
	///</summary>
	double BasicPoi::distance_to(const Graph::node_id& to) const
	{
		std::shared_ptr<BasicRoad const> road = get_static_edge_to(to);
		if (road == nullptr) return id == to ? 0.0 : DBL_MAX;
		return road->distance();
	}
}
