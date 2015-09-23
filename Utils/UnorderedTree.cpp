#include "stdafx.h"
#include "UnorderedTree.h"

namespace Graph
{

	///<summary>
	/// コンストラクタ
	///</summary>
	template <typename NODE, typename NODE_DATA, typename EDGE>
	UnorderedTree<NODE, NODE_DATA, EDGE>::UnorderedTree()
	{
	}


	///<summary>
	/// デストラクタ
	///</summary>
	template <typename NODE, typename NODE_DATA, typename EDGE>
	UnorderedTree<NODE, NODE_DATA, EDGE>::~UnorderedTree()
	{
	}


	///<summary>
	/// ルートノードを設定して木を初期化します
	///</summary>
	template <typename NODE, typename NODE_DATA, typename EDGE>
	void UnorderedTree<NODE, NODE_DATA, EDGE>::initialize(NODE node)
	{
		std::shared_ptr<NODE> root_ptr = std::make_shared<NODE>(node);
		root_node = root_ptr;
		node_collection->add(root_node);
		connected_ids.insert(node.get_id());
	}


	///<summary>
	/// イテレータがさしているノードの子ノードとして
	///</summary>
	template <typename NODE, typename NODE_DATA, typename EDGE>
	void UnorderedTree<NODE, NODE_DATA, EDGE>::insert(depth_first_iterator iter, NODE node)
	{
		
	}
}

