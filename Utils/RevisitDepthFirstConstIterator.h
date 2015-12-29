#pragma once
#include "stdafx.h"
#include "BaseConstIterator.h"

namespace Graph
{

	///<summary>
	/// �����m�[�h�ɍĖK�₷��^�C�v�̐[���D��T���p�C�e���[�^
	///</summary>
	template <typename NODE, typename NODE_DATA, typename EDGE>
	class RevisitDepthFirstConstIterator : public BaseConstIterator<NODE, NODE_DATA, EDGE>
	{
	protected:
		std::stack<node_id> stack;

	public:
		RevisitDepthFirstConstIterator(node_id id, std::shared_ptr<Collection::IdentifiableCollection<node_id, NODE>> node_collection);
		~RevisitDepthFirstConstIterator();
		RevisitDepthFirstConstIterator<NODE, NODE_DATA, EDGE>& operator++(int);
	};
}

#include "RevisitDepthFirstConstIterator.hpp"
