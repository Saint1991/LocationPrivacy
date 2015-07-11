#ifdef UTILS_EXPORTS
#define NODEMANAGER_API __declspec(dllexport)
#else
#define NODEMANAGER_API __declspec(dllimport)
#endif

#pragma once
#include "Edge.h"
#include "Node.h"
#include "IdentifiableCollection.h"


namespace Graph {

	///<summary>
	/// ノードの生成，接続関係の構成を行った後IdentifiableCollectionを返すファクトリテンプレート
	/// 生成後はクラスの内部状態は初期化されるので注意
	/// コンパイラの仕様上実装もここに書くしかない...
	///</summary>
	template <typename NODE_DATA, typename EDGE_DATA>
	class NodeCollectionFactory
	{
	private:
		std::unique_ptr<Collection::IdentifiableCollection<Node<NODE_DATA, EDGE_DATA>>> node_collection;
		
	public:
		NodeCollectionFactory();
		virtual ~NodeCollectionFactory();

		//構成後に用途に応じてこれらのいずれかを用いてIdentifiableCollectionを取得する想定
		std::unique_ptr<Collection::IdentifiableCollection< Node<const NODE_DATA, EDGE_DATA>>> get_static_node_collection();
		std::unique_ptr<Collection::IdentifiableCollection<Node<NODE_DATA, EDGE_DATA>>> get_updateable_node_collection();
		
		//接続関係構成用の各種メソッド


	};
}
