#ifdef SIMULATORCOMPONENTS_EXPORTS
#define BASIC_INTERSECTION_API __declspec(dllexport)
#else
#defineBASIC_INTERSECTION_API __declspec(dllimport)
#endif

#pragma once
#include "BasicMapNode.h"

namespace Map
{

	///<summary>
	/// �����_�m�[�h��\���N���X
	///</summary>
	class BASIC_INTERSECTION_API BasicIntersection : BasicMapNode
	{
	public:
		BasicIntersection(Graph::node_id id, Geography::LatLng position);
		virtual ~BasicIntersection();
	};
}


