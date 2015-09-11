#ifdef SIMULATORCOMPONENTS_EXPORTS
#define BASIC_POI_API __declspec(dllexport)
#else
#define BASIC_POI_API __declspec(dllimport)
#endif

#pragma once
#include "Node.h"
#include "BasicRoad.h"
#include "PoiData.h"

namespace Map
{

	///<summary>
	/// POI��\���N���X
	/// connect_to�̓I�[�o�[���C�h���Ďg�p�֎~�ɂ��Ă��邪�C
	/// �|�����[�t�B�Y���ł��Ȃ��Ȃ�悤�ł���΂���false��Ԃ����\�b�h�Ƃ���public�ɂ��邱�Ƃ���������
	///</summary>
	class BASIC_POI_API BasicPoi : public Graph::Node<Geography::PoiData, BasicRoad>
	{
	private:
		bool connect_to(std::shared_ptr<BasicRoad> road);
	public:
		
		BasicPoi(Graph::node_id id, Geography::PoiData data);
		BasicPoi(Graph::node_id id, Geography::LatLng position, std::string venue_name, std::string category_id, std::string category_name);
		BasicPoi(const BasicPoi& poi);
		~BasicPoi();

		const std::string category_id() const;
		const std::string category_name() const;
		const std::string name() const;
		double lat() const;
		double lng() const;

		bool set_relation(Graph::node_id id1, double distance_to1, Graph::node_id id2, double distance_to2);
		bool set_relation(std::pair<Graph::node_id, double> relation1, std::pair<Graph::node_id, double> relation2);

		std::pair<Graph::node_id, Graph::node_id> between() const;
	};
}


