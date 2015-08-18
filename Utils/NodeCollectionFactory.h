#pragma once
#include "Edge.h"
#include "Node.h"
#include "IdentifiableCollection.h"

namespace Graph 
{

	///<summary>
	/// ノードの生成，接続関係の構成を行った後IdentifiableCollectionを返すファクトリ抽象クラステンプレート
	/// create_nodes, set_connectivityを実装が必要．
	/// create_nodes, set_connectivityの順序で実行され，IdentifiableCollectionの内容を作成する．
	/// 作成後は，今後も更新を加えるかによってcreate_static_node_collectionとcreate_updateable_node_collectionを使い分けて取得する．
	/// 取得後はファクトリ内のコレクションは使用できなくなるので注意
	///</summary>
	template <typename NODE_DATA, typename EDGE>
	class NodeCollectionFactory
	{
	private:
		//コピー禁止
		NodeCollectionFactory(const NodeCollectionFactory& factory);
	
	protected:
		std::shared_ptr<Collection::IdentifiableCollection<std::shared_ptr<Node<NODE_DATA, EDGE>>>> node_collection;
		virtual void create_nodes() = 0;
		virtual void set_connectivities() = 0;

		//接続関係構成用のメソッド
		//これらを用いてcreate_nodesとset_connectivitiesを実装する．
		bool create_node(const node_id& id, std::shared_ptr<NODE_DATA> data);
		bool add_node(std::shared_ptr<Graph::Node<NODE_DATA, EDGE>> node);
		bool remove_node(node_id id);
		bool connect(node_id from, node_id to, EDGE data);
		bool connect_each_other(node_id node1, node_id node2, EDGE data);
		bool disconnect(node_id target, node_id from);
		void disconnect_each_other(node_id node1, node_id node2);
		
	public:
		NodeCollectionFactory();
		virtual ~NodeCollectionFactory();

		//構成後に用途に応じてこれらのいずれかを用いてIdentifiableCollectionを取得する想定
		//これらのメソッド取得後は，このファクトリが保持するnode_collectionは空に初期化し直される．
		std::shared_ptr<const Collection::IdentifiableCollection<std::shared_ptr<const Node<NODE_DATA, EDGE>>>> create_static_node_collection() const;
		std::shared_ptr<Collection::IdentifiableCollection<std::shared_ptr<Node<NODE_DATA, EDGE>>>> create_updateable_node_collection();
	};
}

#include "NodeCollectionFactory.hpp"