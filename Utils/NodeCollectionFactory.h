#pragma once
#include "BasicEdge.h"
#include "Edge.h"
#include "IdentifiableCollection.h"

namespace Graph 
{

	///<summary>
	/// ノードの生成，接続関係の構成を行った後IdentifiableCollectionを返すファクトリ抽象クラステンプレート
	/// create_nodes, set_connectivityを実装が必要．
	/// create_nodes, set_connectivityの順序で実行され，IdentifiableCollectionの内容を作成する．
	/// 作成後は，今後も更新を加えるかによってcreate_static_node_collectionとcreate_updateable_node_collectionを使い分けて取得する．
	/// 取得後はファクトリ内のコレクションはnullptrになるので注意
	/// また，NODE, EDGE, EDGE_DATAの型の整合性がとれていないとコンパイルできないので注意
	/// EDGEにはBasicEdgeかEdge<EDGE_DATA>の派生クラスのみ使用できます
	/// Connect系のメソッドがBasicEdgeで別動作，かつBasicEdge以外で別動作できないようになっているか確認が必要
	/// BasicEdgeを使う時はEDGE_DATAをnullptr_tにしてください
	///</summary>
	template <typename NODE, typename EDGE, typename EDGE_DATA>
	class NodeCollectionFactory
	{
	static_assert(std::is_base_of<Edge<EDGE_DATA>, EDGE>::value || std::is_same<BasicEdge, EDGE>::value, "template type EDGE is not derived from Edge<EDGE_DATA>");
	private:
		//コピー禁止
		NodeCollectionFactory(const NodeCollectionFactory& factory);
	
	protected:
		std::shared_ptr<Collection::IdentifiableCollection<Graph::node_id, NODE>> node_collection;
		virtual void create_nodes() = 0;
		virtual void set_connectivities() = 0;

		//接続関係構成用のメソッド
		//これらを用いてcreate_nodesとset_connectivitiesを実装する．
		bool add_node(std::shared_ptr<NODE> node);
		bool remove_node(const node_id& id);
		bool connect(const node_id& from, const node_id& to, std::shared_ptr<EDGE_DATA> data);
		bool connect_each_other(const node_id& node1, const node_id& node2, std::shared_ptr<EDGE_DATA> data);
		bool disconnect(const node_id& target, const node_id& from);
		void disconnect_each_other(const node_id& node1, const node_id& node2);

	public:
		NodeCollectionFactory();
		virtual ~NodeCollectionFactory();

		//構成後に用途に応じてこれらのいずれかを用いてIdentifiableCollectionを取得する想定
		//これらのメソッド取得後は，このファクトリが保持するnode_collectionは空に初期化し直される．
		std::shared_ptr<const Collection::IdentifiableCollection<long, NODE>> create_static_node_collection();
		std::shared_ptr<Collection::IdentifiableCollection<long, NODE>> create_updateable_node_collection();
	};

	
	#pragma region BasicEdge
	template <typename NODE>
	class NodeCollectionFactory<NODE, BasicEdge, nullptr_t>
	{
		bool connect(const node_id& from, const node_id& to);
		bool connect_each_other(const node_id& id1, const node_id& id2);
	};
	#pragma endregion EDGEがBasicEdgeの時のための部分特殊化
	
}

#include "NodeCollectionFactory.hpp"