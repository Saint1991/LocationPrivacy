#pragma once
#include "stdafx.h"
#include "Edge.h"
#include "Node.h"
#include "IdentifiableCollection.h"

namespace Graph 
{

	constexpr node_id NO_CONNECTION = LONG_MAX;

	///<summary>
	/// ���[�e�B���O�e�[�u���쐬�̃A���S���Y���p�C���^�[�t�F�[�X
	/// NODE�ɂ�BasicPathData�̔h���N���X���G�b�W�Ɏ���Node�N���X���p�������N���X�������ĂȂ��̂Œ���
	///</summary>
	template <typename NODE>
	class RoutingMethod
	{	
	private:
		double distance(const node_id& from, const node_id& to);

	protected:
		std::shared_ptr < const Collection::IdentifiableCollection<std::shared_ptr<NODE const>> node_collection;
		std::unique_ptr<std::vector<std::vector<double>>> distance_map;
		void create_distance_map();
	
	public:
		void set_node_collection(std::shared_ptr < const Collection::IdentifiableCollection<std::shared_ptr<NODE const>> node_collection);
		virtual std::unique_ptr<const std::vector<const std::vector<node_id>>> create_routing_table() const = 0;
	};
}

#include "RoutingMethod.hpp"