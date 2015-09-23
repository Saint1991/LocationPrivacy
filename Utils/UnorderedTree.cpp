#include "stdafx.h"
#include "UnorderedTree.h"

namespace Graph
{

	///<summary>
	/// コンストラクタ
	///</summary>
	template <typename ID_TYPE, typename NODE, typename EDGE>
	UnorderedTree<ID_TYPE, NODE, EDGE>::UnorderedTree()
	{
	}


	///<summary>
	/// デストラクタ
	///</summary>
	template <typename ID_TYPE, typename NODE, typename EDGE>
	UnorderedTree<ID_TYPE, NODE, EDGE>::~UnorderedTree()
	{
	}


	///<summary>
	/// ルートノードを設定して木を初期化します
	///</summary>
	template <typename ID_TYPE, typename NODE, typename EDGE>
	void UnorderedTree<ID_TYPE, NODE, EDGE>::initialize(NODE node)
	{
		std::shared_ptr<NODE> root_ptr = std::make_shared<NODE>(node);
		root_node = root_ptr;
		node_collection->add(root_node);
		connected_ids.insert(node.get_id());
	}


	///<summary>
	/// イテレータがさしているノードの子ノードとして
	///</summary>
	template <typename ID_TYPE, typename NODE, typename EDGE>
	void UnorderedTree<ID_TYPE, NODE, EDGE>::insert(depth_first_iterator iter, NODE node)
	{
		
	}
}

